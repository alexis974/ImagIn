#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <string.h>
#include <tiffio.h>
#include"../imagin.h"
#include "tiff.h"

struct Image *readTIFF(const char *filename)
{
    TIFF *tif=TIFFOpen(filename, "r");
    if(!tif)
    {
        errx(1 , "readTIFF: Unable to open file '%s'\n", filename);
    }
    uint32 width;
    uint32 height;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    uint32 *raster=(uint32 *) _TIFFmalloc(width*height *sizeof(uint32));
    if (!raster)
    {
        errx(1, "readTIFF: Unable to allocate memory\n");
    }
    if(!TIFFReadRGBAImage(tif, width, height, raster, 0))
    {
        errx(1, "readTIFF: Unable to read image\n");
    }
    struct Image *img;

    //alloc memory for image
    img = malloc(sizeof(struct Image));
    if (!img)
    {
        errx(1, "readTIFF: Unable to allocate memory\n");
    }
    img->height = height;
    img->width = width;
    img->bit_depth = 255;

    img->data = (struct Pixel*)malloc(img->width * img->height * sizeof(struct Pixel));
    if (!img->data)
    {
        errx(1, "readTIFF: Unable to allocate memory\n");
    }

    for (size_t j = 0; j < height; j++)
    {
        for (size_t i = 0; i < width; i++)
        {
            img->data[j*width+i].red = (unsigned char)TIFFGetR(raster[j*width+i]);
            img->data[j*width+i].blue = (unsigned char)TIFFGetB(raster[j*width+i]);
            img->data[j*width+i].green = (unsigned char)TIFFGetG(raster[j*width+i]);
        }
    }
    _TIFFfree(raster);
    TIFFClose(tif);
    return img;
}

void writeTIFF(const char *filename, struct Image *img)
{
    //Creating the file
    TIFF *out= TIFFOpen(filename, "w");
    if (!out)
    {
        errx(1, "writeTIFF: Unable to create file\n");
    }
    int sampleperpixel = 3;//Put 4 if alpha channel

    TIFFSetField (out, TIFFTAG_IMAGEWIDTH, img->width);  // set the width of the image
    TIFFSetField(out, TIFFTAG_IMAGELENGTH, img->height);    // set the height of the image
    TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, sampleperpixel);   // set number of channels per pixel
    TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);    // set the size of the channels
    TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);    // set the origin of the image.
    //   Some other essential fields to set that you do not have to understand for now.
    TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

    size_t rowsize = sampleperpixel * img->width;
    unsigned char *buffer = NULL;

    //Allocating memory to store the pixels of current row
    if (TIFFScanlineSize(out))
        buffer =(unsigned char *)_TIFFmalloc(rowsize);
    else
        buffer = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(out));

    //Setting the strip size to be size of one row of pixels
    TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(out,rowsize));

    for (size_t j = 0; j < img->height; j++)
    {
        //Filling Buffer
        for (size_t i = 0; i < img->width; i++)
        {
            buffer[i*sampleperpixel] = img->data[j*img->width+i].red;
            buffer[i*sampleperpixel+1] = img->data[j*img->width+i].green;
            buffer[i*sampleperpixel+2] = img->data[j*img->width+i].blue;
        }
        //Writing in image
        if(TIFFWriteScanline(out, buffer, j, 0) < 0)
        {
            errx(1,"writeTIFF: Unable to write image.\n");
            break;
        }
    }

    TIFFClose(out);
    if (buffer)
        _TIFFfree(buffer);
    printf("TIFF Image saved!\n");
}