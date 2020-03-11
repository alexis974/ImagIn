#ifndef GUI_CROP_H
#define GUI_CROP_H

void draw_crop_rectangle(struct UI *ui, cairo_t *cr);

void set_crop_handles_coordinates(struct UI *ui);

void crop_motion_event(GdkEventMotion *event, struct UI *ui);

#endif /* ! GUI_CROP_H */