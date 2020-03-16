#ifndef IMPORT_H
#define IMPORT_H

const char *get_filename_ext(const char *filename);

struct Image *get_small(struct Image *scaled_img);

struct Images *read_image(const char *filename);

struct Image *get_scale(struct Image *full_img);

#endif /* ! IMPORT_H */
