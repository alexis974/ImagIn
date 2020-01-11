#ifndef GUI_CALLBACKS_H
#define GUI_CALLBACKS_H

#include "gui.h"

void rotate_left(GtkWidget *button, gpointer user_data);
void rotate_right(GtkWidget *button, gpointer user_data);
void flip_hor(GtkWidget *button, gpointer user_data);
void flip_changed(GtkWidget *button, gpointer user_data);
void open_menu(GtkWidget *button, gpointer user_data);
void new_menu(GtkWidget *button, gpointer user_data);
void on_menubar_btn_about_activate(GtkWidget *widget, gpointer _ui);
void open_file_chooser(GtkWidget *widget, gpointer user_data);

#endif
