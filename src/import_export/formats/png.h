#ifndef PNG_H
#define PNG_H

struct Image *read_png(const char *filename);

void write_png(const char *filename, struct Image *img);

#endif /* ! PNG_H */
