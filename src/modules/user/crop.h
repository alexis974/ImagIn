#ifndef CROP_H
#define CROP_H

struct Image *crop(struct Image *img, size_t x_down_left, size_t y_down_left,
        size_t x_up_right, size_t y_up_right);

#endif /* ! CROP_H */
