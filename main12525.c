/* Done by : Majd And Suhaib
 BZU , Dr.Yazan Abu Farha */
// 5/1/2025
// 25*25 sudoku , check valid or not


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>    // for QueryPerformanceCounter
#include <pthread.h>

#define SIZE        25
#define BLOCK       5
#define THREADS_27  27
#define THREADS_75   75

// ---------------------------- prototypes
int  checker(int unit[]);
int  validate_naive();
void *check_rows27(void *arg);
void *check_cols27(void *arg);
void *check_box27(void *arg);
int  validate_27thread();
void *check_row75(void *arg);
void *check_col75(void *arg);
void *check_box75(void *arg);
int  validate_75thread();

// ---------------------------- globals
  int puzzle[SIZE][SIZE] = {
    { 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 },
    { 6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5 },
    {11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10 },
    {16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 },
    {21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 },
    { 2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1 },
    { 7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6 },
    {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11 },
    {17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16 },
    {22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 },
    { 3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2 },
    { 8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7 },
    {13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12 },
    {18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17 },
    {23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 },
    { 4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3 },
    { 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8 },
    {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13 },
    {19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18 },
    {24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 },
    { 5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4 },
    {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9 },
    {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14 },
    {20, 21, 22, 23, 24, 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 },
    {25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 }
};


int valid_27[THREADS_27];

int valid_75[THREADS_75];

      typedef struct {
    int thread_id;

    int row;
    int col;
  }       param_t;

                              // ---------------------------- main
int main() {
    LARGE_INTEGER freq, t1, t2;

QueryPerformanceFrequency(&freq);

                                           // Na ve
       QueryPerformanceCounter(&t1);


         int ok0 = validate_naive();
QueryPerformanceCounter(&t2);
double time_naive = (double)(t2.QuadPart - t1.QuadPart) / freq.QuadPart;
          if (ok0)
           printf("Na ve     : valid,     time = %f s\n", time_naive);
    else
           printf("Na ve     : NOT valid, time = %f s\n", time_naive);

                                              // 27-thread
           QueryPerformanceCounter(&t1);
int ok1 = validate_27thread();
                  QueryPerformanceCounter(&t2);

double time_27 = (double)(t2.QuadPart - t1.QuadPart) / freq.QuadPart;
       if (ok1)
           printf("27-thread : valid,     time = %f s\n", time_27);
else
            printf("27-thread : NOT valid, time = %f s\n", time_27);

                                           // 75-thread
        QueryPerformanceCounter(&t1);
int ok2 = validate_75thread();
     QueryPerformanceCounter(&t2);
    double time_75 = (double)(t2.QuadPart - t1.QuadPart) / freq.QuadPart;
          if (ok2)
        printf("75-thread : valid,     time = %f s\n", time_75);
    else
          printf("75-thread : NOT valid, time = %f s\n", time_75);

return 0;
}

                         // ---------------------------- implementations

int checker(int unit[]) {
      int seen[SIZE+1] = {0};
    for (int i = 0; i < SIZE; i++)
        {
int v = unit[i];
          if (v < 1 || v > SIZE || seen[v]) return 0;
        seen[v] = 1;
    }
       return 1;
}

int validate_naive()
{
    int tmp[SIZE];

    // rows
    for (int r = 0; r < SIZE; r++) {
           for (int c = 0; c < SIZE; c++)
               tmp[c] = puzzle[r][c];
        if (!checker(tmp)) return 0;

    }
    // columns
    for (int c = 0; c < SIZE; c++) {
        for (int r = 0; r < SIZE; r++)
                tmp[r] = puzzle[r][c];
        if (!checker(tmp)) return 0;
    }

    // BLOCK BLOCK blocks
    for (int br = 0; br < SIZE; br += BLOCK) {
        for (int bc = 0; bc < SIZE; bc += BLOCK) {
                    int idx = 0;
            for (int r = 0; r < BLOCK; r++)
                    for (int c = 0; c < BLOCK; c++)
                     tmp[idx++] = puzzle[br + r][bc + c];
            if (!checker(tmp)) return 0;


             }
     }
          return 1;
}

void* check_rows27(void* arg)
{
    param_t* p = (param_t*)arg;
             int id = p->thread_id, tmp[SIZE];
    for (int r = 0; r < SIZE; r++) {
                  for (int c = 0; c < SIZE; c++)
             tmp[c] = puzzle[r][c];
        if (!checker(tmp)) {
                    valid_27[id] = 0;
            pthread_exit(NULL);
        }
    }


      valid_27[id] = 1;

    pthread_exit(NULL);
}

