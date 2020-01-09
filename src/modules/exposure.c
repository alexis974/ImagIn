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
            img->data[i].red = (img->data[i].red * 2) > img->bit_depth ?
                img->bit_depth : 0;
            //img->data[i].red = (img->data[i].red + 200) > img->bit_depth ?
            //    img->bit_depth : img->data[i].red *= 2;

            img->data[i].green = (img->data[i].green * 2) > img->bit_depth ?
                img->bit_depth : 0;
            //img->data[i].green = (img->data[i].green + 200) > img->bit_depth ?
            //    img->bit_depth : img->data[i].green *= 2;

            img->data[i].blue = (img->data[i].blue * 2) > img->bit_depth ?
                img->bit_depth : 0;
            //img->data[i].blue = (img->data[i].blue + 200) > img->bit_depth ?
            //    img->bit_depth : img->data[i].blue *= 2;

           printf("red: %d, green: %d, blue: %d\n", img->data[i].red, img->data[i].green, img->data[i].blue);
        }
    }
}
