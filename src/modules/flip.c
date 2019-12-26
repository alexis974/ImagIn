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


void flip(Image *img)
{
	int i = 0;
	unsigned char tmp;
	int index_fin = img->x * img->y;

	if (img)
	{
		while(i < index_fin)
		{
			tmp = img->data[i].red;

			img->data[i].red = img->data[index_fin].red;
			img->data[i].green = img->data[index_fin].green;
			img->data[i].blue = img->data[index_fin].blue;

			img->data[index_fin].red = tmp;
			img->data[index_fin].green = tmp;
			img->data[index_fin].blue = tmp;

			i++;
			index_fin--;
		}
	}
}
