#include <stdio.h>
#include <err.h>
#include <math.h>

#include "../../imagin.h"

#include "../../tools/minmax.h"

#include "hsl_conv.h"

void init_sliders(struct Sliders sliders)
{
    sliders.r_id = 'r';
    sliders.g_id = 'g';
    sliders.b_id = 'b';
    if (sliders.r == 0){}; //to avoid unused error
}

struct PixelHSL RGBtoHSL(struct Pixel rgb, float bd)
{
    struct PixelHSL hsl;

    float r = rgb.red / bd;
    float g = rgb.green / bd;
    float b = rgb.blue / bd;

    float min = getmin3(r, g, b);
    float max = getmax3(r, g, b);
    float delta = max - min;

    hsl.l = (max + min) / 2;

    if (delta == 0)
    {
        hsl.h = 0;
        hsl.s = 0.0f;
    }
    else
    {
        hsl.s = (hsl.l <= 0.5) ? (delta / (max + min)) :
            (delta / (2 - max - min));

        float h;

        if (r == max)
            h = ((g - b) / 6) / delta;
        else if (g == max)
            h = (1.0f / 3) + ((b - r) / 6) / delta;
        else
            h = (2.0f / 3) + ((r - g) / 6) / delta;

        if (h < 0)
            h ++;
        if (h > 1)
            h --;

        hsl.h = (int)(h * 360);
    }

    return hsl;
}

float huetoRGB(float v1, float v2, float vH)
{
    if (vH < 0)
        vH += 1;

    if (vH > 1)
        vH -= 1;

    if ((6 * vH) < 1)
        return (v1 + (v2 - v1) * 6 * vH);

    if ((2 * vH) < 1)
        return v2;

    if ((3 * vH) < 2)
        return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);

    return v1;
}

struct Pixel HSLtoRGB(struct PixelHSL hsl, float bd)
{
    struct Pixel rgb;

    if (hsl.s == 0)
    {
        rgb.red = (unsigned char)(hsl.l * bd);
        rgb.green = (unsigned char)(hsl.l * bd);
        rgb.blue = (unsigned char)(hsl.l * bd);
    }
    else
    {
        float tmp1;
        float tmp2;
        float h = (float)hsl.h / 360;

        //tmp1, tmp2
        tmp2 = (hsl.l < 0.5) ? (hsl.l * (1 + hsl.s)) : ((hsl.l + hsl.s) -
                (hsl.l * hsl.s));
        tmp1 = 2 * hsl.l - tmp2;

        rgb.red = (unsigned char)(bd * huetoRGB(tmp1, tmp2, h + (1.0f / 3)));
        rgb.green = (unsigned char)(bd * huetoRGB(tmp1, tmp2, h));
        rgb.blue = (unsigned char)(bd * huetoRGB(tmp1, tmp2, h - (1.0f / 3)));
    }

    return rgb;
}
