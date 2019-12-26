#include <stdio.h>
#include <err.h>
#include "../imagin.h"
#include "flip.h"

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
