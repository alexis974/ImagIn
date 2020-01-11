#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "../imagin.h"
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
        errx(1 , "readPPM: Unable to open file '%s'\n", filename);
    }

    //read image format
    if (!fgets(buff, sizeof(buff), fp))
    {
        errx(1, "readPPM: Wrong image format for '%s'\n", filename);
    }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '6')
    {
        errx(1, "readPPM: Invalid image format (must be 'P6')\n");
    }

    //alloc memory form image
    img = malloc(sizeof(struct Image));
    if (!img)
    {
        errx(1, "readPPM: Unable to allocate memory\n");
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
        errx(1, "readPPM: Invalid image size (error loading '%s')\n", filename);
    }

    //read rgb component
    if (fscanf(fp, "%ld", &img->bit_depth) != 1)
    {
        errx(1, "readPPM: Invalid rgb component (error loading '%s')\n", filename);
    }

    //check rgb component depth
    if (img->bit_depth != 255)
    {
        errx(1, "readPPM: '%s' does not have 8-bits components\n", filename);
    }

    //memory allocation for pixel data
    while (fgetc(fp) != '\n') ;
    img->data = (struct Pixel*)malloc(img->width * img->height * sizeof(struct Pixel));

    if (!img->data)
    {
        errx(1, "readPPM: Unable to allocate memory\n");
    }

    //read pixel data from file
    if (fread(img->data, 3 * img->width, img->height, fp) != img->height)
    {
        errx(1, "readPPM: Error loading image '%s'\n", filename);
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
        errx(1, "writePPM: Unable to open file '%s'\n", filename );
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
