#include <gtk/gtk.h>
#include <time.h>
#include <stdlib.h>

#include "../imagin.h"

#include "../import_export/import.h"
#include "../import_export/export.h"
#include "../import_export/free.h"

#include "../debug/error_handler.h"

#include "gui.h"

void reset_modules(struct UI *ui)
{
    gtk_range_set_value(GTK_RANGE(
        ui->modules->cont_exp_sat->contraste_scale), 0);
    gtk_range_set_value(GTK_RANGE(
        ui->modules->cont_exp_sat->exposure_scale), 0);
    gtk_range_set_value(GTK_RANGE(
        ui->modules->cont_exp_sat->saturation_scale), 1);
    gtk_range_set_value(GTK_RANGE(
        ui->modules->shadows_highlights->shadows_scale), 0);
    gtk_range_set_value(GTK_RANGE(
        ui->modules->shadows_highlights->highlights_scale), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(
        ui->modules->orientation->flip_box), 0);
}

unsigned char *from_image_to_buffer(struct Image *img)
{
    unsigned char *buffer =
        malloc(sizeof(unsigned char) * img->width * img->height * 3);

    for (size_t j = 0; j < img->height; j++)
    {
        for (size_t i = 0; i < img->width; i++)
        {
            buffer[j*(img->width*3)+i*3] = img->data[j*img->width+i].red;
            buffer[j*(img->width*3)+i*3+1] = img->data[j*img->width+i].green;
            buffer[j*(img->width*3)+i*3+2] = img->data[j*img->width+i].blue;
        }
    }

    return buffer;
}

//Image data has to be allocated
void fill_image_data_with_buffer(unsigned char *buffer, struct Image *img)
{
    if (!img || !img->data)
    {
        throw_error("fill_image_with_buffer",
                "image memory has not been allocated.");
        return;
    }

    for (size_t j = 0; j < img->height; j++)
    {
        for (size_t i = 0; i < img->width; i++)
        {
            img->data[j*img->width+i].red = buffer[j*(img->width*3)+i*3];
            img->data[j*img->width+i].green = buffer[j*(img->width*3)+i*3+1];
            img->data[j*img->width+i].blue = buffer[j*(img->width*3)+i*3+2];
        }
    }
}

//Free pixel buffer when GdkBuffer is set
void free_buffer(guchar *pixels, gpointer data)
{
    (void) data;
    free(pixels);
}

void reload_images(struct UI *ui)
{
    if (!ui->image_loaded)
        return;

    //Middle image
    unsigned char *buffer = from_image_to_buffer(ui->images->edit);
    GdkPixbuf *pix_buffer =
        gdk_pixbuf_new_from_data(buffer, GDK_COLORSPACE_RGB, FALSE, 8,
            ui->images->edit->width, ui->images->edit->height,
                ui->images->edit->width * 3, free_buffer, NULL);
    gtk_image_set_from_pixbuf(ui->display->display_image, pix_buffer);

    //Small image
    unsigned char *buffer_small = from_image_to_buffer(ui->images->small);
    GdkPixbuf *pix_buffer_small =
        gdk_pixbuf_new_from_data(buffer_small, GDK_COLORSPACE_RGB, FALSE, 8,
            ui->images->small->width, ui->images->small->height,
                ui->images->small->width * 3, free_buffer, NULL);
    gtk_image_set_from_pixbuf(ui->display->small_image, pix_buffer_small);

    //releasing memory
    g_object_unref(pix_buffer);
    g_object_unref(pix_buffer_small);
}

void display_images(struct UI *ui, char* filename)
{
    //Free memory before reimporting
    if (ui->image_loaded)
    {
        free_images(ui->images);
    }

    reset_modules(ui);

    int padding = 10;
    //Setting middle zone info
    g_maxwidth = gtk_widget_get_allocated_width(
        GTK_WIDGET(ui->display->display_image)) - padding;
    g_maxheight = gtk_widget_get_allocated_height(
        GTK_WIDGET(ui->display->display_image)) - padding;

    //Setting preview zone info
    g_maxwidth_small = gtk_widget_get_allocated_width(
        GTK_WIDGET(ui->display->small_image)) - padding;
    g_maxheight_small = gtk_widget_get_allocated_height(
        GTK_WIDGET(ui->display->small_image)) - padding;

    //Getting all scaled images
    ui->images = read_image(filename);

    //Bottom bar
    gtk_label_set_text(ui->bottom_bar->filename_label, filename);

    ui->image_loaded = TRUE;

    reload_images(ui);

    gtk_widget_queue_draw_area(GTK_WIDGET(ui->display->histogram_area),
        0,0,300,150);
}

gboolean draw_histogram(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    struct UI *ui = user_data;
    if(!ui->image_loaded)
        return FALSE;

    (void) widget;
    int height =  gtk_widget_get_allocated_height(widget);
    int width = gtk_widget_get_allocated_width(widget);

    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    cairo_paint(cr);

    unsigned char red_hist[256] = {50};
    unsigned char green_hist[256] = {50};
    unsigned char blue_hist[256] = {50};
    srand(time(NULL));
    for (size_t i = 1; i < 256; i++)
    {
        red_hist[i] = (red_hist[i-1] + (rand() % 5) - (rand() % 5))%140;
        blue_hist[i] = (blue_hist[i-1] + (rand() % 5) - (rand() % 5))%140;
        green_hist[i] = (green_hist[i-1] + (rand() % 5) - (rand() % 5))%140;
    }

    cairo_set_source_rgb(cr, 1, 0, 0);
    cairo_set_line_width(cr,2);
    cairo_move_to(cr, 0, height-red_hist[0]);
    for (size_t i = 1; i < 256; i++)
    {
        cairo_line_to(cr, i*width/256, height-red_hist[i]);
    }
    cairo_stroke(cr);
    cairo_set_source_rgb(cr, 0, 1, 0);
    cairo_set_line_width(cr,2);
    cairo_move_to(cr, 0, height-green_hist[0]);
    for (size_t i = 1; i < 256; i++)
    {
        cairo_line_to(cr, i*width/256, height-green_hist[i]);
    }
    cairo_stroke(cr);
    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_set_line_width(cr,2);
    cairo_move_to(cr, 0, height-blue_hist[0]);
    for (size_t i = 1; i < 256; i++)
    {
        cairo_line_to(cr, i*width/256, height-blue_hist[i]);
    }
    cairo_stroke(cr);
    return FALSE;
}