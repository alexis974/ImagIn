#ifndef GUI_DISPLAY_H
#define GUI_DISPLAY_H

// TODO : Coding style : 4.1 max 5 fcts

void on_center_image_size_change(GtkWidget *widget,
    GtkAllocation *allocation, void *data);

void reset_modules(struct UI *ui);

void display_images(struct UI *ui, char* filename);

void reset_image_info(struct UI *ui);

void reload_images(struct UI *ui);

gboolean on_click_image(GtkWidget *widget, GdkEvent *event,
               gpointer user_data);

gboolean on_scroll_image(GtkWidget *w, GdkEvent *event, gpointer data);

gboolean motion_image(GtkWidget *w, GdkEventMotion *event, gpointer user_data);

gboolean draw_image(GtkWidget *w, cairo_t *cr, gpointer user_data);

#endif /* ! GUI_DISPLAY_H */
