#include "gui_callbacks.h"

void rotate_left(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("Rotate left button pressed !\n");
}

void rotate_right(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("Rotate right button pressed !\n");
}

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

void new_menu(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("New button pressed !\n");
}

// Shows an about dialog window (import it from the glade file each time)
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

void file_selected(GtkWidget *widget, gpointer user_data)
{
    (void) user_data;
    printf("%s\n", gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget)));
    gtk_window_close(GTK_WINDOW(widget));
}

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