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

struct Matrix *testgauss(int radius, float sigma)
{
    struct Matrix *kernel = malloc(sizeof(struct Matrix));
    kernel->lines = 2 * radius + 1;
    kernel->cols = 2 * radius + 1;
    kernel->data = malloc(sizeof(float) * kernel->cols * kernel->lines);
    if (sigma){};
    for (int x = -radius; x < radius+1; x++)
    {
        for (int y = -radius; y < radius+1; y++)
        {
            kernel->data[(x+radius) * kernel->cols + (y+radius)] = (float)1/25;
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

    size_t radius = ceil((2 * sigma));

    struct Matrix *kernel = initGaussianKernel((int)radius, sigma);
    //struct Matrix *kernel = testgauss((int)radius, sigma);

    printf("Gaussian kernel:\n");
    printMatrix(kernel);
    size_t a = 0;

    for (size_t i = radius; i < img->height - radius; i++)
    {
        for (size_t j = radius; j < img->width - radius; j++)
        {
            float acc[3] = {0, 0, 0};

            for (size_t x = 0; x < kernel->lines; x++)
            {
                for (size_t y = 0; y < kernel->cols; y++)
                {
                    int in = i + x - radius;
                    int jn = j + y - radius;
                    struct Pixel pxl = img->data[in * img->width + jn];
                    acc[0] += pxl.red * kernel->data[x * kernel->cols + y];
                    acc[1] += pxl.green * kernel->data[x * kernel->cols + y];
                    acc[2] += pxl.blue * kernel->data[x * kernel->cols + y];
                }
            }

            img->data[i * img->width + j].red = (size_t)acc[0];
            img->data[i * img->width + j].green = (size_t)acc[1];
            img->data[i * img->width + j].blue = (size_t)acc[2];
        }
    }

    printf("a = %ld\n", a);
    printf("width = %ld\n", img->width);
    printf("\n------------------\n");
    freeMatrix(kernel);
}
