#include <gtk/gtk.h>

#include  "../../imagin.h"

#include "../gui.h"
#include "../gui_display.h"

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

// Setup the four handles at beginning (at each corner)
void set_crop_handles_coordinates(struct UI *ui)
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

    ui->modules->crop->handles[0].x = origin_x;
    ui->modules->crop->handles[0].y = origin_y;

    ui->modules->crop->handles[1].x = origin_x + img_width;
    ui->modules->crop->handles[1].y = origin_y;

    ui->modules->crop->handles[2].x = origin_x + img_width;
    ui->modules->crop->handles[2].y = origin_y + img_height;

    ui->modules->crop->handles[3].x = origin_x;
    ui->modules->crop->handles[3].y = origin_y + img_height;
}

void draw_crop_rectangle(struct UI *ui, cairo_t *cr)
{
    // Clarify code by creating clear variable names
    GtkWidget *drawing_area = GTK_WIDGET(ui->display->display_image);
    int draw_area_width = gtk_widget_get_allocated_width(drawing_area);

    struct Coordinates *handle = ui->modules->crop->handles;

    // Draw rectangle
    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_set_line_width(cr, 4);
    cairo_move_to(cr, handle[0].x, handle[0].y);
    cairo_line_to(cr, handle[1].x, handle[1].y);
    cairo_line_to(cr, handle[2].x, handle[2].y);
    cairo_line_to(cr, handle[3].x, handle[3].y);
    cairo_line_to(cr, handle[0].x, handle[0].y);

    // Drawing four handles
    int handle_size = 0.05 * draw_area_width;
    cairo_move_to(cr, handle[0].x + handle_size, handle[0].y);
    cairo_rel_line_to(cr, 0, handle_size);
    cairo_rel_line_to(cr, -handle_size, 0);

    cairo_move_to(cr, handle[1].x - handle_size, handle[1].y);
    cairo_rel_line_to(cr, 0, handle_size);
    cairo_rel_line_to(cr, handle_size, 0);

    cairo_move_to(cr, handle[2].x - handle_size, handle[2].y);
    cairo_rel_line_to(cr, 0, -handle_size);
    cairo_rel_line_to(cr, handle_size, 0);

    cairo_move_to(cr, handle[3].x + handle_size, handle[3].y);
    cairo_rel_line_to(cr, 0, -handle_size);
    cairo_rel_line_to(cr, -handle_size, 0);

    cairo_stroke(cr);
}

void crop_motion_event(GdkEventMotion *event, struct UI *ui)
{
    int selected = ui->modules->crop->selected_handle;
    selected = 0;
    if (selected != -1)
    {
        ui->modules->crop->handles[selected].x =
            event->x;
        ui->modules->crop->handles[selected].y =
            event->y;

        // Moving others to keep it a rectangle
        if (selected == 0 || selected == 2)
        {
            ui->modules->crop->handles[mod((selected - 1), 4)].x =
                event->x;
            ui->modules->crop->handles[mod((selected + 1), 4)].y =
                event->y;
        }
        else
        {
            ui->modules->crop->handles[mod((selected + 1), 4)].x =
                event->x;
            ui->modules->crop->handles[mod((selected - 1), 4)].y =
                event->y;
        }

        gtk_widget_queue_draw_area(GTK_WIDGET(ui->display->display_image),0,0,
        gtk_widget_get_allocated_width(
        GTK_WIDGET(ui->display->display_image)),
        gtk_widget_get_allocated_height(
        GTK_WIDGET(ui->display->display_image)));
    }
}