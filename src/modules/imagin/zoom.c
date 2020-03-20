#include <stdio.h>
#include <gtk/gtk.h>
#include <err.h>

#include "../../imagin.h"

#include "../../gui/gui.h"
#include "../../gui/gui_display.h"

#include "../user/crop.h"

#include "scale.h"

#include "zoom.h"

float zoom_percentage(struct Images *images)
{
    // DO NOT REMOVE EXPLICIT CAST IF YOU ARE NOT
    // SURE THAT YOU FIND A SOLUTION TO KEEP DECIMALS
    float percentage = (float)(images->scale->width * 100) / images->full->width;

    return percentage;
}


void zoom_init(struct Images *images, struct zoom2 *zoom)
{
    zoom->x_down_full = 0;
    zoom->y_down_full = 0;
    zoom->x_up_full = images->full->width - 1;
    zoom->y_up_full = images->full->height - 1;

    zoom->x_down = 0;
    zoom->y_down = 0;
    zoom->x_up = images->full->width - 1;
    zoom->y_up = images->full->height - 1;
}


struct Image *zoom(struct Images *images, float *zoom_value, size_t x_center,
        size_t y_center)
{
    if (!images)
    {
        errx(1, "zoom: No images found");
    }

    if (x_center >=  images->full->width || y_center > images->full->height)
    {
        errx(1, "zoom: Center coordonates invalid");
    }

    //Not use yet;
    *zoom_value *= 1.2;

    struct zoom2 *zoom = malloc(sizeof(struct zoom2));
    zoom_init(images, zoom);

    //Convert center to full image
    float w_ratio = zoom->x_down_full / images->scale->width;
    float h_ratio = zoom->y_down_full / images->scale->height;
    x_center *= w_ratio;
    y_center *= h_ratio;

    size_t nb_x = (zoom->x_up - zoom->x_down) * 2 / 3;
    size_t nb_y = (zoom->y_up - zoom->y_down) * 2 / 3;


    // X
    // Can go left no prob
    if (x_center > (nb_x / 2))
    {
        zoom->x_down -= (nb_x / 2);
    }
    // Goes to far left
    else
    {
        zoom->x_down = 0;
        zoom->x_up += (nb_x / 2) - (zoom->x_down);
    }

    // Can go right no prob
    if (x_center + (nb_x / 2) < images->full->width - 1)
    {
        zoom->x_up += (nb_x / 2);
    }
    // Goes to far right
    else
    {
        zoom->x_up = images->full->width - 1;
        zoom->x_down -= (nb_x / 2) - ((images->full->width - 1) - x_center);
    }

    // Y
    // Can go down no prob
    if (y_center > (nb_y / 2))
    {
        zoom->y_down -= (nb_y / 2);
    }
    // Goes to far to bottom
    else
    {
        zoom->y_down = 0;
        zoom->y_up += (nb_y / 2) - (zoom->y_down);
    }

    // Can go up no prob
    if (y_center + (nb_y / 2) < images->full->height - 1)
    {
        zoom->y_up += (nb_y / 2);
    }
    // Goes to far up
    else
    {
        zoom->y_up = images->full->height - 1;
        zoom->y_down -= (nb_y / 2) - ((images->full->height - 1) - y_center);
    }

    printf("center:(%ld, %ld) || nb_x = %ld | nb_y = %ld\n", x_center, y_center, nb_x, nb_y);
    printf("(%ld, %ld) | (%ld, %ld)\n", zoom->x_down, zoom->y_down, zoom->x_up, zoom->y_up);
    return crop(images->full, zoom->x_down, zoom->y_down, zoom->x_up, zoom->y_up);
}
