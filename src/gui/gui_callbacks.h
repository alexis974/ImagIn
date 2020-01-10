#ifndef GUI_CALLBACKS_H
#define GUI_CALLBACKS_H

#include "gui.h"

void rotate_left(GtkWidget *button, gpointer user_data);
void rotate_right(GtkWidget *button, gpointer user_data);
void flip_hor(GtkWidget *button, gpointer user_data);
void flip_ver(GtkWidget *button, gpointer user_data);
void open_menu(GtkWidget *button, gpointer user_data);
void new_menu(GtkWidget *button, gpointer user_data);

#endif