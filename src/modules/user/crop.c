#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "../../imagin.h"

#include "crop.h"

struct Image *crop(struct Image *img, size_t x_down_left, size_t y_down_left,
        size_t x_up_right, size_t y_up_right)
{
    if (!img)
    {
        errx(1, "crop: No image found");
    }

    if (x_up_right > img->width || y_up_right > img->height)
    {
        errx(1, "crop : crop image bigger than input image");
    }

    struct Image *crop_img = malloc(sizeof(struct Image));
    if (!crop_img)
    {
        errx(1, "crop : Could not allocate memory for crop_img.");
    }

    crop_img->width = x_up_right - x_down_left + 1;
    crop_img->height = y_up_right - y_down_left + 1;
    crop_img->bit_depth = img->bit_depth;
    crop_img->data = malloc(crop_img->width * crop_img->height *
            sizeof(struct Pixel));

    size_t index = 0;
    size_t offset = img->width;

    for (size_t y = y_down_left; y <= y_up_right ; y++)
    {
        for (size_t x = x_down_left; x <= x_up_right ; x++)
        {
            if (index > crop_img->width * crop_img->height)
            {
                errx(1, "crop: crop_img smaller than crop surface");
            }

            crop_img->data[index].r = img->data[x + offset * y].r;
            crop_img->data[index].g = img->data[x + offset * y].g;
            crop_img->data[index].b = img->data[x + offset * y].b;
            index++;
        }
    }

    if (index < crop_img->width * crop_img->height)
    {
        printf("\ncrop width:%ld | crop height: %ld\n", crop_img->width, crop_img->height);
        printf("%ld | %ld", y_up_right, y_down_left);
        errx(1, "crop: crop_img bigger than crop surface");
    }

    return crop_img;
}
