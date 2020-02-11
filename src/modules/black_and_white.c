#include <stdio.h>
#include <err.h>

#include "../imagin.h"

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
    unsigned char tmp;

    for (i = 0; i < img->width * img->height; i++)
    {
        sum = img->data[i].red + img->data[i].green + img->data[i].blue;
        tmp = sum / 3;

        img->data[i].red = tmp;
        img->data[i].green = tmp;
        img->data[i].blue = tmp;
    }
}
