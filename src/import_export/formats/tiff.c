#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <tiffio.h>
#include <err.h>
#include <stdint.h>
#include <gtk/gtk.h>

#include "../../imagin.h"
#include "../../debug/error_handler.h"

#include "../../tools/bits.h"

#include "tiff.h"

void tiff_get_pixel(struct Image *img, uint32 *raster, size_t i, size_t j)
{
    size_t bytes_per_sample =
        (img->bit_depth < 255 ? 1 : depth_to_bits(img->bit_depth) / 8);

    size_t width = img->width;
    size_t height = img->height;

    img->data[(height-j-1) * width + i].r = 0;
    img->data[(height-j-1) * width + i].b = 0;
    img->data[(height-j-1) * width + i].g = 0;

    for (size_t k = 0; k < bytes_per_sample; k++)
    {
        size_t b_t_d = bits_to_depth(k * 8) + 1;

        // TIFF is save from bottom to top
        img->data[(height-j-1) * width + i].r =
            (size_t)TIFFGetR(raster[j*width+ i + k]) * b_t_d;

        img->data[(height-j-1) * width + i].b =
            (size_t)TIFFGetB(raster[j*width+ i + k]) * b_t_d;

        img->data[(height-j-1) * width + i].g =
            (size_t)TIFFGetG(raster[j*width+ i + k]) * b_t_d;
    }
}

// TODO : Coding style : Fct 25 lines max
struct Image *read_tiff(const char *filename)
{
    TIFF *tif=TIFFOpen(filename, "r");
    if (!tif)
    {
        throw_error("readTIFF", "Unable to open file");

        return NULL;
    }

    uint32 width;
    uint32 height;
    uint16 bits_per_sample;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);

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
    img->bit_depth = bits_to_depth(bits_per_sample);

    img->data = malloc(img->width * img->height * sizeof(struct Pixel));
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
            tiff_get_pixel(img, raster, i, j);
        }
    }

    _TIFFfree(raster);
    TIFFClose(tif);
    printf("TIFF Image read!\n");

    return img;
}

// TODO : Coding style : Fct 25 lines max
void write_tiff(const char *filename, struct Image *img)
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
    TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, depth_to_bits(img->bit_depth));
    // Set the origin of the image
    TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    // Some other essential fields
    TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

    size_t bytes_per_samples =
        (img->bit_depth <= 255 ? 1 : depth_to_bits(img->bit_depth) / 8);

    size_t rowsize = sampleperpixel * img->width * bytes_per_samples;
    unsigned char *buffer = NULL;

    // Allocating memory to store the pixels of current row
    if (TIFFScanlineSize(out))
    {
        buffer = (unsigned char *)_TIFFmalloc(rowsize);
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
        if (img->bit_depth <= 255)
        {
            for (size_t i = 0; i < img->width; i++)
            {
                buffer[i*sampleperpixel] = img->data[j*img->width+i].r;
                buffer[i*sampleperpixel+1] = img->data[j*img->width+i].g;
                buffer[i*sampleperpixel+2] = img->data[j*img->width+i].b;
            }
        }
        else if (depth_to_bits(img->bit_depth) == 16)
        {
            uint16_t *row = malloc(sizeof(uint16_t) * 3 * img->width);

            for (size_t i = 0; i < img->width; i++)
            {
                row[i*3] = img->data[j*img->width+i].r;
                row[i*3+1] = img->data[j*img->width+i].g;
                row[i*3+2] = img->data[j*img->width+i].b;
            }

            memcpy(buffer, row, sizeof(unsigned char) * 3 * img->width * 2);

            free(row);
        }
        else if (depth_to_bits(img->bit_depth) == 32)
        {
            uint32_t *row = malloc(sizeof(uint32_t) * 3 * img->width);

            for (size_t i = 0; i < img->width; i++)
            {
                row[i*3] = img->data[j*img->width+i].r;
                row[i*3+1] = img->data[j*img->width+i].g;
                row[i*3+2] = img->data[j*img->width+i].b;
            }

            memcpy(buffer, row, sizeof(unsigned char) * 3 * img->width * 4);

            free(row);
        }
        else
        {
            throw_error("writeTIFF", "Unsupported bit depth");
            return;
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
