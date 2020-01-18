#include <stdlib.h>

#include "free.h"

#include "../imagin.h"

void free_image(struct Image *img)
{
    free(img->data);
    free(img);
    return;
}

void free_images(struct Images *images)
{
    free_image(images->full);
    free_image(images->scale);
    free_image(images->edit);
    free_image(images->small);
    free(images);
    return;
}
