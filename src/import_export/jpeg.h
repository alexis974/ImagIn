#ifndef JPEG_H
#define JPEG_H

struct Image *readJPEG(const char *filename);

void writeJPEG(const char *filename, struct Image *img);

#endif
