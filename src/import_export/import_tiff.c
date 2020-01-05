#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include<math.h>
#include <tiffio.h>
#include"../imagin.h"
#include"import_tiff.h"

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
