#ifndef GUI_MODULES_H
#define GUI_MODULES_H

void export_at(struct UI *ui, char *filename);

void rotate_left(GtkWidget *button, gpointer user_data);

void rotate_right(GtkWidget *button, gpointer user_data);

void flip_changed(GtkComboBox *button, gpointer user_data);

gboolean contraste_changed(GtkRange *range, GdkEvent *event,
        gpointer user_data);

gboolean saturation_changed(GtkRange *range, GdkEvent *event,
        gpointer user_data);

gboolean exposure_changed(GtkRange *range, GdkEvent *event,
        gpointer user_data);

gboolean shadows_changed(GtkRange *range, GdkEvent *event,
        gpointer user_data);

gboolean highlights_changed(GtkRange *range, GdkEvent *event,
        gpointer user_data);

gboolean bw_changed(GtkSwitch *widget, gboolean state, gpointer user_data);

gboolean invert_changed(GtkSwitch *widget, gboolean state, gpointer user_data);

void bw_cb_toggled(GtkToggleButton *togglebutton,
                            gpointer user_data);

void inv_cb_toggled(GtkToggleButton *togglebutton,
                            gpointer user_data);

void ori_cb_toggled(GtkToggleButton *togglebutton,
                            gpointer user_data);

void sh_cb_toggled(GtkToggleButton *togglebutton,
                            gpointer user_data);

void ces_cb_toggled(GtkToggleButton *togglebutton,
                            gpointer user_data);

#endif /* ! GUI_MODULES_H */
