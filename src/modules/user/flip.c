#include <stdio.h>
#include <err.h>

#include "../../imagin.h"

#include "flip.h"

void vertical_flip(struct Image *img)
{
    if (!img)
    {
        errx(1, "vertical_flip: No image found");
    }

    size_t tmp;
    size_t offset = img->width;
    size_t index_left;
    size_t index_right;

    for (size_t y = 0; y < img->height ; y++)
    {
        for (size_t x = 0; x < (img->width / 2); x++)
        {
            index_left = y * offset + x;
            index_right = y * offset + (img->width - x) - 1;

            tmp = img->data[index_left].r;
            img->data[index_left].r = img->data[index_right].r;
            img->data[index_right].r = tmp;

            tmp = img->data[index_left].g;
            img->data[index_left].g = img->data[index_right].g;
            img->data[index_right].g = tmp;

            tmp = img->data[index_left].b;
            img->data[index_left].b = img->data[index_right].b;
            img->data[index_right].b = tmp;
        }
    }
}

void horizontal_flip(struct Image *img)
{
    if (!img)
    {
        errx(1, "horizontal_flip: No image found");
    }

    size_t tmp;
    size_t offset = img->width;
    size_t index_top;
    size_t index_bottom;

    for (size_t x = 0; x < img->width; x++)
    {
        for (size_t y = 0; y < (img->height / 2) ; y++)
        {
            index_top = y * offset + x;
            index_bottom = offset * (img->height - y) + x - offset;

            tmp = img->data[index_top].r;
            img->data[index_top].r = img->data[index_bottom].r;
            img->data[index_bottom].r = tmp;

            tmp = img->data[index_top].g;
            img->data[index_top].g = img->data[index_bottom].g;
            img->data[index_bottom].g = tmp;

            tmp = img->data[index_top].b;
            img->data[index_top].b = img->data[index_bottom].b;
            img->data[index_bottom].b = tmp;
        }
    }
}

void flip_both_axis(struct Image *img)
{
    if (!img)
    {
        errx(1, "flip_both_axis: No image found");
    }

    size_t tmp;
    size_t index_end = img->width * img->height - 1;

    for (size_t i = 0; i < index_end; i++, index_end--)
    {
        tmp = img->data[i].r;
        img->data[i].r = img->data[index_end].r;
        img->data[index_end].r = tmp;

        tmp = img->data[i].g;
        img->data[i].g = img->data[index_end].g;
        img->data[index_end].g = tmp;

        tmp = img->data[i].b;
        img->data[i].b = img->data[index_end].b;
        img->data[index_end].b = tmp;
    }
}

void flip(struct Image *img, size_t value)
{
    switch (value)
    {
    case 1:
        vertical_flip(img);
        break;
    case 2:
        horizontal_flip(img);
        break;
    case 3:
        flip_both_axis(img);
        break;
    default:
        break;
    }
}
