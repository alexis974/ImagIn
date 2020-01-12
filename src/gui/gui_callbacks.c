#include "gui_callbacks.h"
#include "../import_export/import.h"

//Rotate module callback
void rotate_left(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("Rotate left button pressed !\n");
}
//Rotate module callback
void rotate_right(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("Rotate right button pressed !\n");
}

//Flip module callback
void flip_changed(GtkWidget *box, gpointer user_data)
{
    (void) user_data;
    int element_id = gtk_combo_box_get_active(GTK_COMBO_BOX(box));
    switch (element_id)
    {
    case 0:
        printf("Flip is now 'None' !\n");
        break;
    case 1:
        printf("Flip is now 'Vertical' !\n");
        break;
    case 2:
        printf("Flip is now 'Horizontal' !\n");
        break;
    case 3:
        printf("Flip is now 'Both' !\n");
        break;
    }
}

//Called when 'new file' is pressed
void new_menu(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("New button pressed !\n");
}

// Shows an about dialog window
void on_menubar_btn_about_activate(GtkWidget *widget, gpointer user_data)
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

    GdkPixbuf *pix_buffer =
        gdk_pixbuf_new_from_file_at_scale(
            filename,
            500,-1,TRUE,NULL
        );

    gtk_image_set_from_pixbuf(ui->display->display_image, pix_buffer);

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