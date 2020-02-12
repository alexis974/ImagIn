#ifndef JPEG_H
#define JPEG_H

struct Image *read_jpeg(const char *filename);

void write_jpeg(const char *filename, struct Image *img);

#endif /* ! JPEH_H */
