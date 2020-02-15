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

    if (change == 1)
        return;

    struct PixelHSL pxlhsl;
    for (size_t i = 0; i < img->width * img->height; i++)
    {
        pxlhsl = RGBtoHSL(img->data[i], (float)img->bit_depth);

        pxlhsl.s *= change;
        if (pxlhsl.s < 0)
            pxlhsl.s = 0;
        if (pxlhsl.s > 1)
            pxlhsl.s = 1;

        img->data[i] = HSLtoRGB(pxlhsl, (float)img->bit_depth);
    }
}
