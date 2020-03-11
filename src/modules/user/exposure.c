#include <stdio.h>
#include <err.h>
#include <math.h>

#include "../../imagin.h"

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
        img->data[i].r = (img->data[i].r * scale) > img->bit_depth ?
            img->bit_depth : img->data[i].r * scale;

        img->data[i].g = (img->data[i].g * scale) > img->bit_depth ?
            img->bit_depth : img->data[i].g * scale;

        img->data[i].b = (img->data[i].b * scale) > img->bit_depth ?
            img->bit_depth : img->data[i].b * scale;
    }
}
