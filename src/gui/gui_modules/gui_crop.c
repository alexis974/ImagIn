#include <math.h>
#include <gtk/gtk.h>

#include  "../../imagin.h"

#include "../gui.h"
#include "../gui_display.h"

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

long clamp(long value, long min, long max)
{
    if (value < min)
        return min;
    return (value > max ? max : value);
}

double distance(struct Coordinates a, struct Coordinates b)
{
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

// Setup the four handles at beginning (at each corner)
void set_crop_handles_coordinates(struct UI *ui)
{
    // Clarify code by creating clear variable names
    GtkWidget *drawing_area = GTK_WIDGET(ui->display->display_image);
    int draw_area_width = gtk_widget_get_allocated_width(drawing_area);
    int draw_area_height = gtk_widget_get_allocated_height(drawing_area);
    size_t img_width = ui->images->edit->width;
    size_t img_height = ui->images->edit->height;
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
    int draw_area_height = gtk_widget_get_allocated_height(drawing_area);

    struct Coordinates *handle = ui->modules->crop->handles;

    //Opacity mask
    cairo_rectangle(cr, handle[0].x, handle[0].y,
        handle[1].x - handle[0].x,
        handle[3].y - handle[0].y);
    cairo_rectangle(cr, 0, 0, draw_area_width, draw_area_height);
    cairo_set_source_rgba(cr, 0.4, 0.4, 0.4, 0.6);
    cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
    cairo_fill(cr);
    cairo_set_fill_rule(cr, CAIRO_FILL_RULE_WINDING);

    // Drawing four handles
    cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
    cairo_set_line_width(cr, 2);
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

    // Draw rectangle
    cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 3);
    cairo_rectangle(cr, handle[0].x, handle[0].y,
        handle[1].x - handle[0].x,
        handle[3].y - handle[0].y);
    cairo_stroke(cr);
}

void crop_on_click(GdkEventButton *event, struct UI *ui)
{
    struct Coordinates mouse;
    mouse.x = event->x;
    mouse.y = event->y;

    struct Coordinates *handle = ui->modules->crop->handles;

    GtkWidget *drawing_area = GTK_WIDGET(ui->display->display_image);
    int draw_area_width = gtk_widget_get_allocated_width(drawing_area);

    float proximity = draw_area_width * 0.05;

    // Getting selected handle if there is one
    if (mouse.x >= handle[0].x && mouse.x <= handle[0].x + proximity)
    {
        if (mouse.y >= handle[0].y && mouse.y <= handle[0].y + proximity)
        {
            ui->modules->crop->selected_handle = 0;
        }
        else if (mouse.y >= handle[3].y - proximity && mouse.y <= handle[3].y)
        {
            ui->modules->crop->selected_handle = 3;
        }
    }
    else if (mouse.x >= handle[1].x - proximity && mouse.x <= handle[1].x)
    {
        if (mouse.y >= handle[1].y && mouse.y <= handle[1].y + proximity)
        {
            ui->modules->crop->selected_handle = 1;
        }
        else if (mouse.y >= handle[2].y - proximity && mouse.y <= handle[2].y)
        {
            ui->modules->crop->selected_handle = 2;
        }
    }
}

void crop_motion_event(GdkEventMotion *event, struct UI *ui)
{
    int selected = ui->modules->crop->selected_handle;

    if (selected != -1)
    {
        GtkWidget *drawing_area = GTK_WIDGET(ui->display->display_image);
        int draw_area_width = gtk_widget_get_allocated_width(drawing_area);
        int draw_area_height = gtk_widget_get_allocated_height(drawing_area);
        size_t img_width = ui->images->edit->width;
        size_t img_height = ui->images->edit->height;
        // Padding / 2
        int origin_x = (draw_area_width - img_width) / 2;
        int origin_y = (draw_area_height - img_height) / 2;

        struct Coordinates *handle = ui->modules->crop->handles;

        // Future coordinates that will be applied after some verifications
        long new_x = handle[selected].x +
            event->x - ui->mouse->last_position.x;
        long new_y = handle[selected].y +
            event->y - ui->mouse->last_position.y;

        // min gap between to side
        float min_gap = draw_area_width * 0.05 * 2 + 20;

        GdkRectangle old_rectangle;
        old_rectangle.x = handle[0].x;
        old_rectangle.y = handle[0].y;
        old_rectangle.width = handle[1].x- handle[0].x;
        old_rectangle.height = handle[3].y - handle[0].y;

        //Checking if not too small on x
        if (labs(new_x - handle[mod(selected + 2, 4)].x) >= min_gap)
        {
            // Prevent from being outside image on x
            handle[selected].x = clamp(new_x, origin_x, origin_x + img_width);
        }

        //Checking if not too small on y
        if (labs(new_y - handle[mod(selected + 2, 4)].y) >= min_gap)
        {
            // Prevent from being outside image on y
            handle[selected].y = clamp(new_y, origin_y, origin_y + img_height);
        }

        // Moving other sides to keep it a rectangle
        if (selected == 0 || selected == 2)
        {
            ui->modules->crop->handles[mod((selected - 1), 4)].x =
                ui->modules->crop->handles[selected].x;
            ui->modules->crop->handles[mod((selected + 1), 4)].y =
                ui->modules->crop->handles[selected].y;
        }
        else
        {
            ui->modules->crop->handles[mod((selected + 1), 4)].x =
                ui->modules->crop->handles[selected].x;
            ui->modules->crop->handles[mod((selected - 1), 4)].y =
                ui->modules->crop->handles[selected].y;
        }

        /*gtk_widget_queue_draw_area(GTK_WIDGET(ui->display->display_image),0,0,
        gtk_widget_get_allocated_width(
        GTK_WIDGET(ui->display->display_image)),
        gtk_widget_get_allocated_height(
        GTK_WIDGET(ui->display->display_image)));*/

        GdkRectangle new_rectangle;
        new_rectangle.x = handle[0].x;
        new_rectangle.y = handle[0].y;
        new_rectangle.width = handle[1].x- handle[0].x;
        new_rectangle.height = handle[3].y - handle[0].y;

        gdk_rectangle_union(&old_rectangle, &new_rectangle, &old_rectangle);

        gtk_widget_queue_draw_area(GTK_WIDGET(ui->display->display_image),
            old_rectangle.x - 2, old_rectangle.y - 2,
            old_rectangle.width + 4, old_rectangle.height + 4);
    }
}

void start_btn(GtkWidget *w, gpointer user_data)
{
    (void) w;
    struct UI *ui = user_data;

    if (!ui->image_loaded)
        return;

    if (ui->modules->crop->is_active == 0)
    {
        gtk_button_set_label(ui->modules->crop->start_btn, "Leave");
        gtk_widget_set_sensitive(GTK_WIDGET(ui->modules->crop->crop_btn),
            TRUE);
        ui->modules->crop->is_active = 1;
    }
    else
    {
        gtk_button_set_label(ui->modules->crop->start_btn, "Start");
        gtk_widget_set_sensitive(GTK_WIDGET(ui->modules->crop->crop_btn),
            FALSE);
        ui->modules->crop->is_active = 0;
    }

    gtk_widget_queue_draw_area(GTK_WIDGET(ui->display->display_image),0,0,
        gtk_widget_get_allocated_width(
        GTK_WIDGET(ui->display->display_image)),
        gtk_widget_get_allocated_height(
        GTK_WIDGET(ui->display->display_image)));
}

void crop_btn(GtkWidget *w, gpointer user_data)
{
    (void) w;
    (void) user_data;
}