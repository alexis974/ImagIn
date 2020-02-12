#include <stdio.h>
#include <err.h>
#include <math.h>

#include "../../imagin.h"

#include "saturation.h"

float getmin(float x, float y)
{
    return x < y ? x : y;
}

float getmax(float x, float y)
{
    return x > y ? x : y;
}

// TODO : Coding style : 2.8 Fct name no upper case
// TODO : Coding style : 4.10 Fct max 25 lines
struct PixelHSL RGBtoHSL(struct Pixel pxl, float bd)
{
    struct PixelHSL newpxl;
    float h;
    float s;
    float l;

    float r = pxl.red / bd;
    float g = pxl.green / bd;
    float b = pxl.blue / bd;
    float min = getmin(r, getmin(g, b)); //min(r, g, b)
    float max = getmax(r, getmax(g, b)); //max(r, g, b)

    if (r == g && g == b)
    {
        h = 0.0;
        s = 0.0;
        l = r;
    }
    else
    {
        float delta = max - min;
        float sum = max + min;

        l = sum / 2;
        if (l < 0.5)
            s = delta / sum;
        else
            s = delta / (2.0 - delta);

        if (r == max)
            h = (g - b) / delta;
        else if (g == max)
            h = 2.0 + (b - r) / delta;
        else
            h = 4.0 + (r - g) / delta;

        h /= 6;
        if (h < 0)
            h ++;
    }

    newpxl.h = (int)(h * 360.0);
    newpxl.s = (int)(s * bd);
    newpxl.l = (int)(l * bd);
    return newpxl;
}

// TODO : Coding style : 2.8 Fct name no upper case
// TODO : Coding style : 4.10 Fct max 25 lines
struct Pixel HSLtoRGB(struct PixelHSL pxl, float bd)
{
    struct Pixel newpxl;
    float r;
    float g;
    float b;

    float h = (pxl.h % 260) / 360.0;
    float s = pxl.s / (bd + 1.0);
    float l = pxl.l / (bd + 1.0);

    if (s == 0)
    {
        r = l;
        g = l;
        b = l;
    }
    else
    {
        float tmp1;
        float tmp2;
        float tmpr;
        float tmpg;
        float tmpb;

        //tmp1, tmp2
        if (l < 0.5)
            tmp2 = l * (1 + s);
        else
            tmp2 = (l + s) - (l * s);
        tmp1 = 2 * l - tmp2;

        //tmpr, tmpg, tmpb
        tmpr = h + 1.0 / 3.0;
        if (tmpr > 1)
            tmpr --;
        tmpg = h;
        tmpb = h - 1.0 / 3.0;
        if (tmpb < 0)
            tmpb ++;

        //red
        if (tmpr < 1.0 / 6.0)
            r = tmp1 + (tmp2 - tmp1) * 6.0 * tmpr;
        else if (tmpr < 0.5)
            r = tmp2;
        else if (tmpr < 2.0 / 3.0)
            r = tmp1 + (tmp2 - tmp1) * ((2.0 / 3.0) - tmpr) * 6.0;
        else
            r = tmp1;

        //green
        if (tmpg < 1.0 / 6.0)
            g = tmp1 + (tmp2 - tmp1) * 6.0 * tmpg;
        else if (tmpg < 0.5)
            g = tmp2;
        else if (tmpg < 2.0 / 3.0)
            g = tmp1 + (tmp2 - tmp1) * ((2.0 / 3.0) - tmpg) * 6.0;
        else
            g = tmp1;

        //blue
        if (tmpb < 1.0 / 6.0)
            b = tmp1 + (tmp2 - tmp1) * 6.0 * tmpb;
        else if (tmpb < 0.5)
            b = tmp2;
        else if (tmpb < 2.0 / 3.0)
            b = tmp1 + (tmp2 - tmp1) * ((2.0 / 3.0) - tmpb) * 6.0;
        else
            b = tmp1;
    }

    newpxl.red = (int)(r * bd);
    newpxl.green = (int)(g * bd);
    newpxl.blue = (int)(b * bd);

    return newpxl;
}

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
