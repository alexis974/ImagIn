#ifndef GUI_CALLBACKS_H
#define GUI_CALLBACKS_H

// TODO : Coding style : 4.1 max 5 fcts
void undo(GtkWidget *widget, gpointer user_data);

gboolean on_key_press (GtkWidget *widget, GdkEventKey *event,
        gpointer user_data);

void compress_history(GtkWidget *button, gpointer user_data);

void hst_selection_changed(GtkListBox *box, GtkListBoxRow *row,
        gpointer user_data);

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

void open_menu(GtkWidget *button, gpointer user_data);

void open_about_window(GtkWidget *widget, gpointer _ui);

void open_file_chooser(GtkWidget *widget, gpointer user_data);

void open_export_as_window(GtkWidget *widget, gpointer user_data);

void quit(GtkWidget *widget, gpointer user_data);

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

#endif /* ! GUI_CALLBACKS_H */