void* check_cols27(void* arg) {
param_t* p = (param_t*)arg;
       int id = p->thread_id, tmp[SIZE];

    for (int c = 0; c < SIZE; c++) {
           for (int r = 0; r < SIZE; r++)
                   tmp[r] = puzzle[r][c];
                 if (!checker(tmp)) {
               valid_27[id] = 0;
            pthread_exit(NULL);
        }

    }
    valid_27[id] = 1;

    pthread_exit(NULL);
}

void* check_box27(void* arg) {

param_t* p = (param_t*)arg;

int id = p->thread_id, br = p->row, bc = p->col, tmp[SIZE], idx = 0;
    for (int r = 0; r < BLOCK; r++)

        for (int c = 0; c < BLOCK; c++)
            tmp[idx++] = puzzle[br + r][bc + c];
         valid_27[id] = checker(tmp);

    pthread_exit(NULL);

}

int validate_27thread() {
            pthread_t threads[THREADS_27];
    param_t params[THREADS_27];

int t = 0;

    // thread 0 for all rows
    params[t].thread_id = t;

        pthread_create(&threads[t], NULL, check_rows27, &params[t]);
t++;

    // thread 1 for all columns
        params[t].thread_id = t;
      pthread_create(&threads[t], NULL, check_cols27, &params[t]);


    t++;
    // threads 2..26 for each BLOCK BLOCK block
    for (int br = 0; br < SIZE; br += BLOCK) {
        for (int bc = 0; bc < SIZE; bc += BLOCK) {

        params[t].thread_id = t;
    params[t].row       = br;

               params[t].col       = bc;
             pthread_create(&threads[t], NULL, check_box27, &params[t]);

            t++;
        }

    }
    for (int i = 0; i < THREADS_27; i++)
           pthread_join(threads[i], NULL);

    for (int i = 0; i < THREADS_27; i++)
          if (!valid_27[i]) return 0;

    return 1;
}

void* check_row75(void* arg) {

      param_t* p = (param_t*)arg;

int id = p->thread_id, r = p->row, tmp[SIZE];

    for (int c = 0; c < SIZE; c++)
          tmp[c] = puzzle[r][c];
    valid_75[id] = checker(tmp);

             pthread_exit(NULL);
}

void* check_col75(void* arg) {

    param_t* p = (param_t*)arg;

           int id = p->thread_id, c = p->col, tmp[SIZE];

    for (int r = 0; r < SIZE; r++)
          tmp[r] = puzzle[r][c];

    valid_75[id] = checker(tmp);

            pthread_exit(NULL);
}

void* check_box75(void* arg) {

    param_t* p = (param_t*)arg;

    int id = p->thread_id, br = p->row, bc = p->col, tmp[SIZE], idx = 0;
for (int r = 0; r < BLOCK; r++)

        for (int c = 0; c < BLOCK; c++)

             tmp[idx++] = puzzle[br + r][bc + c];
     valid_75[id] = checker(tmp);


    pthread_exit(NULL);
}

int validate_75thread() {

    pthread_t threads[THREADS_75];

              param_t params[THREADS_75];
    int t = 0;


    // threads 0..24 for one row each
    for (int r = 0; r < SIZE; r++) {
params[t].thread_id = t;
           params[t].row       = r;

            pthread_create(&threads[t], NULL, check_row75, &params[t]);
        t++;

    }
    // threads 25..49 for one column each
    for (int c = 0; c < SIZE; c++) {

            params[t].thread_id = t;
    params[t].col       = c;
             pthread_create(&threads[t], NULL, check_col75, &params[t]);
        t++;
    }
    // threads 50..74 for each BLOCK BLOCK block
    for (int br = 0; br < SIZE; br += BLOCK) {
        for (int bc = 0; bc < SIZE; bc += BLOCK) {

    params[t].thread_id = t;
               params[t].row       = br;
        params[t].col       = bc;
        pthread_create(&threads[t], NULL, check_box75, &params[t]);
             t++;

        }
    }



    for (int i = 0; i < THREADS_75; i++)
          pthread_join(threads[i], NULL);
    for (int i = 0; i < THREADS_75; i++)

        if (!valid_75[i]) return 0;

    return 1;
}
