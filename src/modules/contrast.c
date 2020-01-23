#include <stdio.h>
#include <err.h>

#include "../imagin.h"

#include "contrast.h"

/*
** Modify contrast of given image
** 'range' is in range (0.1, 2.0)
*/
void contrast(struct Image *img, float range)
{
    if (!img)
    {
        errx(1, "contrast: no image found");
    }

    size_t mid = img->bit_depth / 2;
    if (img->bit_depth % 2 == 1)
    {
        mid ++;
    }

    size_t average;
    size_t s1 = 100;
    size_t s2 = 150;

    for (size_t i = 0; i < img->width * img->height; i++)
    {
        average = (img->data[i].red + img->data[i].green + img->data[i].blue)/3;
        if (s1 < average && average < s2)
        {
            img->data[i].red = range * (img->data[i].red - mid) + mid;
            img->data[i].green = range * (img->data[i].green - mid) + mid;
            img->data[i].blue = range * (img->data[i].blue - mid) + mid;
        }
    }
}
