#include <stdio.h>
#include <err.h>
#include "../imagin.h"
#include "flip.h"


void horizontal_flip(Image *img)
{
	unsigned char tmp;
	size_t offset = img->x;
	size_t index_top;
	size_t index_bottom;

	if (img)
	{
		for (size_t x = 0; x < img->x; x++)
		{
			for (size_t y = 0; y < (img->y / 2) ; y++)
			{
				index_top = y * offset + x;
				index_bottom = offset * (img->y - y) + x - offset;

				tmp = img->data[index_top].red;
				img->data[index_top].red = img->data[index_bottom].red;
				img->data[index_bottom].red = tmp;

				tmp = img->data[index_top].green;
				img->data[index_top].green = img->data[index_bottom].green;
				img->data[index_bottom].green = tmp;

				tmp = img->data[index_top].blue;
				img->data[index_top].blue = img->data[index_bottom].blue;
				img->data[index_bottom].blue = tmp;
			}
		}
	}
}


void flip_both_axis(Image *img)
{
	unsigned char tmp;
	size_t index_end = img->x * img->y;

	if (img)
	{
		for (size_t i = 0; i < index_end; i++, index_end--)
		{
			tmp = img->data[i].red;
			img->data[i].red = img->data[index_end].red;
			img->data[index_end].red = tmp;

			tmp = img->data[i].green;
			img->data[i].green = img->data[index_end].green;
			img->data[index_end].green = tmp;

			tmp = img->data[i].blue;
			img->data[i].blue = img->data[index_end].blue;
			img->data[index_end].blue = tmp;
		}
	}
}
