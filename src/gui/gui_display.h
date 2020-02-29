#ifndef GUI_DISPLAY_H
#define GUI_DISPLAY_H

// TODO : Coding style : 4.1 max 5 fcts
void reset_modules(struct UI *ui);

void display_images(struct UI *ui, char* filename);

void reset_image_info(struct UI *ui);

void reload_images(struct UI *ui);

gboolean on_image_event(GtkWidget *widget, GdkEvent *event,
               gpointer user_data);

#endif /* ! GUI_DISPLAY_H */
