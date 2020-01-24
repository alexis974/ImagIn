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

    range ++; //to use param still useless for now [WIP]

    size_t average;
    size_t s1 = 100;
    size_t s2 = 155;

    for (size_t i = 0; i < img->width * img->height; i++)
    {
        /*//red
        if (img->data[i].red < s1)
        {
            img->data[i].red = 0;
        }
        else if (s1 <= img->data[i].red && img->data[i].red <= s2)
        {
            img->data[i].red = img->bit_depth * ((img->data[i].red-s1) / (s2-s1));
        }
        else
        {
            img->data[i].red = img->bit_depth;
        }

        //green
        if (img->data[i].green < s1)
        {
            img->data[i].green = 0;
        }
        else if (s1 <= img->data[i].green && img->data[i].green <= s2)
        {
            img->data[i].green = img->bit_depth * ((img->data[i].green-s1) / (s2-s1));
        }
        else
        {
            img->data[i].green = img->bit_depth;
        }

        //blue
        if (img->data[i].blue < s1)
        {
            img->data[i].blue = 0;
        }
        else if (s1 <= img->data[i].blue && img->data[i].blue <= s2)
        {
            img->data[i].blue = img->bit_depth * ((img->data[i].blue-s1) / (s2-s1));
        }
        else
        {
            img->data[i].blue = img->bit_depth;
        }*/

        average = (img->data[i].red + img->data[i].green + img->data[i].blue)/3;

        if (average < s1)
        {
            img->data[i].red = 0;
            img->data[i].green = 0;
            img->data[i].blue = 0;
        }
        else if (s1 <= average && average <= s2)
        {
            //img->data[i].red = range * (img->data[i].red - mid) + mid;
            //img->data[i].green = range * (img->data[i].green - mid) + mid;
            //img->data[i].blue = range * (img->data[i].blue - mid) + mid;
            img->data[i].red = img->bit_depth * ((img->data[i].red-s1) / (s2-s1));
            img->data[i].green = img->bit_depth * ((img->data[i].green-s1) / (s2-s1));
            img->data[i].blue = img->bit_depth * ((img->data[i].blue-s1) / (s2-s1));
        }
        else
        {
            img->data[i].red = img->bit_depth;
            img->data[i].green = img->bit_depth;
            img->data[i].blue = img->bit_depth;
        }
    }
}
