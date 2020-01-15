#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "../imagin.h"

void interpolation(struct Image *src, struct Image *dst, size_t i, size_t j,
        float valx, float valy)
{
    size_t minx = (size_t)valx;
    size_t miny = (size_t)valy;
    size_t maxx = minx + 1;
    if (maxx >= src->width)
    {
        maxx --;
    }
    size_t maxy = miny + 1;
    if (maxy >= src->height)
    {
        maxy --;
    }
    double fx = valx - minx;
    double fy = valy - miny;

    //red
    dst->data[j*dst->width+i].red =
        src->data[miny*src->width+minx].red * (1-fx) * (1-fy) +
        src->data[maxy*src->width+minx].red * fx * (1-fy) +
        src->data[miny*src->width+maxx].red * (1-fx) * fy +
        src->data[maxy*src->width+maxx].red * fx * fy;

    //green
    dst->data[j*dst->width+i].green =
        src->data[miny*src->width+minx].green * (1-fx) * (1-fy) +
        src->data[maxy*src->width+minx].green * fx * (1-fy) +
        src->data[miny*src->width+maxx].green * (1-fx) * fy +
        src->data[maxy*src->width+maxx].green * fx * fy;

    //blue
    dst->data[j*dst->width+i].blue =
        src->data[miny*src->width+minx].blue * (1-fx) * (1-fy) +
        src->data[maxy*src->width+minx].blue * fx * (1-fy) +
        src->data[miny*src->width+maxx].blue * (1-fx) * fy +
        src->data[maxy*src->width+maxx].blue * fx * fy;
}

struct Image *scale_img(struct Image *img, size_t width, size_t height)
{
    if (!img)
    {
        errx(1, "scale_img: No image found");
    }

    //Initialize scaled image
    struct Image *img_scale = malloc(sizeof(struct Image));
    img_scale->width = width;
    img_scale->height = height;
    img_scale->bit_depth = img->bit_depth;
    img_scale->data = malloc(width * height * sizeof(struct Pixel));

    float old_width = img->width;
    float new_width = width;
    float ratio_w = new_width / old_width;

    float old_height = img->height;
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
            interpolation(img, img_scale, i, j, valw, valh);
        }
    }

    return img_scale;
}
