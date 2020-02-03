#include <stdio.h>
#include <err.h>
#include <math.h>

#include "../imagin.h"

#include "contrast.h"

size_t apply_contrast(size_t c, float n, size_t depth)
{
    if (c <= depth / 2)
    {
        return (size_t)(depth/2) * pow((double)2*c/255, n);
    }
    else
    {
        return 255 - apply_contrast(255 - c, n, depth);
    }
}

/*
** Modify contrast of given image
*/
void contrast(struct Image *img, float range)
{
    if (!img)
    {
        errx(1, "contrast: no image found");
    }

    range +=1;

    for (size_t i = 0; i < img->width * img->height; i++)
    {
        img->data[i].red = apply_contrast(img->data[i].red, range,
                img->bit_depth);
        img->data[i].green = apply_contrast(img->data[i].green, range,
                img->bit_depth);
        img->data[i].blue = apply_contrast(img->data[i].blue, range,
                img->bit_depth);
    }
}
