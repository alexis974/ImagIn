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
    printf("JPEG Image read!\n");
    fclose(infile);
    return img;
}

void writeJPEG(const char *filename, struct Image *img)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE * outfile;
    JSAMPROW row_pointer[1];
    int row_stride;
    cinfo.err = jpeg_std_error(&jerr);
    /*Initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);
    if ((outfile = fopen(filename, "wb")) == NULL)
    {
        errx(1 , "writeJPEG: Unable to open file '%s'\n", filename);
    }

    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = img->width;
    cinfo.image_height = img->height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 100, TRUE);
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = img->width * 3;

    row_pointer[0] = malloc(sizeof(unsigned char)*row_stride);
    if (!row_pointer[0])
    {
        errx(1,"writeJPEG : Unable to allocate.");
    }
    while (cinfo.next_scanline < cinfo.image_height)
    {
        //Filling Buffer
        for (size_t i = 0; i < img->width; i++)
        {
            row_pointer[0][i*3] = img->data[cinfo.next_scanline*img->width+i].red;
            row_pointer[0][i*3+1] = img->data[cinfo.next_scanline*img->width+i].green;
            row_pointer[0][i*3+2] = img->data[cinfo.next_scanline*img->width+i].blue;
        }
        //Writing in image
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
    printf("JPEG Image saved!\n");
}