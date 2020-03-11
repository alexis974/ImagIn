#include <stdio.h>
#include <err.h>

#include "../../imagin.h"

#include "black_and_white.h"

// TODO : Coding style : 2.8 Fct name no upper case
void simple_BW(struct Image *img)
{
    if (!img)
    {
        errx(1, "simple_BW: No image found");
    }

    size_t i;
    size_t sum;
    size_t tmp;

    for (i = 0; i < img->width * img->height; i++)
    {
        sum = img->data[i].r + img->data[i].g + img->data[i].b;
        tmp = sum / 3;

        img->data[i].r = tmp;
        img->data[i].g = tmp;
        img->data[i].b = tmp;
    }
}
