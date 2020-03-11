#ifndef ZOOM_H
#define ZOOM_H

float zoom_percentage(struct Images *images);

struct Image *zoom(struct Images *images, float zoom_value, size_t x_center,
        size_t y_center);

#endif /* ! ZOOM_H */
