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

void flip_hor(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("Flip horizontal button pressed !\n");
}
void flip_ver(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("Flip vertical button pressed !\n");
}

void open_menu(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("Open button pressed !\n");
}

void new_menu(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("New button pressed !\n");
}

// Shows an about dialog window (import it from the glade file each time)
void on_menubar_btn_about_activate(GtkWidget *widget, gpointer _ui)
{
    (void) widget;
    struct UI *ui =_ui;
    GtkBuilder *builder = gtk_builder_new_from_file("src/gui/gui.glade");
    GtkWidget *window_about =
        GTK_WIDGET(gtk_builder_get_object(builder, "window_about"));
    gtk_builder_connect_signals(builder, NULL);
    gtk_window_set_transient_for(GTK_WINDOW(window_about), ui->window);
    gtk_widget_show(window_about);
    g_object_unref(builder);
}
