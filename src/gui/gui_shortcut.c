#include <gtk/gtk.h>

#include "../imagin.h"

#include "gui.h"
#include "gui_windows.h"
#include "gui_display.h"
#include "gui_history.h"

#include "../tools/history/history.h"
#include "../tools/free.h"

void undo(GtkWidget *widget, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;
    if (!ui->image_loaded || hst_length(ui->hist) == 0)
    {
        return;
    }

    gtk_list_box_unselect_all(ui->modules->history_list->list);

    int delete = hst_pop(ui->hist);
    if (delete)
        gtk_widget_destroy(GTK_WIDGET(gtk_list_box_get_row_at_index(
            ui->modules->history_list->list,0)));

    //Update compressed hist
    hst_free_recursively(ui->compressed_hist);
    ui->compressed_hist = hst_duplicate(ui->hist);
    hst_compress(ui->compressed_hist);
    hst_sort(ui->compressed_hist);

    reset_modules(ui);
    reset_widgets(ui->compressed_hist, ui);
    reload_images(ui);
}

gboolean on_key_press (GtkWidget *widget, GdkEventKey *event,
        gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;

    if (event->state & GDK_CONTROL_MASK && event->keyval == GDK_KEY_z)
    {
        gpointer tmp = ui;
        undo(NULL, tmp);
    }

    else if (event->state & GDK_CONTROL_MASK && event->keyval == GDK_KEY_o)
    {
        gpointer tmp = ui;
        open_file_chooser(NULL, tmp);
    }

    else if (event->state & GDK_CONTROL_MASK && event->keyval == GDK_KEY_e)
    {
        gpointer tmp = ui;
        open_export_as_window(NULL, tmp);
    }

    return FALSE;
}