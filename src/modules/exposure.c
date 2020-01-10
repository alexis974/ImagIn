#include <stdio.h>
#include <err.h>
#include <math.h>

#include "../imagin.h"
#include "exposure.h"

void exposure(struct Image *img, float EV)
{
    if (!img)
    {
        errx(1, "exposure: No image found ");
    }

    float scale = pow(2, EV);
    for (size_t i = 0; i < img->width * img->height; i++)
    {
        img->data[i].red = (img->data[i].red * scale) > img->bit_depth ?
            img->bit_depth : img->data[i].red * scale;

        img->data[i].green = (img->data[i].green * scale) > img->bit_depth ?
            img->bit_depth : img->data[i].green * scale;

        img->data[i].blue = (img->data[i].blue * scale) > img->bit_depth ?
            img->bit_depth : img->data[i].blue * scale;
    }
}
