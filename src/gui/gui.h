#ifndef UI_H
#define UI_H

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
    GtkMenuItem *save_as_button;
    GtkMenuItem *close_button;
    GtkMenuItem *about_button;
};

struct Cont_exp_sat
{
    GtkScale *contraste_scale;
    GtkScale *exposure_scale;
    GtkScale *saturation_scale;
};

struct Shadows_highlights
{
    GtkScale *shadows_scale;
    GtkScale *highlights_scale;
};

struct Modules
{
    struct Orientation *orientation;
    struct Cont_exp_sat *cont_exp_sat;
    struct Shadows_highlights *shadows_highlights;
};

struct Display
{
    GtkImage *display_image;
};

struct UI
{
    GtkWindow *window;
    struct Menu_bar *menu_bar;
    struct Modules *modules;
    struct Display *display;
    struct Images *images;
    gboolean image_loaded;
};

int GUI_main(void);

#endif
