#ifndef PNG_H
#define PNG_H

struct Image *readPNG(const char *filename);
void writePNG(const char *filename, struct Image *img);

#endif
