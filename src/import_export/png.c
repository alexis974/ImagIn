#include <png.h>
#include <stdio.h>
#include <stdlib.h>

#include "../error_handler.h"
#include "../imagin.h"

struct Image *readPNG(const char *filename)
{
    FILE *fp = fopen(filename, "rb");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL);
    if (!png)
    {
        throw_error("readPNG","Unable to use png library.");
        return NULL;
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        throw_error("readPNG","Unable to use png library.");
        return NULL;
    }

    if (setjmp(png_jmpbuf(png)))
    {
        throw_error("readPNG","Unable to use png library.");
        return NULL;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    struct Image *img = malloc(sizeof(struct Image));
    if (!img)
    {
        throw_error("readPNG","Unable to allocate memory.");
        return NULL;
    }

    img->width = png_get_image_width(png, info);
    img->height = png_get_image_height(png, info);
    img->data = malloc(sizeof(struct Pixel) * img->height * img->width);

    if (!img->data)
    {
        throw_error("readPNG","Unable to allocate memory.");
        return NULL;
    }

    img->bit_depth = png_get_bit_depth(png, info);
    png_byte color_type = png_get_color_type(png, info);

    if (img->bit_depth == 16)
        png_set_strip_16(png);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    if (color_type == PNG_COLOR_TYPE_GRAY && img->bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    if (color_type == PNG_COLOR_TYPE_RGB ||
            color_type == PNG_COLOR_TYPE_GRAY ||
                color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    png_bytep *row_pointers = (png_bytep*)
        malloc(sizeof(png_bytep) * img->height);

    for(size_t y = 0; y < img->height; y++)
    {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }

    png_byte nb_channels = png_get_channels(png,info);
    png_read_image(png, row_pointers);

    for (size_t j = 0; j < img->height; j++)
    {
        for (size_t i = 0; i < img->width; i++)
        {
            img->data[img->width * j + i].red =
                row_pointers[j][i * nb_channels];
            img->data[img->width * j + i].green =
                row_pointers[j][i * nb_channels + 1];
            img->data[img->width * j + i].blue =
                row_pointers[j][i * nb_channels + 2];
        }
    }

    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);
    return img;
}
