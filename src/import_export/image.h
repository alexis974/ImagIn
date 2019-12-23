#ifndef IMAGE_H
#define IMAGE_H

void init_image();

typedef struct image {
    //Pixel of the image
    size_t  *pixel;
    //The size of the imported image, height first
    size_t *size;
    //The list of 4 coordinates we currently working on
    size_t coordinates;
};

#endif
