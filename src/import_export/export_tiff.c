#include <stdio.h>
#include <err.h>
#include <tiffio.h>
#include <string.h>
#include "../imagin.h"
#include "export_tiff.h"

void writeTIFF(const char *filename, struct Image *img)
{
    //Creating the file
    TIFF *out= TIFFOpen(filename, "w");
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
            errx(1,"export_tiff: Unable to write image.");
            break;
        }
    }


    TIFFClose(out);
    if (buffer)
        _TIFFfree(buffer);
}