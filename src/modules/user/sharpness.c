#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>

#include "../../imagin.h"

#include "../../tools/matrix.h"

#include "sharpness.h"

struct Matrix *initGaussianKernel(int radius, float sigma)
{
    float factor = 0;
    struct Matrix *kernel = malloc(sizeof(struct Matrix));
    kernel->lines = 2 * radius + 1;
    kernel->cols = 2 * radius + 1;
    kernel->data = malloc(sizeof(float) * kernel->cols * kernel->lines);

    for (int x = -radius; x < radius+1; x++)
    {
        for (int y = -radius; y < radius+1; y++)
        {
            float e = exp(-(x*x + y*y) / (2*sigma*sigma));
            factor += e;
            kernel->data[(x+radius) * kernel->cols + (y+radius)] = e;
        }
    }

    for (int x = -radius; x < radius + 1; x++)
    {
        for (int y = -radius; y <= radius + 1; y++)
        {
            kernel->data[(x+radius) * kernel->cols + (y+radius)] /= factor;
        }
    }

    return kernel;
}

void blur(struct Image *img, float sigma)
{
    printf("\n------------------\n");
    if (!img)
    {
        errx(1, "blur: No image found");
    }

    if (sigma <= 0)
    {
        return;
    }

    int radius = (5 * sigma) / 2;

    struct Matrix *kernel = initGaussianKernel(radius, sigma);

    printf("Gaussian kernel:\n");
    printMatrix(kernel);

    printf("\n------------------\n");
    freeMatrix(kernel);
}
























