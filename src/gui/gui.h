#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>

struct Orientation
{
    GtkButton *rot_l_button;
    GtkButton *rot_r_button;
    GtkButton *flip_h_button;
    GtkButton *flip_v_button;
};

struct Menu_bar
{
    GtkButton *new_button;
    GtkButton *open_button;
    GtkButton *close_button;
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
