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

size_t get_x_pixel(struct Images *images, struct zoom2 *zoom, size_t x_center)
{
    float w_ratio = zoom->x_up_full / images->scale->width;
    x_center *= w_ratio;
    return x_center;
}


size_t get_y_pixel(struct Images *images, struct zoom2 *zoom, size_t y_center)
{
    float h_ratio = zoom->y_up_full / images->scale->height;
    y_center *= h_ratio;
    return y_center;
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
    x_center = get_x_pixel(images, zoom, x_center);
    y_center = get_y_pixel(images, zoom, y_center);

    //Get width and height of crop image
    size_t nb_x = (zoom->x_up - zoom->x_down) * 2 / 3;
    size_t nb_y = (zoom->y_up - zoom->y_down) * 2 / 3;

    if ((zoom->x_up - zoom->x_down) < nb_x)
    {
        errx(1, "zoom: Can't crop with this width");
    }
    if ((zoom->y_up - zoom->y_down) < nb_y)
    {
        errx(1, "zoom: Can't crop with this height");
    }

    printf("\npath = ");


    // X
    // Can go left no prob
    if (x_center - (nb_x / 2) > zoom->x_down && (x_center - (nb_x / 2)) < zoom->x_up_full)
    {
        printf("1");
        zoom->x_down = x_center - (nb_x / 2);

        //Can go right no prob
        if (x_center + (nb_x / 2) < zoom->x_up)
        {
            printf("3");
            zoom->x_up = x_center + (nb_x / 2);
        }
        else
        {
            printf("4");
            zoom->x_up = zoom->x_up;
            zoom->x_down = zoom->x_up - (nb_x);
        }
    }
    // Goes to far left
    else
    {
        printf("2");
        zoom->x_down = zoom->x_down;
        zoom->x_up = zoom->x_down + nb_x;
    }

    // Y
    // Can go down no prob
    if (y_center - (nb_y / 2) > zoom->y_down && (y_center - (nb_y / 2)) < zoom->y_up_full)
    {
        printf("5");
        zoom->y_down = y_center - (nb_y / 2);

        //Can go up no prob
        if (y_center + (nb_y / 2) < zoom->y_up)
        {
            printf("7");
            zoom->y_up = y_center + (nb_y / 2);
        }
        else
        {
            printf("8");
            zoom->y_up = zoom->y_up;
            zoom->y_down = zoom->y_up - (nb_y / 2);
        }
    }
    // Goes to far down
    else
    {
        printf("6");
        zoom->y_down = zoom->y_down;
        zoom->y_up = zoom->y_down + nb_y;
    }


    printf("\n");


    printf("center:(%ld, %ld) || nb_x = %ld | nb_y = %ld\n", x_center, y_center, nb_x, nb_y);
    printf("GOT  : (%ld, %ld) | (%ld, %ld)\n", zoom->x_down, zoom->y_down, zoom->x_up, zoom->y_up);
    printf("FULL : (%ld, %ld) | (%ld, %ld)\n", zoom->x_down_full, zoom->y_down_full, zoom->x_up_full, zoom->y_up_full);


    // TEST :
    if (zoom->x_up > zoom->x_up_full)
    {
        errx(1, "x_up is bigger than image width");
    }
    if (zoom->y_up > zoom->y_up_full)
    {
        errx(1, "y_up is bigger than image height");
    }


    return crop(images->full, zoom->x_down, zoom->y_down, zoom->x_up, zoom->y_up);
}

/*
struct Image *unzoom(struct Images *images, float *zoom_value, size_t x_center,
        size_t y_center)
{
    //
}
*/
