#include <gtk/gtk.h>

#include  "../../imagin.h"

#include  "../gui.h"

void draw_crop_rectangle(struct UI *ui, cairo_t *cr)
{
    // Clarify code by creating clear variable names
    GtkWidget *drawing_area = GTK_WIDGET(ui->display->display_image);
    int draw_area_width = gtk_widget_get_allocated_width(drawing_area);
    int draw_area_height = gtk_widget_get_allocated_height(drawing_area);
    size_t img_width = ui->images->scale->width;
    size_t img_height = ui->images->scale->height;
    // Padding / 2
    int origin_x = (draw_area_width - img_width) / 2;
    int origin_y = (draw_area_height - img_height) / 2;

    // Draw rectangle
    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_set_line_width(cr, 4);
    cairo_move_to(cr, origin_x, origin_y);
    cairo_rel_line_to(cr, img_width, 0);
    cairo_rel_line_to(cr, 0, img_height);
    cairo_move_to(cr, origin_x, origin_y);
    cairo_rel_line_to(cr, 0, img_height);
    cairo_rel_line_to(cr, img_width, 0);

    // Drawing four handles
    int handle_size = 0.05 * draw_area_width;
    cairo_move_to(cr, origin_x + handle_size, origin_y);
    cairo_rel_line_to(cr, 0, handle_size);
    cairo_rel_line_to(cr, -handle_size, 0);

    cairo_move_to(cr, origin_x + img_width - handle_size, origin_y);
    cairo_rel_line_to(cr, 0, handle_size);
    cairo_rel_line_to(cr, handle_size, 0);

    cairo_move_to(cr, origin_x + handle_size, origin_y + img_height);
    cairo_rel_line_to(cr, 0, -handle_size);
    cairo_rel_line_to(cr, -handle_size, 0);

    cairo_move_to(cr, origin_x + img_width - handle_size, origin_y + img_height);
    cairo_rel_line_to(cr, 0, -handle_size);
    cairo_rel_line_to(cr, handle_size, 0);

    cairo_stroke(cr);
}