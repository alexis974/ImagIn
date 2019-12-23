#include <err.h>

void init_image()
{
	image->coordinates = malloc(8 * sizeof(size_t))
	image->size = malloc(2 * sizeof(size_t))
	image->pixel = malloc(3 * image->size[0] * image->size[1] * sizeof(size_t));
	//TODO
}
