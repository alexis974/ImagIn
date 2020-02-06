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
