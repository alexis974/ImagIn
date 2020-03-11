#include <stdio.h>
#include <err.h>

#include "../../imagin.h"

void copy_img(struct Image *img_src, struct Image *img_dst)
{
    if (!img_src || !img_dst)
    {
        errx(1, "copy_img: No image found");
    }

    if (img_src->width != img_dst->width || img_src->height != img_dst->height)
    {
        errx(1, "copy_img: images don't have the same size");
    }

    for (size_t i = 0; i < img_src->width * img_src->height; i++)
    {
        img_dst->data[i].r = img_src->data[i].r;
        img_dst->data[i].g = img_src->data[i].g;
        img_dst->data[i].b = img_src->data[i].b;
    }
}
