#include <stdio.h>
#include <err.h>
#include <math.h>

#include "../imagin.h"

void exposure(struct Image *img)
{
    if (img)
    {
        for (size_t i = 0; i < img->width * img->height; i++)
        {
            img->data[i].red += 200;
            if (img->data[i].red > img->bit_depth) //pixel value too big
            {
                img->data[i].red = img->bit_depth;
            }

            img->data[i].green += 200;
            if (img->data[i].green > img->bit_depth) //pixel value too big
            {
                img->data[i].green = img->bit_depth;
            }

            img->data[i].blue += 255;
            if (img->data[i].blue > img->bit_depth) //pixel value to big
            {
                img->data[i].blue = img->bit_depth;
            }

            printf("red: %d, green: %d, blue: %d\n", img->data[i].red, img->data[i].green, img->data[i].blue);
        }
    }
}
