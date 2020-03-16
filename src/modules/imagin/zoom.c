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
    // DO NOT REMOVE EXPLICIT CAST IF YOU ARE NOT
    // SURE THAT YOU FIND A SOLUTION TO KEEP DECIMALS
    float percentage = (float)(images->scale->width * 100) / images->full->width;

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

    // To remove if you work on absolute positionning
    float tmp_x = x_center;
    float  tmp_y = y_center;
    float temp_x = (tmp_x / images->scale->width) * images->full->width;
    float temp_y = (tmp_y / images->scale->height)  * images->full->height;
    x_center =  temp_x;
    y_center = temp_y;

    size_t nb_x = (images->edit->width) * 100 / *zoom_value;
    size_t nb_y = (images->edit->height) * 100 / *zoom_value;

    size_t x_down_left = 0;
    size_t y_down_left = 0;
    size_t x_up_right = 0;
    size_t y_up_right = 0;

    // X
    if (x_center > (nb_x/2))
    {
        if ((x_center + (nb_x/2)) < images->full->width)
        {
            // Everything fine on x
            x_down_left = x_center - (nb_x / 2);
            x_up_right = x_center + (nb_x / 2);
        }
        else // Goes to far on right
        {
            x_up_right = images->full->width;
            x_down_left = images->full->width - nb_x;
        }
    }
    else // Goes too far on left
    {
        x_down_left = 0;
        x_up_right = nb_x;
    }

    // Y
    if (y_center > (nb_y/2))
    {
        if ((y_center + (nb_y/2)) < images->full->height)
        {
            // Everything fine on y
            y_down_left = y_center - (nb_y / 2);
            y_up_right = y_center + (nb_y / 2);
        }
        else // Goes to far on bottom
        {
            y_up_right = images->full->height;
            y_down_left = images->full->height - nb_y;
        }
    }
    else // Goes too far on top
    {
        y_down_left = 0;
        y_up_right = nb_y;
    }

    printf("%ld | %ld | %ld | %ld\n", x_down_left, y_down_left, x_up_right, y_up_right);

    return crop(images->full, x_down_left, y_down_left, x_up_right, y_up_right);
}
