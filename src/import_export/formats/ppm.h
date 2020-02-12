#ifndef PPM_H
#define PPM_H

struct Image *readPPM(const char *filename);

void writePPM(const char *filename, struct Image *img);

#endif /* ! PPM_H */
