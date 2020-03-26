#ifndef ZOOM_H
#define ZOOM_H

struct zoom2
{
    size_t x_down_full;
    size_t y_down_full;
    size_t x_up_full;
    size_t y_up_full;

    size_t x_down;
    size_t y_down;
    size_t x_up;
    size_t y_up;
};

float zoom_percentage(struct Images *images);

void zoom_init(struct Images *images, struct zoom2 *zoom);

struct Image *zoom(struct Images *images, struct zoom2 *zoom,
        size_t x_center, size_t y_center);

struct Image *unzoom(struct Images *images, struct zoom2 *zoom);

#endif /* ! ZOOM_H */
