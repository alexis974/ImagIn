#ifndef FLIP_H
#define FLIP_H

void flip(struct Image *img, size_t value);

void vertical_flip(struct Image *img);

void horizontal_flip(struct Image *img);

void flip_both_axis(struct Image *img);

#endif /* ! FLIP_H */
