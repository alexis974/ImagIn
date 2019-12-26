#ifndef IMAGIN_H
#define IMAGIN_H

#define RGB_COMPONENT_COLOR 255

typedef struct
{
    unsigned char red,green,blue;
} Pixel;

typedef struct
{
    size_t x, y;
    Pixel *data;
} Image;

#endif
