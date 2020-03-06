#ifndef GUI_HISTORY_H
#define GUI_HISTORY_H

void add_module_to_list(struct UI*ui, char *name);

void compress_until_selected(struct UI *ui);

void compress_history_btn(GtkWidget *button, gpointer user_data);

void hst_selection_changed(GtkListBox *box, GtkListBoxRow *row,
               gpointer user_data);

void reset_widgets(struct history *hist, struct UI *ui);

void reset_history_list(struct UI *ui);

#endif /* ! GUI_HISTORY_H */