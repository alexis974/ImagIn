#ifndef IMPORT_H
#define IMPORT_H

const char *get_filename_ext(const char *filename);
struct Image *read(const char *filename);

#endif