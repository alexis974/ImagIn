#include <stdio.h>
#include <err.h>

#include "../../imagin.h"

#include "invert.h"

void invert(struct Image *img)
{
    if (!img)
    {
        errx(1, "invert: No image found");
    }

    for (size_t i = 0; i < img->width * img->height; i++)
    {
        img->data[i].r = img->bit_depth - img->data[i].r;
        img->data[i].g = img->bit_depth - img->data[i].g;
        img->data[i].b = img->bit_depth - img->data[i].b;
    }
}
