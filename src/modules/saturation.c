#include <stdio.h>
#include <math.h>
#include <err.h>
#include "../imagin.h"
#include "saturation.h"

//For now, the saturation module doesn't handle increase saturation.
void saturation(Image *img, float change)
{
	size_t i;
	float P;

	if (img)
	{
		for (i = 0; i < img->width * img->height; i++)
		{
			P = sqrt(img->data[i].red * img->data[i].red * 0.299 +
			img->data[i].green * img->data[i].green * 0.587 +
			img->data[i].blue * img->data[i].blue * 0.144);

			img->data[i].red = P + ((img->data[i].red) - P) * change;
			img->data[i].green= P + ((img->data[i].green) - P) * change;
			img->data[i].blue = P + ((img->data[i].blue) - P) * change;
		}
	}
}
