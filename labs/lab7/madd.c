#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

#define     NUM_THREADS     2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;

/* the main function of threads */
static void * thread_main(void * p_arg)
{
    // TODO
    int i,j;
    thread_arg_t* matrix = (thread_arg_t*) p_arg;
    for (i = 0; i < matrix->m->nrows; i++) {
        for (j = 0; j < matrix->m->ncols; j++) {
            if (matrix->id == 0) {
                if (i % 2 == 0) {
                    matrix->t->data[i][j] = matrix->m->data[i][j] + matrix->n->data[i][j];
                }
            }
            if (matrix->id == 1) {
                if (i % 2 != 0) {
                    matrix->t->data[i][j] = matrix->m->data[i][j] + matrix->n->data[i][j];
                }
            }
        }
    }
    return NULL;
}

/* Return the sum of two matrices. The result is in a newly creaed matrix. 
 *
 * If a pthread function fails, report error and exit. 
 * Return NULL if something else is wrong.
 *
 * Similar to addMatrix, but this function uses 2 threads.
 */
TMatrix * addMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->nrows != n->nrows || m->ncols != n->ncols )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, m->ncols);
    if (t == NULL)
        return t;

    int rc;

    pthread_t threads[NUM_THREADS];
    thread_arg_t thread_data[NUM_THREADS];
    int num_threads = (m->nrows < NUM_THREADS) ? m->nrows : NUM_THREADS;
    for (int i = 0; i<num_threads; i++) {
        thread_data[i].id = i;
        thread_data[i].t = t;
        thread_data[i].m = m;
        thread_data[i].n = n;
        rc = pthread_create(&threads[i], NULL, thread_main, &thread_data[i]);
        if (rc) {
            printf("Error\n");
            exit(-1);
        }
    }
    for (int i = 0; i<num_threads; i++) {
        rc = pthread_join(threads[i], NULL);
        if (rc) {
            printf("Error\n");
            exit(-1);
        }
    }

    return t;
}
