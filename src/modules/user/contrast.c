#include <stdio.h>
#include <err.h>
#include <math.h>

#include "../../imagin.h"

#include "contrast.h"

// TODO : Fix explicit cast
size_t apply_contrast(size_t c, float n, size_t depth)
{
    if (c <= depth / 2)
    {
        return (size_t)(depth / 2) * pow((double)2 * c / depth, n);
    }

    else
    {
        return depth - apply_contrast(depth - c, n, depth);
    }
}


// Modify contrast of given image
void contrast(struct Image *img, float range)
{
    if (!img)
    {
        errx(1, "contrast: no image found");
    }

    if (range == 1)
        return;

    for (size_t i = 0; i < img->width * img->height; i++)
    {
        img->data[i].r = apply_contrast(img->data[i].r, range,
                img->bit_depth);
        img->data[i].g = apply_contrast(img->data[i].g, range,
                img->bit_depth);
        img->data[i].b = apply_contrast(img->data[i].b, range,
                img->bit_depth);
    }
}
