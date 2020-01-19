#ifndef GUI_CALLBACKS_H
#define GUI_CALLBACKS_H

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

void open_menu(GtkWidget *button, gpointer user_data);
void new_menu(GtkWidget *button, gpointer user_data);
void open_about_window(GtkWidget *widget, gpointer _ui);
void open_file_chooser(GtkWidget *widget, gpointer user_data);
void open_save_as_window(GtkWidget *widget, gpointer user_data);
void quit(GtkWidget *widget, gpointer user_data);

#endif
