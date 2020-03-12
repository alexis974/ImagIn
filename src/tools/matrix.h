#ifndef MATRIX_H
#define MATRIX_H

struct Matrix
{
    size_t lines;
    size_t cols;
    float data[];
};

void freeMatrix(struct Matrix matrix);

#endif
