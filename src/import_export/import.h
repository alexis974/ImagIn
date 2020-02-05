#ifndef IMPORT_H
#define IMPORT_H

//Max dimensions for scaled image
size_t g_maxheight;
size_t g_maxwidth;

//Max dimensions for small image
size_t g_maxheight_small;
size_t g_maxwidth_small;

const char *get_filename_ext(const char *filename);

struct Image *get_small(struct Image *scaled_img);

struct Images *read_image(const char *filename);

#endif
