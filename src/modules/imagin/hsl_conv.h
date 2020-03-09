#ifndef HSL_CONV_H
#define HSL_CONV_H

struct PixelHSL
{
    int h;
    float s;
    float l;
};

struct SlidersH
{
    float r;
    float g;
    float b;

    size_t r_id = 0;
    size_t g_id = 1;
    size_t b_id = 2;
};

struct PixelHSL RGBtoHSL(struct Pixel pxl, float bd);

struct Pixel HSLtoRGB(struct PixelHSL pxl, float bd);

#endif /* ! HSL_H */
