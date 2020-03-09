#ifndef HSL_CONV_H
#define HSL_CONV_H

struct PixelHSL
{
    int h;
    float s;
    float l;
};

struct Sliders
{
    float r;
    float g;
    float b;

    char r_id;
    char g_id;
    char b_id;
};

struct PixelHSL RGBtoHSL(struct Pixel pxl, float bd);

struct Pixel HSLtoRGB(struct PixelHSL pxl, float bd);

#endif /* ! HSL_H */
