#include <stdio.h>
#include <err.h>
#include "../imagin.h"
#include "flip.h"

void vertical_flip(struct Image *img)
{
    unsigned char tmp;
    size_t offset = img->width;
    size_t index_left;
    size_t index_right;

    if (img)
    {
        for (size_t y = 0; y < img->height ; y++)
        {
            for (size_t x = 0; x < (img->width / 2); x++)
            {
                index_left = y * offset + x;
                index_right = y * offset + (img->width - x) ;

                tmp = img->data[index_left].red;
                img->data[index_left].red = img->data[index_right].red;
                img->data[index_right].red = tmp;

                tmp = img->data[index_left].green;
                img->data[index_left].green = img->data[index_right].green;
                img->data[index_right].green = tmp;

                tmp = img->data[index_left].blue;
                img->data[index_left].blue = img->data[index_right].blue;
                img->data[index_right].blue = tmp;
            }
        }
    }
}


void horizontal_flip(struct Image *img)
{
    unsigned char tmp;
    size_t offset = img->width;
    size_t index_top;
    size_t index_bottom;

    if (img)
    {
        for (size_t x = 0; x < img->width; x++)
        {
            for (size_t y = 0; y < (img->height / 2) ; y++)
            {
                index_top = y * offset + x;
                index_bottom = offset * (img->height - y) + x - offset;

                tmp = img->data[index_top].red;
                img->data[index_top].red = img->data[index_bottom].red;
                img->data[index_bottom].red = tmp;

                tmp = img->data[index_top].green;
                img->data[index_top].green = img->data[index_bottom].green;
                img->data[index_bottom].green = tmp;

                tmp = img->data[index_top].blue;
                img->data[index_top].blue = img->data[index_bottom].blue;
                img->data[index_bottom].blue = tmp;
            }
        }
    }
}


void flip_both_axis(struct Image *img)
{
    unsigned char tmp;
    size_t index_end = img->width * img->height;

    if (img)
    {
        for (size_t i = 0; i < index_end; i++, index_end--)
        {
            tmp = img->data[i].red;
            img->data[i].red = img->data[index_end].red;
            img->data[index_end].red = tmp;

            tmp = img->data[i].green;
            img->data[i].green = img->data[index_end].green;
            img->data[index_end].green = tmp;

            tmp = img->data[i].blue;
            img->data[i].blue = img->data[index_end].blue;
            img->data[index_end].blue = tmp;
        }
    }
}
