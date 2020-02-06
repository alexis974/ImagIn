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

    for (size_t i = 0; i < img->width * img->height; i++)
    {
        histo->red[img->data[i].red] += 1;
        histo->green[img->data[i].green] += 1;
        histo->blue[img->data[i].blue] += 1;
    }

    return histo;
}

void free_histogram(struct Histogram *histo)
{
    free(histo->red);
    free(histo->green);
    free(histo->blue);
    free(histo);
}

size_t histo_max_value(struct Histogram *histo)
{
    size_t max = 0;
    for (size_t i = 0; i < 256; i++)
    {
        if (histo->red[i] > max)
        {
            max = histo->red[i];
        }

        if (histo->green[i] > max)
        {
            max = histo->green[i];
        }

        if (histo->blue[i] > max)
        {
            max = histo->blue[i];
        }
    }

    return max;
}
