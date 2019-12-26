#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include<math.h>
#include"../imagin.h"
#include"import_ppm.h"

Image *readPPM(const char *filename)
{
	char buff[16];
	Image *img;
	FILE *fp;
	int c, rgb_comp_color;

	//open PPM file for reading
	fp = fopen(filename, "rb");
	if (!fp)
	{
		errx(1 , "readPPM: Unable to open file '%s'\n", filename);
	}

	//read image format
	if (!fgets(buff, sizeof(buff), fp))
	{
		errx(1, "readPPM: Wrong image format for '%s'\n", filename);
	}

	//check the image format
	if (buff[0] != 'P' || buff[1] != '6')
	{
		errx(1, "readPPM: Invalid image format (must be 'P6')\n");
	}

	//alloc memory form image
	img = malloc(sizeof(Image));
	if (!img)
	{
		errx(1, "readPPM: Unable to allocate memory\n");
	}

	//check for comments
	c = getc(fp);
	while (c == '#')
	{
		while (getc(fp) != '\n')
		{
			c = getc(fp);
		}
	}

	ungetc(c, fp);
	//read image size information
	if (fscanf(fp, "%ld %ld", &img->x, &img->y) != 2)
	{
		errx(1, "readPPM: Invalid image size (error loading '%s')\n", filename);
	}

	//read rgb component
	if (fscanf(fp, "%d", &rgb_comp_color) != 1)
	{
		errx(1, "readPPM: Invalid rgb component (error loading '%s')\n", filename);
	}

	//check rgb component depth
	if (rgb_comp_color!= RGB_COMPONENT_COLOR)
	{
		errx(1, "readPPM: '%s' does not have 8-bits components\n", filename);
	}

	//memory allocation for pixel data
	while (fgetc(fp) != '\n') ;
	img->data = (Pixel*)malloc(img->x * img->y * sizeof(Pixel));

	if (!img)
	{
		errx(1, "readPPM: Unable to allocate memory\n");
	}

	//read pixel data from file
	if (fread(img->data, 3 * img->x, img->y, fp) != img->y)
	{
		errx(1, "readPPM: Error loading image '%s'\n", filename);
	}

	fclose(fp);
	printf("Image imported!\n");
	return img;
}
