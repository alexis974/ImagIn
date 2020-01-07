#include<stdio.h>
#include"../imagin.h"
#include"black_and_white.h"

void simple_BW(struct Image *img)
{
    size_t i;
    size_t sum;
    unsigned char tmp;

    if (img)
    {
        for (i = 0; i < img->width * img->height; i++)
        {
            sum = img->data[i].red + img->data[i].green + img->data[i].blue;
            tmp = sum / 3;

            img->data[i].red = tmp;
            img->data[i].green = tmp;
            img->data[i].blue = tmp;
        }
    }
}
