#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../imagin.h"
#include "../debug/error_handler.h"

#include "ppm.h"

struct Image *readPPM(const char *filename)
{
    char buff[16];
    struct Image *img;
    FILE *fp;
    int c;

    //open PPM file for reading
    fp = fopen(filename, "rb");
    if (!fp)
    {
        throw_error("readPPM", "Unable to open file");
        return NULL;
    }

    //read image format
    if (!fgets(buff, sizeof(buff), fp))
    {
        throw_error("readPPM", "Wrong image format");
        return NULL;
    }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '6')
    {
        throw_error("readPPM","Invalid image format (must be 'P6')");
        return NULL;
    }

    //alloc memory form image
    img = malloc(sizeof(struct Image));
    if (!img)
    {
        throw_error("readPPM","Unable to allocate memory");
        return NULL;
    }

    //check for comments
    c = getc(fp);
    while (c == '#')
    {
        while (getc(fp) != '\n')
        {
            c = getc(fp);
        }
    }

    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%ld %ld", &img->width, &img->height) != 2)
    {
        throw_error("readPPM", "Invalid image size");
        return NULL;
    }

    //read rgb component
    if (fscanf(fp, "%ld", &img->bit_depth) != 1)
    {
        throw_error("readPPM","Invalid rgb component");
        return NULL;
    }

    //check rgb component depth
    if (img->bit_depth != 255)
    {
        throw_error("readPPM","file does not have 8-bits components");
        return NULL;
    }

    //memory allocation for pixel data
    while (fgetc(fp) != '\n') ;
    img->data = (struct Pixel*)
        malloc(img->width * img->height * sizeof(struct Pixel));

    if (!img->data)
    {
        throw_error("readPPM","Unable to allocate memory");
        return NULL;
    }

    //read pixel data from file
    if (fread(img->data, 3 * img->width, img->height, fp) != img->height)
    {
        throw_error("readPPM", "Error loading image");
        return NULL;
    }

    fclose(fp);
    printf("PPM Image read!\n");
    return img;
}

void writePPM(const char *filename, struct Image *img)
{
    FILE *fp;
    //open file for output
    fp = fopen(filename, "wb");
    if (!fp)
    {
        throw_error("writePPM", "Unable to open file");
    }

    //write the header file
    //image format
    fprintf(fp, "P6\n");

    //comments
    fprintf(fp, "# Image created by SilverGrain\n");

    //image size
    fprintf(fp, "%ld %ld\n", img->width, img->height);

    // rgb component depth
    fprintf(fp, "%ld\n", img->bit_depth);

    // pixel data
    fwrite(img->data, 3 * img->width, img->height, fp);
    fclose(fp);
    printf("PPM Image saved!\n");
}
