#include <stdio.h>
#include <gtk/gtk.h>

#include "../../imagin.h"

#include "../../gui/gui.h"
#include "../../gui/gui_display.h"

#include "../user/crop.h"

#include "scale.h"

float zoom_percentage(struct Image *full_img)
{
    float percentage = (full_img->width * full_img->height) /
        (g_maxwidth * g_maxheight);

    return percentage;
}


struct Image *zoom(struct Image *full_img, float zoom_value, size_t x_center,
        size_t y_center)
{
    if (zoom_value > 100 || zoom_value < zoom_percentage(full_img))
    {
        zoom_value = zoom_value > 100 ? 100 : zoom_percentage(full_img);
    }

    size_t nb_x = g_maxwidth * 100 / zoom_value;
    size_t nb_y = g_maxheight * 100 / zoom_value;


    size_t x_down_left = 0;
    size_t y_down_left = 0;
    size_t x_up_right = 0;
    size_t y_up_right = 0;

    // X_DOWN_LEFT
    if ((x_center - (nb_x/2)) > 0)
    {
        x_down_left += x_center - (nb_x / 2);
    }
    else
    {
        x_down_left = 0;
        x_up_right += (x_center - (nb_x/2)) * -1 - x_center;
    }


    // X_UP_RIGHT
    if ((x_center + (nb_x/2)) < full_img->width)
    {
        x_up_right += x_center + (nb_x / 2);
    }
    else
    {
        x_up_right = full_img->width;
        x_down_left -= (x_center + (nb_x/2)) - (full_img->width - x_center);
    }

    // Y_UP_RIGHT
    if ((y_center - (nb_y / 2)) > 0)
    {
        y_up_right += y_center - (nb_y / 2);
    }
    else
    {
        y_up_right = 0;
        y_down_left += (y_center - (nb_y / 2)) * -1 - y_center;
    }



    // Y_DOWN_LEFT
    if ((y_center + (nb_y / 2)) < full_img->height)
    {
        y_down_left += y_center + (nb_y / 2);
    }
    else
    {
        y_down_left = full_img->height;
        y_up_right += (y_center + (nb_y / 2)) - (full_img->height - y_center);
    }

    return full_img;
}
