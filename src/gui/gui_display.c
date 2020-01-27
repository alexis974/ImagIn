#include <gtk/gtk.h>

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
    unsigned char *buffer = from_image_to_buffer(ui->images->edit);

    GdkPixbuf *pix_buffer =
        gdk_pixbuf_new_from_data(buffer, GDK_COLORSPACE_RGB, FALSE, 8,
        ui->images->edit->width, ui->images->edit->height,
        ui->images->edit->width * 3, free_buffer, NULL);
    gtk_image_set_from_pixbuf(ui->display->display_image, pix_buffer);

    //releasing memory
    g_object_unref(pix_buffer);
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

    //Bottom bar
    gtk_label_set_text(ui->bottom_bar->filename_label, filename);

    ui->image_loaded = TRUE;

    //releasing memory
    g_object_unref(pix_buffer);
    g_object_unref(pix_buffer_small);
}
