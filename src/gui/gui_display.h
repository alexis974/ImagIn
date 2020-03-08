#ifndef GUI_DISPLAY_H
#define GUI_DISPLAY_H

// TODO : Coding style : 4.1 max 5 fcts
//
//Max dimensions for scaled image
size_t g_maxheight;
size_t g_maxwidth;

//Max dimensions for small image
size_t g_maxheight_small;
size_t g_maxwidth_small;

void reset_image_info(struct UI *ui);

void reset_modules(struct UI *ui);

void on_center_image_size_change(GtkWidget *widget,
    GtkAllocation *allocation, void *data);

void reload_images(struct UI *ui);

void display_images(struct UI *ui, char* filename);

gboolean on_click_image(GtkWidget *widget, GdkEventButton *event,
        gpointer user_data);

gboolean on_scroll_image(GtkWidget *w, GdkEventScroll *event, gpointer data);

#endif /* ! GUI_DISPLAY_H */
