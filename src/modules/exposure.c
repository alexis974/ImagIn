#include <stdio.h>
#include <err.h>
#include <math.h>

#include "../imagin.h"

void exposure(struct Image *img)
{
    if (img)
    {
        size_t threshold = pow(2, img->bit_depth) - 1;

        for (size_t i=0; i < img->width * img->height; i++)
        {
            img->data[i].red *= 2;
            if (img->data[i].red > threshold) //pixel value too big
            {
                img->data[i].red = threshold;
            }

            img->data[i].green *= 2;
            if (img->data[i].green > threshold) //pixel value too big
            {
                img->data[i].green = threshold;
            }

            img->data[i].blue *= 2;
            if (img->data[i].blue > threshold) //pixel value to big
            {
                img->data[i].blue = threshold;
            }
        }
    }
}
