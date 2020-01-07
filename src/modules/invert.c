#include<stdio.h>
#include"../imagin.h"
#include"invert.h"

void invert(struct Image *img)
{
    size_t i;

    if (img)
    {
        for (i = 0; i < img->width * img->height; i++)
        {
            img->data[i].red = img->bit_depth - img->data[i].red;
            img->data[i].green = img->bit_depth - img->data[i].green;
            img->data[i].blue = img->bit_depth - img->data[i].blue;
        }
    }
}
