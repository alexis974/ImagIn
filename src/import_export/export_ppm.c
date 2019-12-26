#include<stdio.h>
#include <err.h>
#include"../imagin.h"
#include"export_ppm.h"


 void writePPM(const char *filename, Image *img)
{
	FILE *fp;
	//open file for output
	fp = fopen(filename, "wb");
	if (!fp)
	{
		errx(1, "Unable to open file '%s'\n", filename );
	}

	//write the header file
	//image format
	fprintf(fp, "P6\n");

	//comments
	fprintf(fp, "# Image created by SilverGrain\n");

	//image size
	fprintf(fp, "%ld %ld\n",img->x,img->y);

	// rgb component depth
	fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);

	// pixel data
	fwrite(img->data, 3 * img->x, img->y, fp);
	fclose(fp);
	printf("Image saved!\n");
}
