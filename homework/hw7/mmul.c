#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

// Search TODO to find the locations where code needs to be completed

#define     NUM_THREADS     2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;

static void * thread_main(void * p_arg)
{
    // TODO
    int i,j,rowlowerbound,rowupperbound;
    thread_arg_t *matrix = (thread_arg_t*) p_arg;

    rowlowerbound = (matrix->m->nrows / NUM_THREADS) * matrix->id;
    rowupperbound = (matrix->m->nrows / NUM_THREADS) * (matrix->id + 1);

    for (i = rowlowerbound; i < rowupperbound; i++) {
        for (j = 0; j < matrix->n->ncols; j++) {
            TElement sum = (TElement)0;
            for (int k = 0; k < matrix->m->ncols; k++)
                sum += matrix->m->data[i][k] * matrix->n->data[k][j];
            matrix->t->data[i][j] = sum;
        }
    }
    return NULL;
}

/* Return the sum of two matrices.
 *
 * If any pthread function fails, report error and exit. 
 * Return NULL if anything else is wrong.
 *
 * Similar to mulMatrix, but with multi-threading.
 */
TMatrix * mulMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->ncols != n->nrows )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, n->ncols);
    if (t == NULL)
        return t;

    // TODO
    int rc;
    pthread_t threads[NUM_THREADS];
    thread_arg_t thread_data[NUM_THREADS];
    //int num_threads = (m->nrows < NUM_THREADS) ? m->nrows : NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++){
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

    for (int i = 0; i < NUM_THREADS; i++) {
        rc = pthread_join(threads[i], NULL);
        if (rc) {
            printf("Error\n");
            exit(-1);
        }
    }
    return t;
}
