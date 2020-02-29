#ifndef GUI_HISTORY_H
#define GUI_HISTORY_H

void add_module_to_list(struct UI*ui, int module_id);

void compress_until_selected(struct UI *ui);

void compress_history(GtkWidget *button, gpointer user_data);

void hst_selection_changed(GtkListBox *box, GtkListBoxRow *row,
               gpointer user_data);

#endif /* ! GUI_HISTORY_H */