#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>

#include "../imagin.h"

#include "../debug/error_handler.h"

#include "ppm.h"

// TODO : Coding style : 2.8 Fct name no upper case
// TODO : Coding style : Fct 25 lines max
struct Image *readPPM(const char *filename)
{
    // TODO : Coding style : 5.1 Initialized at declaration
    char buff[16];
    // TODO : Coding style : 5.1 Initialized at declaration
    struct Image *img;
    // TODO : Coding style : 5.1 Initialized at declaration
    FILE *fp;
    // TODO : Coding style : 5.1 Initialized at declaration
    int c;

    // Open PPM file for reading
    fp = fopen(filename, "rb");
    if (!fp)
    {
        throw_error("readPPM", "Unable to open file");

        return NULL;
    }

    // Read image format
    if (!fgets(buff, sizeof(buff), fp))
    {
        throw_error("readPPM", "Wrong image format");

        return NULL;
    }

    // Check the image format
    if (buff[0] != 'P' || buff[1] != '6')
    {
        throw_error("readPPM","Invalid image format (must be 'P6')");

        return NULL;
    }

    // Alloc memory form image
    img = malloc(sizeof(struct Image));
    if (!img)
    {
        throw_error("readPPM","Unable to allocate memory");

        return NULL;
    }

    // Check for comments
    c = getc(fp);
    while (c == '#')
    {
        while (getc(fp) != '\n')
        {
            c = getc(fp);
        }
    }

    ungetc(c, fp);
    // Read image size information
    if (fscanf(fp, "%ld %ld", &img->width, &img->height) != 2)
    {
        throw_error("readPPM", "Invalid image size");

        return NULL;
    }

    // Read rgb component
    if (fscanf(fp, "%ld", &img->bit_depth) != 1)
    {
        throw_error("readPPM","Invalid rgb component");

        return NULL;
    }

    // Check rgb component depth
    if (img->bit_depth != 255)
    {
        throw_error("readPPM","file does not have 8-bits components");

        return NULL;
    }

    // Memory allocation for pixel data
    while (fgetc(fp) != '\n') ;
    img->data =
        // TODO : Coding style : Explicite cast ?
        (struct Pixel*)malloc(img->width * img->height * sizeof(struct Pixel));

    if (!img->data)
    {
        throw_error("readPPM","Unable to allocate memory");

        return NULL;
    }

    // Read pixel data from file
    if (fread(img->data, 3 * img->width, img->height, fp) != img->height)
    {
        throw_error("readPPM", "Error loading image");

        return NULL;
    }

    fclose(fp);
    printf("PPM Image read!\n");

    return img;
}

// TODO : Coding style : 2.8 Fct name no upper case
void writePPM(const char *filename, struct Image *img)
{
    // TODO : Coding style : 5.1 Initialized at declaration
    FILE *fp;

    // Open file for output
    fp = fopen(filename, "wb");
    if (!fp)
    {
        throw_error("writePPM", "Unable to open file");
    }

    // Write the header file
    // Image format
    fprintf(fp, "P6\n");

    // Comments
    fprintf(fp, "# Image created by SilverGrain\n");

    // Image size
    fprintf(fp, "%ld %ld\n", img->width, img->height);

    // Rgb component depth
    fprintf(fp, "%ld\n", img->bit_depth);

    // Pixel data
    fwrite(img->data, 3 * img->width, img->height, fp);
    fclose(fp);
    printf("PPM Image saved!\n");
}
