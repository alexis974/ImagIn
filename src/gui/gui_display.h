#ifndef GUI_DISPLAY_H
#define GUI_DISPLAY_H

void display_images(struct UI *ui, char* filename);
void reload_images(struct UI *ui);
gboolean draw_histogram(GtkWidget *widget, cairo_t *cr, gpointer user_data);

#endif