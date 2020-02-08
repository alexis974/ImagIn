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
    GtkMenuItem *open_button;
    GtkMenuItem *export_as_button;
    GtkMenuItem *close_button;
    GtkMenuItem *about_button;
    GtkMenuItem *undo_button;
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

struct History_List
{
    GtkListBox *list;
};

struct Modules
{
    struct Orientation *orientation;
    struct Cont_exp_sat *cont_exp_sat;
    struct Shadows_highlights *shadows_highlights;
    struct History_List *history_list;
    GtkSwitch *bw_switch;
    GtkSwitch *invert_switch;
};

struct Display
{
    GtkImage *display_image;
    GtkImage *small_image;
    GtkDrawingArea *histogram_area;
    GtkEventBox *middle_area_events;
};

struct ImageInfo
{
    GtkLabel *date;
    GtkLabel *time;
    GtkLabel *exposure_time;
    GtkLabel *focal_length;
    GtkLabel *shutter_speed;
    GtkLabel *aperture;
    GtkLabel *filename;
    GtkLabel *iso;
};

struct Bottom_bar
{
    GtkLabel *filename_label;
};

struct UI
{
    GtkWindow *window;
    struct Menu_bar *menu_bar;
    struct Modules *modules;
    struct Display *display;
    struct Bottom_bar *bottom_bar;
    struct Images *images;
    struct ImageInfo *image_info;
    gboolean image_loaded;
    gboolean can_modify;
    struct history *hist;
};

int gui_main(void);

#endif /* ! UI_H */
