#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

void freeMatrix(struct Matrix matrix)
{
    free(matrix.data);
    free(matrix);
}
