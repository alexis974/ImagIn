#ifndef UI_H
#define UI_H

struct Coordinates
{
    size_t x;
    size_t y;
};

struct Crop
{
    /* There are four handles we register their positions and which one is
    selected */
    int selected_handle;
    struct Coordinates handles[4];
    int is_active;
    GtkButton *start_btn;
    GtkButton *crop_btn;
};

struct Mouse
{
    struct Coordinates last_position;
};

struct Orientation
{
    struct Imagin_expander *exp;
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
    struct Imagin_expander *exp;
    GtkScale *contraste_scale;
    GtkScale *exposure_scale;
    GtkScale *saturation_scale;
};

struct Shadows_highlights
{
    struct Imagin_expander *exp;
    GtkScale *shadows_scale;
    GtkScale *highlights_scale;
};

struct Hue
{
    struct Imagin_expander *exp;
};

struct History_List
{
    GtkListBox *list;
    GtkButton *compress_button;
};

struct Modules
{
    struct Orientation *orientation;
    struct Cont_exp_sat *cont_exp_sat;
    struct Shadows_highlights *shadows_highlights;
    struct History_List *history_list;
    struct Hue *hue;
    struct Crop *crop;
    GtkSwitch *bw_switch;
    GtkSwitch *invert_switch;
    struct Imagin_expander *bw_exp;
    struct Imagin_expander *invert_exp;
};

struct Display
{
    GtkWidget *box;
    GtkDrawingArea *display_image;
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
    struct history *hist;
    struct history *compressed_hist;
    struct Mouse *mouse;
    gboolean image_loaded;
    gboolean can_modify;
};

int gui_main(void);

#endif /* ! UI_H */
