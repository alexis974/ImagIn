#include <stdio.h>
#include <err.h>
#include <math.h>

#include "../imagin.h"

void exposure(struct Image *img)
{
    if (img)
    {
        size_t maxvalue = pow(2, img->bit_depth-1);

        for (size_t i=0; i < img->width * img->height; i++)
        {
            img->data[i].red *= 2;
            if (img->data[i].red > maxvalue) //pixel value too big
            {
                img->data[i].red = maxvalue;
            }

            img->data[i].green *= 2;
            if (img->data[i].green > maxvalue) //pixel value too big
            {
                img->data[i].green = maxvalue;
            }

            img->data[i].blue *= 2;
            if (img->data[i].blue > maxvalue) //pixel value to big
            {
                img->data[i].blue = maxvalue;
            }
        }
    }
}
