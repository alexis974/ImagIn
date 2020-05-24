#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "../../imagin.h"

#include "../../tools/minmax.h"

#include "../imagin/hsl_conv.h"

#include "hsl.h"

void apply_change(struct PixelHSL *pxlhsl, float change, char component)
{
    if (component == 'h')
    {
        change *= 360;
        pxlhsl->h += change;
        if (pxlhsl->h < 0)
            pxlhsl->h = 0;
        if (pxlhsl->h > 360)
            pxlhsl->h = 360;
        return;
    }

    else if (component == 's')
    {
        change += 1;
        pxlhsl->s *= change;
        if (pxlhsl->s < 0)
            pxlhsl->s = 0;
        if (pxlhsl->s > 1)
            pxlhsl->s = 1;
        return;
    }

    else if (component == 'l')
    {
        change += 1;
        pxlhsl->l *= change;
        if (pxlhsl->l < 0)
            pxlhsl->l = 0;
        if (pxlhsl->l > 1)
            pxlhsl->l = 1;
        return;
    }

    errx(1, "apply_change: No function applied");
}

void hsl(struct Image *img, float change, char id, char component)
{
    if (!img)
    {
        errx(1, "hsl_h: No image found");
    }

    if (change == 0)
        return;

    struct PixelHSL *pxlhsl = malloc(sizeof(struct PixelHSL));
    for (size_t i = 0; i < img->width * img->height; i++)
    {
        *pxlhsl = RGBtoHSL(img->data[i], (float)img->bit_depth);

        //which color
        if (id == 'r') //red
        {
            if (getmax3(img->data[i].red, img->data[i].green,
                    img->data[i].blue) == img->data[i].red)
                apply_change(pxlhsl, change, component);
        }
        else if (id == 'g') //green
        {
            if (getmax3(img->data[i].red, img->data[i].green,
                    img->data[i].blue) == img->data[i].green)
                apply_change(pxlhsl, change, component);
        }
        else if(id == 'b') //blue
        {
            if (getmax3(img->data[i].red, img->data[i].green,
                    img->data[i].blue) == img->data[i].blue)
                apply_change(pxlhsl, change, component);
        }

        img->data[i] = HSLtoRGB(*pxlhsl, (float)img->bit_depth);
    }
    free(pxlhsl);
}
