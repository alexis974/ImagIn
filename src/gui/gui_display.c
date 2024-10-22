#include <gtk/gtk.h>

#include "../imagin.h"

#include "gui.h"
#include "gui_modules.h"
#include "gui_windows.h"
#include "gui_history.h"
#include "gui_widgets/gui_expander.h"

#include "../import_export/import.h"
#include "../import_export/export.h"

#include "../tools/history/history.h"
#include "../tools/history/image_handler.h"
#include "../tools/history/truncate.h"
#include "../tools/history/apply.h"

#include "../tools/strings.h"
#include "../tools/free.h"
#include "../tools/exif.h"

#include "../debug/error_handler.h"

size_t g_maxheight;
size_t g_maxwidth;
size_t g_maxwidth_small;
size_t g_maxheight_small;

void reset_image_info(struct UI *ui)
{
    gtk_label_set_text(ui->image_info->aperture, "-");
    gtk_label_set_text(ui->image_info->date, "-");
    gtk_label_set_text(ui->image_info->time, "-");
    gtk_label_set_text(ui->image_info->exposure_time, "-");
    gtk_label_set_text(ui->image_info->filename, "-");
    gtk_label_set_text(ui->image_info->focal_length, "-");
    gtk_label_set_text(ui->image_info->iso, "-");
    gtk_label_set_text(ui->image_info->shutter_speed, "-");
}

void reset_modules(struct UI *ui)
{
    ui->can_modify = FALSE;

    gtk_range_set_value(GTK_RANGE(
                ui->modules->cont_exp_sat->contraste_scale), 0);
    gtk_range_set_value(GTK_RANGE(
                ui->modules->cont_exp_sat->exposure_scale), 0);
    gtk_range_set_value(GTK_RANGE(
                ui->modules->cont_exp_sat->saturation_scale), 0);
    gtk_range_set_value(GTK_RANGE(
                ui->modules->shadows_highlights->shadows_scale), 0);
    gtk_range_set_value(GTK_RANGE(
                ui->modules->shadows_highlights->highlights_scale), 0);

    gtk_toggle_button_set_active(
                ui->modules->orientation->exp->check_box, TRUE);
    gtk_toggle_button_set_active(
                ui->modules->cont_exp_sat->exp->check_box, TRUE);
    gtk_toggle_button_set_active(
                ui->modules->shadows_highlights->exp->check_box, TRUE);
    gtk_toggle_button_set_active(
                ui->modules->bw_exp->check_box, TRUE);
    gtk_toggle_button_set_active(
                ui->modules->invert_exp->check_box, TRUE);

    gtk_combo_box_set_active(GTK_COMBO_BOX(
                ui->modules->orientation->flip_box), 0);

    gtk_switch_set_state (ui->modules->bw_switch, FALSE);
    gtk_switch_set_state (ui->modules->invert_switch, FALSE);
    ui->can_modify = TRUE;
}

