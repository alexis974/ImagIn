#ifndef HSL_H
#define HSL_H

struct PixelHSL
{
    int h;
    float s;
    float l;
};

struct PixelHSL RGBtoHSL(struct Pixel pxl, float bd);

struct Pixel HSLtoRGB(struct PixelHSL pxl, float bd);

#endif /* ! HSL_H */
