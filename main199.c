/* Done by : Majd And Suhaib
 BZU , Dr.Yazan Abu Farha */
// 5/1/2025
// 9*9 sudoku , check valid or not
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <pthread.h>

#define SIZE 9
#define THREADS_11 11
#define THREADS_27 27

// ----------------------------prototypes
int checker(int unit[]);

int validate_naive();

void* check_rows11(void* arg);
void* check_cols11(void* arg);
void* check_box11(void* arg);
int validate_11thread();

void* check_row27(void* arg);
void* check_col27(void* arg);

void* check_box27(void* arg);
int validate_27thread();

// -----------------------------global variables
int puzzle[SIZE][SIZE] = {
    {5,3,4, 6,7,8, 9,1,2},
    {6,7,2, 1,9,5, 3,4,8},
    {1,9,8, 3,4,2, 5,6,7},
    {8,5,9, 7,6,1, 4,2,3},
    {4,2,6, 8,5,3, 7,9,1},
    {7,1,3, 9,2,4, 8,5,6},
    {9,6,1, 5,3,7, 2,8,4},
    {2,8,7, 4,1,9, 6,3,5},
    {3,4,5, 2,8,6, 1,7,9}
};

int valid_11[THREADS_11];
int valid_27[THREADS_27];

typedef struct {
    int thread_id;
    int row;
    int col;
} param_t;

// -------------------------- main of process
int main() {
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);

    QueryPerformanceCounter(&start);

    int ok_naive = validate_naive();
    QueryPerformanceCounter(&end);


     double time_naive = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    if (ok_naive)
        printf("Na ve  : valid, time = %f s\n", time_naive);
    else
        printf("Na ve   : NOT valid, time = %f s\n", time_naive);

    QueryPerformanceCounter(&start);
    int ok_11 = validate_11thread();



     QueryPerformanceCounter(&end);
    double time_11 = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;

    if (ok_11)
        printf("11-thread  : valid, time = %f s\n", time_11);
    else
        printf("11-thread  : NOT valid, time = %f s\n", time_11);




    QueryPerformanceCounter(&start);
    int ok_27 = validate_27thread();

     QueryPerformanceCounter(&end);
    double time_27 = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    if (ok_27)
        printf("27-thread  : valid, time = %f s\n", time_27);
    else
        printf("27-thread  : NOT valid, time = %f s\n", time_27);

    return 0;
}

// ----------- implementations

int checker(int temp[]) {
    int seen[SIZE + 1] = {0};

    for (int i = 0; i < SIZE; i++) {


        int value = temp[i];


         if (value < 1 || value > SIZE) return 0;


          if (seen[value]) return 0;

        seen[value] = 1;
    }
    return 1;
}

int validate_naive() {
    int temp[SIZE];

    for (int row = 0; row < SIZE; row++) {
         for (int col = 0; col < SIZE; col++)

            temp[col] = puzzle[row][col];

        if (!checker(temp)) return 0;
    }

    for (int col = 0; col < SIZE; col++) {
        for (int row = 0; row < SIZE; row++)
            temp[row] = puzzle[row][col];


        if (!checker(temp)) return 0;
    }

    for (int boxRow = 0; boxRow < SIZE; boxRow += 3) {
        for (int boxCol = 0; boxCol < SIZE; boxCol += 3) {
            int index = 0;
            for (int rowOffset = 0; rowOffset < 3; rowOffset++)
                for (int colOffset = 0; colOffset < 3; colOffset++)
                    temp[index++] = puzzle[boxRow + rowOffset][boxCol + colOffset];
            if (!checker(temp)) return 0;
        }
    }

    return 1;
}

void* check_rows11(void* arg) {
    param_t* input = (param_t*)arg;

    int threadIndex = input->thread_id;


     int temp[SIZE];

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++)
            temp[col] = puzzle[row][col];


        if (!checker(temp)) {
            valid_11[threadIndex] = 0;
            pthread_exit(NULL);
        }
    }

    valid_11[threadIndex] = 1;


    pthread_exit(NULL);
}

 void* check_cols11(void* arg) {
    param_t* input = (param_t*)arg;


     int threadIndex = input->thread_id;

               int temp[SIZE];

    for (int col = 0; col < SIZE; col++) {
        for (int row = 0; row < SIZE; row++)
            temp[row] = puzzle[row][col];
        if (!checker(temp)) {
            valid_11[threadIndex] = 0;
            pthread_exit(NULL);
        }
    }

    valid_11[threadIndex] = 1;

     pthread_exit(NULL);
}

