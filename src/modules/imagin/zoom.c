#include <stdio.h>
#include <gtk/gtk.h>

#include "../../imagin.h"

#include "../../gui/gui.h"
#include "../../gui/gui_display.h"

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

    /*
     *
     */
    x_center++;
    y_center++;

    return full_img;
}
