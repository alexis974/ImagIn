#include <stdio.h>
#include <gtk/gtk.h>
#include <err.h>

#include "../../imagin.h"

#include "../../gui/gui.h"
#include "../../gui/gui_display.h"

#include "../user/crop.h"

#include "scale.h"

float zoom_percentage(struct Images *images)
{
    float percentage = (images->scale->width * 100) / images->full->width;

    return percentage;
}


struct Image *zoom(struct Images *images, float *zoom_value, size_t x_center,
        size_t y_center)
{
    if (!images)
    {
        errx(1, "zoom: No images found");
    }

    if (*zoom_value > 100)
    {
        *zoom_value = 100;
    }
    if (*zoom_value - 0.01 < zoom_percentage(images))
    {
        *zoom_value = zoom_percentage(images);
        return images->full;
    }

    float tmp_x = x_center;
    float  tmp_y = y_center;
    float temp_x = (tmp_x / images->scale->width) * images->full->width;
    float temp_y = (tmp_y / images->scale->height)  * images->full->height;
    x_center =  temp_x;
    y_center = temp_y;
    printf("%zu\n", x_center);
    printf("%zu\n", y_center);

    size_t nb_x = (images->scale->width) * 100 / *zoom_value;
    size_t nb_y = (images->scale->height) * 100 / *zoom_value;

    printf("nb_x = %ld | nb_y = %ld\n", nb_x, nb_y);

    size_t x_down_left = 0;
    size_t y_down_left = 0;
    size_t x_up_right = 0;
    size_t y_up_right = 0;

    printf("x_down_left = %ld | x_up_right = %ld\n", x_down_left, x_up_right);
    // X_DOWN_LEFT
    if ((x_center > (nb_x/2)))
    {
        x_down_left += x_center - (nb_x / 2);
    }
    else
    {
        x_down_left = 0;
        x_up_right = nb_x;
    }
    printf("x_down_left = %ld | x_up_right = %ld\n", x_down_left, x_up_right);

    // X_UP_RIGHT
    if ((x_center + (nb_x/2)) < images->full->width)
    {
        x_up_right += x_center + (nb_x / 2);
    }
    else
    {
        x_up_right = images->full->width;
        x_down_left = images->full->width - nb_x;
    }
    printf("x_down_left = %ld | x_up_right = %ld\n", x_down_left, x_up_right);

    // Y_UP_RIGHT
    // Checked
    if ((y_center + (nb_y / 2)) < images->full->height)
    {
        y_up_right += y_center + (nb_y / 2);
    }
    else
    {
        y_up_right = images->full->height;
        y_down_left = images->full->height - nb_y;
    }

    // Y_DOWN_LEFT
    if ((y_center > (nb_y / 2)))
    {
        y_down_left += y_center - (nb_y / 2);
    }
    else
    {
        y_down_left = 0;
        y_up_right = nb_y;
    }

    printf("%ld | %ld | %ld | %ld\n", x_down_left, y_down_left, x_up_right, y_up_right);

    return crop(images->full, x_down_left, y_down_left, x_up_right, y_up_right);
}
