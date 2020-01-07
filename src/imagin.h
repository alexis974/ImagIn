#ifndef IMAGIN_H
#define IMAGIN_H

struct Pixel
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct Image
{
    size_t width;
    size_t height;
    size_t bit_depth;
    struct Pixel *data;
};

#endif
