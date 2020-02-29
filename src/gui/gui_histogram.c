#include <gtk/gtk.h>

#include "../imagin.h"

#include "gui.h"

#include "../modules/imagin/histogram.h"

gboolean draw_histogram(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    struct UI *ui = user_data;

    if (!ui->image_loaded)
    {
        return FALSE;
    }

    float height =  gtk_widget_get_allocated_height(widget);
    int width = gtk_widget_get_allocated_width(widget);

    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    cairo_paint(cr);

    struct Histogram *histogram = compute_histogram(ui->images->small);

    float max_value = histo_max_value(histogram);
    float scale = height/max_value;

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_set_line_width(cr,2);

    cairo_move_to(cr, 0, height);

    for (size_t i = 0; i < 256; i++)
    {
        size_t value = histogram->white[i];
        value *= scale;
        cairo_line_to(cr, i * width/256, height-value);
    }

    cairo_line_to(cr, width, height);

    cairo_fill(cr);

    cairo_stroke(cr);
    free_histogram(histogram);
    return FALSE;
}
