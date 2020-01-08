#ifndef TIFF_H
#define TIFF_H

struct Image *readTIFF(const char *filename);

void writeTIFF(const char *filename, struct Image *img);

#endif