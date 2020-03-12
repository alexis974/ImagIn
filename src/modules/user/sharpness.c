#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>

#include "../../imagin.h"

#include "../../tools/matrix.h"

#include "sharpness.h"

struct Matrix initGaussianKernel(size_t radius, float sigma)
{
    float factor = 0;
    struct Matrix kernel = malloc(sizeof(struct Matrix));
    kernel.lines = radius;
    kernel.cols = radius;
    kernel.data = malloc(kernel.lines * kernel.cols * sizeof(float));

    for (int x = -radius; x <= radius; x++)
    {
        for (int y = -radius; x <= radius; y++)
        {
            float e = exp(-(x*x + y*y) / (2*sigma*sigma));
            factor += e;
            kernel.data[x * cols + j];
        }
    }

    for (int x = -radius; x <= radius; x++)
    {
        for (int y = -radius; x <= radius; y++)
        {
            kernel.data[x * cols + j] /= factor;
        }
    }

    return kernel;
}

void blur(struct Image *img, float sigma)
{
    if (!img)
    {
        errx(1, "blur: No image found");
    }

    if (sigma <= 0)
    {
        return;
    }

   struct Matrix kernel = initGaussianKernel(radius, sigma);


























