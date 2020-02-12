#ifndef TIFF_H
#define TIFF_H

struct Image *read_tiff(const char *filename);

void write_tiff(const char *filename, struct Image *img);

#endif /* ! TIFF_H */
