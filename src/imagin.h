#ifndef IMAGIN_H
#define IMAGIN_H

struct Pixel
{
    size_t red;
    size_t green;
    size_t blue;
};

struct Image
{
    size_t width;
    size_t height;
    size_t bit_depth;
    struct Pixel *data;
};

struct Images
{
    struct Image *full;
    struct Image *scale;
    struct Image *edit;
    struct Image *small;
};

#endif /* ! IMAGIN_H */
