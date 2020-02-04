#ifndef SATURATION_H
#define SATURATION_H

struct PixelHSL
{
    int h;
    int s;
    int l;
};

void saturation(struct Image *img, float change);

#endif
