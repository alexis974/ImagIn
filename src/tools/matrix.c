#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

void freeMatrix(struct Matrix *m)
{
    free(m->data);
    free(m);
}

void printMatrix(struct Matrix *m)
{
    for (size_t i = 0; i < m->lines*m->cols; i++)
    {
        printf("%f ", m->data[i]);
        if ((i+1) % m->cols == 0)
        {
            printf("\n");
        }
    }
}
