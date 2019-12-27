#ifndef IMAGIN_H
#define IMAGIN_H

typedef struct
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Pixel;

typedef struct
{
    size_t width;
    size_t height;
	size_t bit_depth;
    Pixel *data;
} Image;

#endif
