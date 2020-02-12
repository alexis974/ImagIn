#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <tiffio.h>
#include <gtk/gtk.h>

#include "../../imagin.h"
#include "../../debug/error_handler.h"

#include "tiff.h"

// TODO : Coding style : 2.8 Fct name no upper case
// TODO : Coding style : Fct 25 lines max
struct Image *readTIFF(const char *filename)
{
    TIFF *tif=TIFFOpen(filename, "r");
    if (!tif)
    {
        throw_error("readTIFF", "Unable to open file");

        return NULL;
    }

    uint32 width;
    uint32 height;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    uint32 *raster= (uint32 *) _TIFFmalloc(width*height * sizeof(uint32));

    if (!raster)
    {
        throw_error("readTIFF", "Unable to allocate memory");

        return NULL;
    }

    if (!TIFFReadRGBAImage(tif, width, height, raster, 0))
    {
        throw_error("readTIFF", "Unable to read image");

        return NULL;
    }

    struct Image *img;

    // Alloc memory for image
    img = malloc(sizeof(struct Image));
    if (!img)
    {
        throw_error("readTIFF", "Unable to allocate memory");

        return NULL;
    }

    img->height = height;
    img->width = width;
    img->bit_depth = 255;

    img->data =
        (struct Pixel*)malloc(img->width * img->height * sizeof(struct Pixel));
    if (!img->data)
    {
        throw_error("readTIFF", "Unable to allocate memory");

        return NULL;
    }

    for (size_t j = 0; j < height; j++)
    {
        for (size_t i = 0; i < width; i++)
        {
            // TIFF is save from bottom to top
            img->data[(height-j-1)*width+i].red =
                (unsigned char)TIFFGetR(raster[j*width+i]);
            img->data[(height-j-1)*width+i].blue =
                (unsigned char)TIFFGetB(raster[j*width+i]);
            img->data[(height-j-1)*width+i].green =
                (unsigned char)TIFFGetG(raster[j*width+i]);
        }
    }

    _TIFFfree(raster);
    TIFFClose(tif);
    printf("TIFF Image read!\n");

    return img;
}

// TODO : Coding style : 2.8 Fct name no upper case
// TODO : Coding style : Fct 25 lines max
void writeTIFF(const char *filename, struct Image *img)
{
    // Creating the file
    TIFF *out= TIFFOpen(filename, "w");
    if (!out)
    {
        throw_error("writeTIFF", "Unable to create file");

        return;
    }

    int sampleperpixel = 3; // Put 4 if alpha channel

    // Set the width of the image
    TIFFSetField (out, TIFFTAG_IMAGEWIDTH, img->width);
    // Set the height of the image
    TIFFSetField(out, TIFFTAG_IMAGELENGTH, img->height);
    // Set number of channels per pixel
    TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, sampleperpixel);
    // Set the size of the channels
    TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
    // Set the origin of the image
    TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    // Some other essential fields
    TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

    size_t rowsize = sampleperpixel * img->width;
    unsigned char *buffer = NULL;

    // Allocating memory to store the pixels of current row
    if (TIFFScanlineSize(out))
    {
        buffer =(unsigned char *)_TIFFmalloc(rowsize);
    }
    else
    {
        buffer = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(out));
    }

    // Setting the strip size to be size of one row of pixels
    TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(out,rowsize));

    for (size_t j = 0; j < img->height; j++)
    {
        // Filling Buffer
        for (size_t i = 0; i < img->width; i++)
        {
            buffer[i*sampleperpixel] = img->data[j*img->width+i].red;
            buffer[i*sampleperpixel+1] = img->data[j*img->width+i].green;
            buffer[i*sampleperpixel+2] = img->data[j*img->width+i].blue;
        }

        // Writing in image
        if (TIFFWriteScanline(out, buffer, j, 0) < 0)
        {
            throw_error("writeTIFF", "Unable to write image.");

            return;
        }
    }

    TIFFClose(out);
    if (buffer)
    {
        _TIFFfree(buffer);
    }

    printf("TIFF Image saved!\n");
}
