#include <gtk/gtk.h>

#include "../imagin.h"

#include "gui.h"
#include "gui_modules.h"
#include "gui_display.h"

#include "../tools/free.h"
#include "../tools/history.h"

// Shows an about dialog window
void open_about_window(GtkWidget *widget, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;
    GtkBuilder *builder = gtk_builder_new_from_file("data/glade/gui.glade");
    GtkWidget *window_about =
        GTK_WIDGET(gtk_builder_get_object(builder, "window_about"));
    gtk_builder_connect_signals(builder, NULL);
    gtk_window_set_transient_for(GTK_WINDOW(window_about), ui->window);
    gtk_widget_show(window_about);
    g_object_unref(builder);
}

// Called to open the file chooser
void open_file_chooser(GtkWidget *widget, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    dialog = gtk_file_chooser_dialog_new ("Open File", ui->window, action,
            ("Cancel"), GTK_RESPONSE_CANCEL,
            ("Open"), GTK_RESPONSE_ACCEPT, NULL);

    gint res = gtk_dialog_run (GTK_DIALOG(dialog));

    if (res == GTK_RESPONSE_ACCEPT)
    {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        char *filename = gtk_file_chooser_get_filename(chooser);
        display_images(ui, filename);
        g_free (filename);
    }

    gtk_widget_destroy(dialog);
}

void open_export_as_window(GtkWidget *widget, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;

    if (!ui->image_loaded)
    {
        return;
    }

    GtkWidget *dialog = gtk_file_chooser_dialog_new("Save Result",
            GTK_WINDOW(ui->window),
            GTK_FILE_CHOOSER_ACTION_SAVE, "Cancel",
            GTK_RESPONSE_CANCEL, "Save",
            GTK_RESPONSE_ACCEPT, NULL);

    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog),
            TRUE);

    gint res = gtk_dialog_run(GTK_DIALOG(dialog));

    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *path = NULL;
        path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        export_at(ui, path);
        g_free(path);
    }

    gtk_widget_destroy(dialog);
}

// Called when window is closed
void quit(GtkWidget *widget, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;

    free(ui->modules->bw_exp);
    free(ui->modules->invert_exp);
    free(ui->modules->cont_exp_sat->exp);
    free(ui->modules->orientation->exp);
    free(ui->modules->shadows_highlights->exp);

    free(ui->image_info);

    free(ui->modules->history_list);
    free(ui->modules->shadows_highlights);
    free(ui->modules->orientation);
    free(ui->modules->cont_exp_sat);

    free(ui->bottom_bar);
    free(ui->display);
    free(ui->menu_bar);
    free(ui->modules);

    if (ui->image_loaded)
    {
        free_images(ui->images);
    }

    hst_free_recursively(ui->hist);
    hst_free_recursively(ui->compressed_hist);
    free(ui);
    gtk_main_quit();
}