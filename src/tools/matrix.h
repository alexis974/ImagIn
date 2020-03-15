#ifndef MATRIX_H
#define MATRIX_H

struct Matrix
{
    size_t lines;
    size_t cols;
    double *data;
};

void freeMatrix(struct Matrix *m);

void printMatrix(struct Matrix *m);

#endif /* MATRIX_H */
