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