void* check_box11(void* arg) {
        param_t* input = (param_t*)arg;

    int threadIndex = input->thread_id;

    int startRow = input->row, startCol = input->col;

    int temp[SIZE], index = 0;

    for (int rowOffset = 0; rowOffset < 3; rowOffset++)

        for (int colOffset = 0; colOffset < 3; colOffset++)

            temp[index++] = puzzle[startRow + rowOffset][startCol + colOffset];

    valid_11[threadIndex] = checker(temp);

    pthread_exit(NULL);
}

 int validate_11thread() {

    pthread_t threads[THREADS_11];

    param_t params[THREADS_11];

    params[0].thread_id = 0;
    pthread_create(&threads[0], NULL, check_rows11, &params[0]);


    params[1].thread_id = 1;
    pthread_create(&threads[1], NULL, check_cols11, &params[1]);

    int threadIndex = 2;
    for (int row = 0; row < SIZE; row += 3) {
        for (int col = 0; col < SIZE; col += 3) {
            params[threadIndex].thread_id = threadIndex;

            params[threadIndex].row = row;

            params[threadIndex].col = col;


            pthread_create(&threads[threadIndex], NULL, check_box11, &params[threadIndex]);
               threadIndex++;
        }
    }

    for (int i = 0; i < THREADS_11; i++)


        pthread_join(threads[i], NULL);

    for (int i = 0; i < THREADS_11; i++)
        if (!valid_11[i]) return 0;
    return 1;
}

void* check_row27(void* arg) {
    param_t* input = (param_t*)arg;

      int threadIndex = input->thread_id;
     int row = input->row;

    int temp[SIZE];

    for (int col = 0; col < SIZE; col++)
        temp[col] = puzzle[row][col];

    valid_27[threadIndex] = checker(temp);

    pthread_exit(NULL);
}

void* check_col27(void* arg) {
    param_t* input = (param_t*)arg;
    int threadIndex = input->thread_id;

    int col = input->col;




     int temp[SIZE];

    for (int row = 0; row < SIZE; row++)
        temp[row] = puzzle[row][col];

    valid_27[threadIndex] = checker(temp);
    pthread_exit(NULL);
}

void* check_box27(void* arg) {

    param_t* input = (param_t*)arg;

     int threadIndex = input->thread_id;


     int startRow = input->row, startCol = input->col;

    int temp[SIZE], index = 0;

    for (int rowOffset = 0; rowOffset < 3; rowOffset++)
        for (int colOffset = 0; colOffset < 3; colOffset++)
            temp[index++] = puzzle[startRow + rowOffset][startCol + colOffset];

    valid_27[threadIndex] = checker(temp);

    pthread_exit(NULL);
}

int validate_27thread() {
    pthread_t threads[THREADS_27];
             param_t params[THREADS_27];

     int threadIndex = 0;

    for (int row = 0; row < SIZE; row++) {
        params[threadIndex].thread_id = threadIndex;

         params[threadIndex].row = row;


        pthread_create(&threads[threadIndex], NULL, check_row27, &params[threadIndex]);
        threadIndex++;
    }

    for (int col = 0; col < SIZE; col++) {
        params[threadIndex].thread_id = threadIndex;


        params[threadIndex].col = col;

          pthread_create(&threads[threadIndex], NULL, check_col27, &params[threadIndex]);
        threadIndex++;
    }

    for (int row = 0; row < SIZE; row += 3) {
        for (int col = 0; col < SIZE; col += 3) {
            params[threadIndex].thread_id = threadIndex;

            params[threadIndex].row = row;

            params[threadIndex].col = col;



            pthread_create(&threads[threadIndex], NULL, check_box27, &params[threadIndex]);
            threadIndex++;
        }
    }

    for (int i = 0; i < THREADS_27; i++)
        pthread_join(threads[i], NULL);


    for (int i = 0; i < THREADS_27; i++)

        if (!valid_27[i]) return 0;

    return 1;
}
