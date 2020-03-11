#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "../../imagin.h"

#include "scale.h"

// TODO : Coding style : 4.5 Fct max 4 arg
void interpolation(struct Image *src, struct Image *dst, size_t i, size_t j,
        float valx, float valy)
{
    size_t minx = (size_t)valx;
    size_t miny = (size_t)valy;
    size_t maxx = minx + 1;
    size_t maxy = miny + 1;

    if (maxx >= src->width)
    {
        maxx --;
    }

    if (maxy >= src->height)
    {
        maxy --;
    }

    double fx = valx - minx;
    double fy = valy - miny;

    // Red
    dst->data[j*dst->width+i].r =
        src->data[miny*src->width+minx].r * (1-fx) * (1-fy) +
        src->data[maxy*src->width+minx].r * fx * (1-fy) +
        src->data[miny*src->width+maxx].r * (1-fx) * fy +
        src->data[maxy*src->width+maxx].r * fx * fy;

    // Green
    dst->data[j*dst->width+i].g =
        src->data[miny*src->width+minx].g * (1-fx) * (1-fy) +
        src->data[maxy*src->width+minx].g * fx * (1-fy) +
        src->data[miny*src->width+maxx].g * (1-fx) * fy +
        src->data[maxy*src->width+maxx].g * fx * fy;

    // Blue
    dst->data[j*dst->width+i].b =
        src->data[miny*src->width+minx].b * (1-fx) * (1-fy) +
        src->data[maxy*src->width+minx].b * fx * (1-fy) +
        src->data[miny*src->width+maxx].b * (1-fx) * fy +
        src->data[maxy*src->width+maxx].b * fx * fy;
}

struct Image *scale_img(struct Image *full_img, size_t width, size_t height)
{
    if (!full_img)
    {
        errx(1, "scale_img: No image found");
    }

    struct Image *small_img = malloc(width * height * sizeof(struct Image));
    if (!small_img)
    {
        errx(1,"scale_img : Could not allocate memory.");
    }

    // Initialize scaled image
    small_img->width = width;
    small_img->height = height;
    small_img->bit_depth = full_img->bit_depth;
    small_img->data = malloc(width * height * sizeof(struct Pixel));

    if (!small_img->data)
    {
        errx(1,"scale_img : Could not allocate memory.");
    }

    float old_width = full_img->width;
    float new_width = width;
    float ratio_w = new_width / old_width;

    float old_height = full_img->height;
    float new_height = height;
    float ratio_h = new_height / old_height;

    float valw;
    float valh;

    for (size_t i = 0; i < width; i++)
    {
        for (size_t j = 0; j < height; j++)
        {
            valw = i / ratio_w;
            valh = j / ratio_h;
            interpolation(full_img, small_img, i, j, valw, valh);
        }
    }

    return small_img;
}
