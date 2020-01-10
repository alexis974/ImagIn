#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>

struct Orientation
{
    GtkButton *rot_l_button;
    GtkButton *rot_r_button;
    GtkComboBox *flip_box;
};

struct Menu_bar
{
    GtkMenuItem *new_button;
    GtkMenuItem *open_button;
    GtkMenuItem *close_button;
    GtkMenuItem *about_button;
};

struct Modules
{
    struct Orientation *orientation;
};

struct UI
{
    GtkWindow *window;
    struct Menu_bar *menu_bar;
    struct Modules *modules;
};

int GUI_main(void);

#endif
