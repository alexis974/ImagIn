#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "../imagin.h"

#include "histogram.h"

struct Histogram *compute_histogram(struct Image *img)
{
    struct Histogram *histo = malloc(sizeof(struct Histogram));

    histo->red = calloc(256, sizeof(size_t));
    histo->green = calloc(256, sizeof(size_t));
    histo->blue = calloc(256, sizeof(size_t));
    histo->white = calloc(256, sizeof(size_t));

    size_t sum = 0;
    size_t tmp = 0;

    for (size_t i = 0; i < img->width * img->height; i++)
    {
        histo->red[img->data[i].red] += 1;
        histo->green[img->data[i].green] += 1;
        histo->blue[img->data[i].blue] += 1;

        sum = img->data[i].red + img->data[i].green + img->data[i].blue;
        tmp = sum / 3;

        histo->white[tmp] += 1;
    }

    return histo;
}

void free_histogram(struct Histogram *histo)
{
    free(histo->red);
    free(histo->green);
    free(histo->blue);
    free(histo->white);
    free(histo);
}

size_t histo_max_value(struct Histogram *histo)
{
    size_t max = 0;
    for (size_t i = 0; i < 256; i++)
    {
        if (histo->white[i] > max)
        {
            max = histo->white[i];
        }

    }

    return max;
}
