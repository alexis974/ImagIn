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

    for (size_t y = (img->height - y_up_right);
            y <= (img->height - y_down_left) ; y++)
    {
        for (size_t x = x_down_left; x <= x_up_right ; x++)
        {
            if (index > crop_img->width * crop_img->height)
            {
                errx(1, "crop: crop_img smaller than crop surface");
            }

            crop_img->data[index].red = img->data[x + offset * y].red;
            crop_img->data[index].green = img->data[x + offset * y].green;
            crop_img->data[index].blue = img->data[x + offset * y].blue;
            index++;
        }
    }

    if (index < crop_img->width * crop_img->height)
    {
        errx(1, "crop: crop_img bigger than crop surface");
    }

    return crop_img;
}
