#include<stdio.h>
#include <err.h>
#include"../imagin.h"
#include"export_ppm.h"


void writePPM(const char *filename, struct Image *img)
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
	fprintf(fp, "%ld %ld\n", img->width, img->height);

	// rgb component depth
	fprintf(fp, "%ld\n", img->bit_depth);

	// pixel data
	fwrite(img->data, 3 * img->width, img->height, fp);
	fclose(fp);
	printf("PPM Image saved!\n");
}
