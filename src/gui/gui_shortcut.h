#ifndef GUI_SHORTCUT_H
#define GUI_SHORTCUT_H

void undo(GtkWidget *widget, gpointer user_data);

gboolean on_key_press (GtkWidget *widget, GdkEventKey *event,
        gpointer user_data);

#endif /* ! GUI_SHORTCUT_H */