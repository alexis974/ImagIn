#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <gtk/gtk.h>
#include <png.h>

#include "../../imagin.h"

#include "png.h"

#include "../../tools/bits.h"


#include "../../debug/error_handler.h"

void png_get_pixel(struct Image *img, png_bytep *row_pointers,
    size_t i, size_t j, size_t nb_channel)
{
    size_t bytes_per_sample =
        (img->bit_depth < 255 ? 1 : depth_to_bits(img->bit_depth) / 8);

    img->data[img->width * j + i].r = 0;
    img->data[img->width * j + i].g = 0;
    img->data[img->width * j + i].b = 0;

    for (size_t k = 0; k < bytes_per_sample; k++)
    {
        size_t b_t_d = bits_to_depth(k * 8) + 1;
        img->data[img->width * j + i].r +=
            row_pointers[j][i * nb_channel * bytes_per_sample + k] * b_t_d;

        img->data[img->width * j + i].g +=
            row_pointers[j][(i * nb_channel + 1) * bytes_per_sample + k] *
                b_t_d;

        img->data[img->width * j + i].b +=
            row_pointers[j][(i * nb_channel + 2) * bytes_per_sample + k] *
                b_t_d;
    }
}

// TODO : Coding style : Fct 25 lines max
struct Image *read_png(const char *filename)
{
    FILE *fp = fopen(filename, "rb");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,
            NULL);
    if (!png)
    {
        throw_error("readPNG", "Unable to use png library.");

        return NULL;
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        throw_error("readPNG", "Unable to use png library.");

        return NULL;
    }

    if (setjmp(png_jmpbuf(png)))
    {
        throw_error("readPNG", "Unable to use png library.");

        return NULL;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    struct Image *img = malloc(sizeof(struct Image));
    if (!img)
    {
        throw_error("readPNG", "Unable to allocate memory.");

        return NULL;
    }

    img->width = png_get_image_width(png, info);
    img->height = png_get_image_height(png, info);
    img->data = malloc(sizeof(struct Pixel) * img->height * img->width);

    if (!img->data)
    {
        throw_error("readPNG", "Unable to allocate memory.");

        return NULL;
    }

    png_byte bits_per_sample = png_get_bit_depth(png, info);
    png_byte color_type = png_get_color_type(png, info);

    img->bit_depth = bits_to_depth(bits_per_sample);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_palette_to_rgb(png);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY && img->bit_depth < 255)
    {
        png_set_expand_gray_1_2_4_to_8(png);
    }

    if (png_get_valid(png, info, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(png);
    }

    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        png_set_gray_to_rgb(png);
    }

    png_read_update_info(png, info);

    // TODO : Coding style : Explicite cast ?
    png_bytep *row_pointers = (png_bytep*) malloc(
            sizeof(png_bytep) * img->height);

    for (size_t y = 0; y < img->height; y++)
    {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }

    png_byte nb_channel = png_get_channels(png,info);
    png_read_image(png, row_pointers);

    for (size_t j = 0; j < img->height; j++)
    {
        for (size_t i = 0; i < img->width; i++)
        {
            png_get_pixel(img, row_pointers, i, j, nb_channel);
        }
    }

    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);

    return img;
}

// TODO : Coding style : Fct 25 lines max
void write_png(const char *filename, struct Image *img)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        throw_error("writePNG", "Could not write file");

        return;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL);

    if (!png)
    {
        throw_error("writePNG", "Could not write file");

        return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        throw_error("writePNG", "Could not write file");

        return;
    }

    if (setjmp(png_jmpbuf(png)))
    {
        throw_error("writePNG", "Could not write file");

        return;
    }

    // Write header
    png_init_io(png, fp);

    png_set_IHDR(png, info, img->width, img->height,
            depth_to_bits(img->bit_depth), PNG_COLOR_TYPE_RGB,
            PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png, info);

    png_bytep *row_pointers = malloc(sizeof(png_bytep) * img->height);

    if(img->bit_depth > 255)
    {
        for (size_t j = 0; j < img->height; j++)
        {
            uint16_t *row = malloc(sizeof(size_t)*3*img->width);

            for (size_t i = 0; i < img->width; i++)
            {
                row[i*3] = img->data[j*img->width+i].r;
                row[i*3+1] = img->data[j*img->width+i].g;
                row[i*3+2] = img->data[j*img->width+i].b;
            }

            // swap bytes of 16 bit files to most significant bit first
            //png_set_swap(png);
            row_pointers[j] = malloc(sizeof(png_bytep)*3*img->width * 2);
            memcpy(row_pointers[j], row, sizeof(png_bytep)*3*img->width * 2);

            free(row);
        }
    }
    else
    {
        for (size_t j = 0; j < img->height; j++)
        {
            row_pointers[j] = malloc(sizeof(png_bytep)*3*img->width);

            for (size_t i = 0; i < img->width; i++)
            {
                row_pointers[j][i*3] = img->data[j*img->width+i].r;
                row_pointers[j][i*3+1] = img->data[j*img->width+i].g;
                row_pointers[j][i*3+2] = img->data[j*img->width+i].b;
            }
        }
    }

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    for (size_t j=0; j<img->height; j++)
    {
        free(row_pointers[j]);
    }

    free(row_pointers);
    fclose(fp);
    png_destroy_write_struct(&png, &info);
}
