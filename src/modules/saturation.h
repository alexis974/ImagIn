#ifndef SATURATION_H
#define SATURATION_H

struct PixelHSV
{
    float h;
    float s;
    size_t v;
};

void saturation(struct Image *img, float change);

#endif