void on_center_image_size_change(GtkWidget *widget, GtkAllocation *allocation, void *data)
{
    (void) widget;
    (void) data;
    int padding = 10;
    g_maxheight = allocation->height - padding;
    g_maxwidth = allocation->width - padding;
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

// Free pixel buffer when GdkBuffer is set
// Requested by GTK when loading images with gdk_pixbuf_new_from_data
void free_buffer(guchar *pixels, gpointer data)
{
    (void) data;
    free(pixels);
}

/*
**  Called whenever we want to refresh images
*/
void reload_images(struct UI *ui)
{
    if (!ui->image_loaded)
    {
        return;
    }

    copy_img(ui->images->scale, ui->images->edit);
    hst_apply_all(ui->compressed_hist, ui->images->edit);
    ui->images->small = get_small(ui->images->edit);

    // Middle image
    unsigned char *buffer = from_image_to_buffer(ui->images->edit);
    GdkPixbuf *pix_buffer =
        gdk_pixbuf_new_from_data(buffer, GDK_COLORSPACE_RGB, FALSE, 8,
                ui->images->edit->width, ui->images->edit->height,
                ui->images->edit->width * 3, free_buffer, NULL);
    gtk_image_set_from_pixbuf(ui->display->display_image, pix_buffer);

    // Small image
    unsigned char *buffer_small = from_image_to_buffer(ui->images->small);
    GdkPixbuf *pix_buffer_small =
        gdk_pixbuf_new_from_data(buffer_small, GDK_COLORSPACE_RGB, FALSE, 8,
                ui->images->small->width, ui->images->small->height,
                ui->images->small->width * 3, free_buffer, NULL);
    gtk_image_set_from_pixbuf(ui->display->small_image, pix_buffer_small);

    gtk_widget_queue_draw_area(GTK_WIDGET(ui->display->histogram_area), 0, 0,
        gtk_widget_get_allocated_width(
        GTK_WIDGET(ui->display->histogram_area)),
        gtk_widget_get_allocated_height(
        GTK_WIDGET(ui->display->histogram_area)));

    // Releasing memory
    g_object_unref(pix_buffer);
    g_object_unref(pix_buffer_small);
}

// TODO : Coding style : 4.10 Fct max 25 lines
/* Called the first time we import an image */
void display_images(struct UI *ui, char *filename)
{
    // Free memory before reimporting
    if (ui->image_loaded)
    {
        free_images(ui->images);
        hst_truncate(ui->hist, 0); // Reset histories
        hst_truncate(ui->compressed_hist, 0);
    }

    /*
    ** It is necessary to set image_loaded as false until this function
    ** is not finished
    */
    ui->image_loaded = FALSE;
    reset_history_list(ui);
    reset_image_info(ui);
    reset_modules(ui);

    int padding = 10;
    // Setting preview zone info
    g_maxwidth_small = gtk_widget_get_allocated_width(
            GTK_WIDGET(ui->display->small_image)) - padding;
    g_maxheight_small = gtk_widget_get_allocated_height(
            GTK_WIDGET(ui->display->small_image)) - padding;
    // Getting all scaled images
    ui->images = read_image(filename);
    if (!ui->images)
    {
        return;
    }

    // Image info
    const char *file_name = get_filename_from_path(filename);
    gtk_label_set_text(ui->image_info->filename, file_name);
    const char *ext = get_filename_ext(filename);

    if (strcmp(ext, "jpeg") == 0 || strcmp(ext, "jpg") == 0
            || strcmp(ext, "JPEG") == 0 || strcmp(ext, "JPG") == 0)
    {
        set_image_info(filename, ui);
    }

    // Bottom bar
    gtk_label_set_text(ui->bottom_bar->filename_label, filename);

    ui->image_loaded = TRUE;

    reload_images(ui);

    gtk_widget_queue_draw_area(GTK_WIDGET(ui->display->histogram_area), 0, 0,
        gtk_widget_get_allocated_width(
        GTK_WIDGET(ui->display->histogram_area)),
        gtk_widget_get_allocated_height(
        GTK_WIDGET(ui->display->histogram_area)));
}

gboolean on_click_image(GtkWidget *widget, GdkEventButton *event,
        gpointer user_data)
{
    (void) widget;
    (void) event;
    struct UI *ui = user_data;
    if (!ui->image_loaded)
    {
        open_file_chooser(NULL, user_data);
        return FALSE;
    }
    printf("Image pressed on %f,%f\n", event->x, event->y);
    return FALSE;
}

gboolean on_scroll_image(GtkWidget *w, GdkEventScroll *event, gpointer data)
{
    (void) w;
    struct UI *ui = data;
    if (!ui->image_loaded)
    {
        return FALSE;
    }

    (void) event;
    printf("Scroll direction : %d Scroll delta :  %f\n",
        event->direction, event->x_root);

    return FALSE;
}
