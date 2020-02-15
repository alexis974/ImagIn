#include <stdio.h>
#include <err.h>
#include <math.h>

#include "../../imagin.h"

#include "../imagin/hsl.h"

#include "saturation.h"

// TODO : Coding style : Explicit cast
void saturation(struct Image *img, float change)
{
    if (!img)
    {
        errx(1, "saturation: No image found");
    }

    change -= 1;

    if (change < -1)
        change = -1;
    else if (change > 1)
        change = 1;

    struct PixelHSL pxlhsl;
    float gray_fact;
    float var_delta;
    for (size_t i = 0; i < img->width * img->height; i++)
    {
        pxlhsl = RGBtoHSL(img->data[i], (float)img->bit_depth);

        if (change >= 0)
        {
            gray_fact = (float)pxlhsl.s / (float)img->bit_depth;
            var_delta = img->bit_depth - pxlhsl.s;
            pxlhsl.s += change * var_delta * gray_fact;
        }
        else
        {
            var_delta = pxlhsl.s;
            pxlhsl.s += change * var_delta;
        }

        img->data[i] = HSLtoRGB(pxlhsl, (float)img->bit_depth);
    }
}
