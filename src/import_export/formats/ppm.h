#ifndef PPM_H
#define PPM_H

struct Image *read_ppm(const char *filename);

void write_ppm(const char *filename, struct Image *img);

#endif /* ! PPM_H */
