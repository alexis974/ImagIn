#ifndef GUI_CROP_H
#define GUI_CROP_H

void draw_crop_rectangle(struct UI *ui, cairo_t *cr);

void set_crop_handles_coordinates(struct UI *ui);

void crop_on_click(GdkEventButton *event, struct UI *ui);

void crop_motion_event(GdkEventMotion *event, struct UI *ui);

void start_btn(GtkWidget *w, gpointer user_data);

void crop_btn(GtkWidget *w, gpointer user_data);

#endif /* ! GUI_CROP_H */