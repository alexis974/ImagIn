#include <stdio.h>
#include <err.h>

#include "../../imagin.h"

#include "../imagin/hsl_conv.h"

void hsl(struct Image *img, float change, size_t id, char component)
{
    if (!img)
    {
        errx(1, "hsl_h: No image found");
    }

    if (change == 1)
        return;

    struct PixelHSL pxlhsl;
    for (size_t i = 0; i < img->width * img->height; i++)
    {
        //which color
        if (id == 0) //red
        {
            


