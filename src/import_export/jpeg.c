#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <setjmp.h>
#include "../imagin.h"
#include "jpeg.h"

//Check libjpeg example.c

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;


struct Image *readJPEG(const char *filename)
{
    //Store image info
    struct jpeg_decompress_struct cinfo;

    //Source file
    FILE *infile;

    if(!(infile = fopen(filename, "rb")))
    {
        errx(1 , "readJPEG: Unable to open file '%s'\n", filename);
    }

    //Setting up errors management
    struct my_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr.pub);
    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return 0;
    }

    //Getting info
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    if(cinfo.output_components != 3)
        errx(1,"readJPEG: Too many components (alpha canal ?)");

    //Setting info
    struct Image *img = malloc(sizeof(struct Image));
    if(!img)
    {
        errx(1, "readJPEG: Unable to allocate memory.");
    }
    img->width = cinfo.image_width;
    img->height = cinfo.image_height;
    img->bit_depth = 255;
    img->data = malloc(sizeof(struct Pixel) * img->width * img->height);
    if(!img->data)
    {
        errx(1, "readJPEG: Unable to allocate memory.");
    }

    JSAMPARRAY buffer;//Output row buffer
    size_t row_stride;//Physical row width in output buffer
    //JSAMPLEs per row in output buffer
    row_stride = cinfo.output_width * cinfo.output_components;
    //Make a one-row-high sample array that will go away when done with image
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    buffer[0] = (JSAMPROW)malloc(sizeof(JSAMPLE) * row_stride);
    //While there is at least one line left
    while (cinfo.output_scanline < cinfo.output_height)
    {
        //Reading row
        jpeg_read_scanlines(&cinfo, buffer, 1);
        for (size_t i = 0; i < img->width; i++)
        {
            img->data[img->width * (cinfo.output_scanline - 1) + i].red = (unsigned char)buffer[0][i * 3];
            img->data[img->width * (cinfo.output_scanline - 1) + i].green = (unsigned char)buffer[0][i * 3 + 1];
            img->data[img->width * (cinfo.output_scanline - 1) + i].blue = (unsigned char)buffer[0][i * 3 + 2];
        }
    }

    //Closing everything and freeing memory
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return img;
}