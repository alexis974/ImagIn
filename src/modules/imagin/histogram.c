#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "../../imagin.h"

#include "histogram.h"

struct Histogram *compute_histogram(struct Image *img)
{
    struct Histogram *histo = malloc(sizeof(struct Histogram));

    size_t depth = img->bit_depth + 1;

    histo->r = calloc(depth, sizeof(size_t));
    histo->g = calloc(depth, sizeof(size_t));
    histo->b = calloc(depth, sizeof(size_t));
    histo->w = calloc(depth, sizeof(size_t));

    size_t sum = 0;
    size_t tmp = 0;

    for (size_t i = 0; i < img->width * img->height; i++)
    {
        histo->r[img->data[i].r] += 1;
        histo->g[img->data[i].g] += 1;
        histo->b[img->data[i].b] += 1;

        sum = img->data[i].r + img->data[i].g + img->data[i].b;
        tmp = sum / 3;

        histo->w[tmp] += 1;
    }

    return histo;
}

void free_histogram(struct Histogram *histo)
{
    free(histo->r);
    free(histo->g);
    free(histo->b);
    free(histo->w);
    free(histo);
}

size_t histo_max_value(struct Histogram *histo, size_t depth)
{
    size_t max = 0;
    for (size_t i = 0; i < depth; i++)
    {
        if (histo->w[i] > max)
        {
            max = histo->w[i];
        }

    }

    return max;
}
