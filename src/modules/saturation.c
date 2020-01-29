#include <stdio.h>
#include <err.h>
#include <math.h>

#include "../imagin.h"

#include "saturation.h"

size_t getmin(size_t x, size_t y)
{
    return x < y ? x : y;
}

size_t getmax(size_t x, size_t y)
{
    return x > y ? x : y;
}

PixelHSV RGBtoHSV(Pixel pxl)
{
    PixelHSV newpxl;
    float h;
    float s;
    size_t v;

    size_t r = pxl->red;
    size_t g = pxl->green;
    size_t b = pxl->blue;
    size_t min = getmin(r, getmin(g, b)); //min(r, g, b)
    size_t max = getmax(r, getmax(g, b)); //max(r, g, b)

    //v
    v = max;

    //s
    size_t delta = max - min;
    if (max != 0)
    {
        s = (float) delta / max;
    }
    else //black pxl
    {
        s = 0;
        h = -1;
        newpxl->h = h;
        newpxl->s = s;
        newpxl->v = v;
        return newpxl;
    }

    //h
    if (r == max) //red
    {
        h = (float) (g-b) / delta;
    }
    else if (g == max) //green
    {
        h = (float) (2 + (b-r) / delta);
    }
    else //blue
    {
        h = (float) (4 + (r-g) / delta);
    }
    h *= 60;
    if (h < 0)
    {
        h += 360;
    }

    newpxl->h = h;
    newpxl->s = s;
    newpxl->v = v;
    return newpxl;
}

Pixel HSVtoRGB(PixelHSV pxl)
{
    Pixel newpxl;
    size_t r;
    size_t g;
    size_t b;

    float h = pxl->h;
    float s = pxl->s;
    size_t v = pxl->v;
    int i;
    float f;
    float p;
    float q;
    float t;

    if (s == 0)
    {
        newpxl->red = 0;
        newpxl->green = 0;
        newpxl->blue = 0;
        return newpxl;
    }

    h /= 60;
    i = floor(h);
    f = h - i;
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));

    switch(i)
    {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        default:
            r = v;
            g = p;
            b = q;
            break:
    }

    newpxl->red = r;
    newpxl->green = g;
    newpxl->blue = b;
    return newpxl;
}

void saturate(Pixel pxl, float change)
{
    PixelHSV pxlhsv = RGBtoHSV(pxl);
    pxlhsv->s *= change;
    pxl = HSVtoRGB(pxlhsv);
}

void saturation(struct Image *img, float change)
{
    if (!img)
    {
        errx(1, "saturation: No image found");
    }

    for (size_t i = 0; i < img->width * img->heigth; i++)
    {
        //only non-black pixels
        if (getmax(img->data[i].red,
                    getmax(img->data[i].green, img->data[i].blue)) != 0)
        {
            saturate(img->data[i], change);
        }
    }
}
