#include<stdio.h>
#include"../imagin.h"
#include"invert.h"

void invert(Image *img)
{
	size_t i;

	if (img)
	{
		for (i = 0; i < img->x * img->y; i++)
		{
			img->data[i].red = RGB_COMPONENT_COLOR-img->data[i].red;
			img->data[i].green = RGB_COMPONENT_COLOR-img->data[i].green;
			img->data[i].blue = RGB_COMPONENT_COLOR-img->data[i].blue;
		}
	}
}
