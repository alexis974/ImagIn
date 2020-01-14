#include "gui_callbacks.h"
#include "../import_export/import.h"
#include "../modules/flip.h"
#include "../modules/contrast.h"
#include "../modules/shadows_highlights.h"
#include "../modules/exposure.h"
#include "../modules/saturation.h"
#include <err.h>

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
        errx(1,"fill_image_with_buffer: image memory has not been allocated.");
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
    unsigned char *buffer = from_image_to_buffer(ui->displayed_image);

    GdkPixbuf *pix_buffer =
        gdk_pixbuf_new_from_data(buffer, GDK_COLORSPACE_RGB, FALSE, 8,
            ui->displayed_image->width, ui->displayed_image->height,
                ui->displayed_image->width * 3, free_buffer, NULL);
    gtk_image_set_from_pixbuf(ui->display->display_image, pix_buffer);

    //releasing memory
    g_object_unref(pix_buffer);
}

/*
**  MODULES
*/

//Rotate module callback
void rotate_left(GtkWidget *button, gpointer user_data)
{
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return;
    (void) button;
    printf("Rotate left button pressed !\n");
}
//Rotate module callback
void rotate_right(GtkWidget *button, gpointer user_data)
{
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return;
    (void) button;
    printf("Rotate right button pressed !\n");
}

//Flip module callback
void flip_changed(GtkComboBox *box, gpointer user_data)
{
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return;
    int element_id = gtk_combo_box_get_active(GTK_COMBO_BOX(box));
    switch (element_id)
    {
    case 0:
        printf("Flip is now 'None' !\n");
        break;
    case 1:
        vertical_flip(ui->displayed_image);
        break;
    case 2:
        printf("Flip is now 'Horizontal' !\n");
        horizontal_flip(ui->displayed_image);
        break;
    case 3:
        printf("Flip is now 'Both' !\n");
        flip_both_axis(ui->displayed_image);
        break;
    }
    reload_images(ui);
}

gboolean contraste_changed(GtkRange *range, GdkEvent* event, struct UI *user_data)
{
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return TRUE;
    printf("value : %f\n", gtk_range_get_value(range));
    (void) event; //Prevent unused warning
    return TRUE;
}
gboolean saturation_changed(GtkRange *range, GdkEvent* event, struct UI *user_data)
{
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return TRUE;
    saturation(ui->displayed_image, gtk_range_get_value(range));
    reload_images(ui);
    (void) event; //Prevent unused warning
    return TRUE;
}
gboolean exposure_changed(GtkRange *range, GdkEvent* event, struct UI *user_data)
{
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return TRUE;
    (void) event; //Prevent unused warning
    exposure(ui->displayed_image, gtk_range_get_value(range));
    reload_images(ui);
    return TRUE;
}
gboolean shadows_changed(GtkRange *range, GdkEvent* event, struct UI *user_data)
{
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return TRUE;
    (void) range;
    (void) event; //Prevent unused warning
    return TRUE;
}
gboolean highlights_changed(GtkRange *range, GdkEvent* event, struct UI *user_data)
{
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return TRUE;
    (void) range;
    (void) event; //Prevent unused warning
    return TRUE;
}

/*
**  WINDOWS & DIALOG
*/

//Called when 'new file' is pressed
void new_menu(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("New button pressed !\n");
}

// Shows an about dialog window
void open_about_window(GtkWidget *widget, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;
    GtkBuilder *builder = gtk_builder_new_from_file("src/gui/gui.glade");
    GtkWidget *window_about =
        GTK_WIDGET(gtk_builder_get_object(builder, "window_about"));
    gtk_builder_connect_signals(builder, NULL);
    gtk_window_set_transient_for(GTK_WINDOW(window_about), ui->window);
    gtk_widget_show(window_about);
    g_object_unref(builder);
}

//Called when user select a file in the file chooser menu
void file_selected(GtkWidget *widget, gpointer user_data)
{
    struct UI *ui = user_data;
    gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget));
    printf("Chosen file : %s\n", filename);
    gtk_window_close(GTK_WINDOW(widget));
    ui->displayed_image = read_image(filename);
    unsigned char *buffer = from_image_to_buffer(ui->displayed_image);
    GdkPixbuf *pix_buffer =
        gdk_pixbuf_new_from_data(buffer, GDK_COLORSPACE_RGB, FALSE, 8,
            ui->displayed_image->width, ui->displayed_image->height,
                ui->displayed_image->width * 3, free_buffer, NULL);
    gtk_image_set_from_pixbuf(ui->display->display_image, pix_buffer);
    ui->image_loaded = TRUE;
    //releasing memory
    g_object_unref(pix_buffer);
}

//Called to open the file chooser
void open_file_chooser(GtkWidget *widget, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;
    GtkBuilder *builder = gtk_builder_new_from_file("src/gui/gui.glade");
    GtkWidget *file_chooser =
        GTK_WIDGET(gtk_builder_get_object(builder, "open_file_chooser"));
    gtk_builder_connect_signals(builder, NULL);
    gtk_window_set_transient_for(GTK_WINDOW(file_chooser), ui->window);
    gtk_widget_show(file_chooser);
    g_signal_connect(file_chooser, "file_activated", G_CALLBACK(file_selected), ui);
    g_object_unref(builder);
}

void open_save_as_window(GtkWidget *widget, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;
    GtkWidget *dialog;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Save Result",
                                        GTK_WINDOW(ui->window),
                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                        "Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        "Save",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *path;
        path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        printf("%s\n", path);
        g_free(path);
    }

    gtk_widget_destroy(dialog);
}