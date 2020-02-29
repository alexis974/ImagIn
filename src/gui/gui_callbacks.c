#include <gtk/gtk.h>

#include "../imagin.h"

#include "gui.h"
#include "gui_callbacks.h"
#include "gui_display.h"
#include "gui_widgets/gui_expander.h"

#include "../modules/user/flip.h"
#include "../modules/user/contrast.h"
#include "../modules/user/shadows_highlights.h"
#include "../modules/user/exposure.h"
#include "../modules/user/saturation.h"

#include "../import_export/export.h"

#include "../debug/error_handler.h"

#include "../tools/free.h"
#include "../tools/history.h"

// TODO : Coding style : 4.2 Max 10 fct
/*##############################################################################
** INPUT
*/

void undo(GtkWidget *widget, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;
    if (!ui->image_loaded || hst_is_empty(ui->hist))
    {
        return;
    }

    int delete = hst_pop(ui->hist);
    if (delete)
        gtk_widget_destroy(GTK_WIDGET(gtk_list_box_get_row_at_index(
            ui->modules->history_list->list,0)));

    //Update compressed hist
    hst_free_recursively(ui->compressed_hist);
    ui->compressed_hist = hst_duplicate(ui->hist);
    hst_compress(ui->compressed_hist);

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


/*##############################################################################
** MODULES
*/

void add_module_to_list(struct UI*ui, int module_id)
{
    GtkBuilder *builder = gtk_builder_new();
    GError* error = NULL;

    if (gtk_builder_add_from_file(builder, "data/glade/gui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    GtkWidget *list_elm = GTK_WIDGET(
            gtk_builder_get_object(builder, "hist_elm"));

    GList *children = gtk_container_get_children(GTK_CONTAINER(list_elm));
    for (GList *l = children; l != NULL; l = l->next)
    {
        if (strcmp(gtk_widget_get_name(l->data), "hist_index") == 0)
        {
            char index[3] = {0};
            sprintf(index, "%zu", hst_compressed_length(ui->hist));
            gtk_label_set_text(GTK_LABEL(l->data), index);
        }
        else
        {
            gtk_label_set_text(GTK_LABEL(l->data), get_name(module_id));
        }
    }

    gtk_list_box_insert(ui->modules->history_list->list,list_elm, 0);
    gtk_widget_show(list_elm);

    g_object_unref(builder);
}

// Delete changes above selected history element
void compress_until_selected(struct UI *ui)
{
    if (!ui->image_loaded || ui->hist->next ==  NULL)
        return;

    if (gtk_list_box_get_selected_row(ui->modules->history_list->list))
    {
        hst_compress(ui->hist);

        int index = hst_length(ui->hist) - (gtk_list_box_row_get_index(
            gtk_list_box_get_selected_row(ui->modules->history_list->list)) + 1);

        for (size_t i = 0; i < hst_length(ui->hist) - (index + 1); i++)
        {
            gtk_widget_destroy(GTK_WIDGET(gtk_list_box_get_row_at_index(
                ui->modules->history_list->list,0)));
        }

        hst_truncate(ui->hist, index + 1);

        hst_free_recursively(ui->compressed_hist);
        ui->compressed_hist = hst_duplicate(ui->hist);
        hst_sort(ui->compressed_hist);
        hst_compress(ui->compressed_hist);
    }
}

void apply_module(struct UI *ui, int module_id, float value)
{
    if (!ui->can_modify)
    {
        return;
    }

    // If we are back in history and we make a change
    // We delete the rest of history
    compress_until_selected(ui);

    gtk_list_box_unselect_all(ui->modules->history_list->list);

    int add = hst_append(ui->hist, module_id, 1, value);
    hst_insert_sort(ui->compressed_hist, module_id, 1, value);
    if (add)
        add_module_to_list(ui, module_id);

    reload_images(ui);
}

// "Compress history" button callback
void compress_history(GtkWidget *button, gpointer user_data)
{
    (void) button;
    struct UI *ui = user_data;
    compress_until_selected(ui);

    reset_modules(ui);
    reset_widgets(ui->compressed_hist, ui);
    reload_images(ui);
}

void hst_selection_changed(GtkListBox *box, GtkListBoxRow *row,
               gpointer user_data)
{
    (void) box;
    struct UI *ui = user_data;
    if (!ui->image_loaded || !row)
    {
        return;
    }

    struct history *tmp_hist = hst_duplicate(ui->hist);

    //First compression to match with displayed history
    hst_compress(tmp_hist);

    int index = hst_length(tmp_hist) - (gtk_list_box_row_get_index(
            gtk_list_box_get_selected_row(ui->modules->history_list->list)) + 1);

    hst_truncate(tmp_hist, index + 1);

    // Second compression to apply
    hst_sort(tmp_hist);
    hst_compress(tmp_hist);

    // We have to inject it to modules to make reload functions believe
    // it is the right history
    struct history *tmp = ui->compressed_hist;

    ui->compressed_hist = tmp_hist;
    reset_modules(ui);
    reset_widgets(tmp_hist, ui);
    reload_images(ui);

    //Freeing tmp_hist
    hst_free_recursively(ui->compressed_hist);
    ui->compressed_hist = tmp;
}


// Rotate module callback
void rotate_left(GtkWidget *button, gpointer user_data)
{
    struct UI *ui = user_data;

    // If no image has been opened
    if (!ui->image_loaded)
    {
        return;
    }

    (void) button;
    printf("Rotate left button pressed !\n");

    set_expander_active(ui, ui->modules->orientation->exp, TRUE);
}

// Rotate module callback
void rotate_right(GtkWidget *button, gpointer user_data)
{
    struct UI *ui = user_data;

    // If no image has been opened
    if (!ui->image_loaded)
    {
        return;
    }

    (void) button;
    printf("Rotate right button pressed !\n");
    set_expander_active(ui, ui->modules->orientation->exp, TRUE);
}

// Flip module callback
void flip_changed(GtkComboBox *box, gpointer user_data)
{
    struct UI *ui = user_data;
    // If no image has been opened
    if (!ui->image_loaded)
    {
        return;
    }

    int element_id = gtk_combo_box_get_active(GTK_COMBO_BOX(box));

    switch (element_id)
    {
    case 0:
        apply_module(ui, FLIP, 0);
        break;
    case 1:
        apply_module(ui, FLIP, 1);
        break;
    case 2:
        apply_module(ui, FLIP, 2);
        break;
    case 3:
        apply_module(ui, FLIP, 3);
        break;
    default:
        break;
    }

    set_expander_active(ui, ui->modules->orientation->exp, TRUE);
}

gboolean contraste_changed(GtkRange *range, GdkEvent *event, gpointer user_data)
{
    if (!(gdk_event_get_event_type(event) == GDK_BUTTON_RELEASE))
    {
        return FALSE;
    }

    struct UI *ui = user_data;

    // If no image has been opened
    if (!ui->image_loaded)
    {
        return FALSE;
    }

    apply_module(ui, CONTRASTE, gtk_range_get_value(range));

    set_expander_active(ui, ui->modules->cont_exp_sat->exp, TRUE);

    return FALSE;
}

gboolean saturation_changed(GtkRange *range, GdkEvent *event,
        gpointer user_data)
{
    if (!(gdk_event_get_event_type(event) == GDK_BUTTON_RELEASE))
    {
        return FALSE;
    }

    struct UI *ui = user_data;

    // If no image has been opened
    if (!ui->image_loaded)
    {
        return FALSE;
    }

    apply_module(ui, SATURATION, gtk_range_get_value(range));

    set_expander_active(ui, ui->modules->cont_exp_sat->exp, TRUE);

    return FALSE;
}

gboolean exposure_changed(GtkRange *range, GdkEvent *event, gpointer user_data)
{
    if (!(gdk_event_get_event_type(event) == GDK_BUTTON_RELEASE))
    {
        return FALSE;
    }

    struct UI *ui = user_data;

    // If no image has been opened
    if (!ui->image_loaded)
    {
        return FALSE;
    }

    apply_module(ui, EXPOSURE, gtk_range_get_value(range));

    set_expander_active(ui, ui->modules->cont_exp_sat->exp, TRUE);

    return FALSE;
}

gboolean shadows_changed(GtkRange *range, GdkEvent *event, gpointer user_data)
{
    if (!(gdk_event_get_event_type(event) == GDK_BUTTON_RELEASE))
    {
        return FALSE;
    }

    struct UI *ui = user_data;

    // If no image has been opened
    if (!ui->image_loaded)
    {
        return FALSE;
    }

    apply_module(ui, SHADOWS, gtk_range_get_value(range));

    set_expander_active(ui, ui->modules->shadows_highlights->exp, TRUE);

    return FALSE;
}

gboolean highlights_changed(GtkRange *range, GdkEvent *event,
        gpointer user_data)
{
    if (!(gdk_event_get_event_type(event) == GDK_BUTTON_RELEASE))
    {
        return FALSE;
    }

    struct UI *ui = user_data;

    // If no image has been opened
    if (!ui->image_loaded)
    {
        return FALSE;
    }

    apply_module(ui, HIGHLIGHTS, gtk_range_get_value(range));

    set_expander_active(ui, ui->modules->shadows_highlights->exp, TRUE);

    return FALSE;
}

// Black and white
gboolean bw_changed(GtkSwitch *widget, gboolean state, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;

    // If no image has been opened
    if (!ui->image_loaded)
    {
        return FALSE;
    }

    apply_module(ui, BW, state);

    set_expander_active(ui, ui->modules->bw_exp, TRUE);

    return FALSE;
}

// Invert
gboolean invert_changed(GtkSwitch *widget, gboolean state, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;

    // If no image has been opened
    if (!ui->image_loaded)
    {
        return FALSE;
    }

    apply_module(ui, INVERT, state);

    set_expander_active(ui, ui->modules->invert_exp, TRUE);

    return FALSE;
}


/*##############################################################################
** WINDOWS & DIALOG
*/

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

void export_at(struct UI *ui, char *filename)
{
    struct Image *exported = malloc(sizeof(struct Image));

    exported->width = ui->images->full->width;
    exported->height = ui->images->full->height;
    exported->bit_depth = ui->images->full->bit_depth;
    exported->data = malloc(sizeof(struct Pixel) *
        exported->width * exported->height);

    copy_img(ui->images->full, exported);
    hst_apply_all(ui->compressed_hist, exported);
    write_image(filename, exported);
    free_image(exported);
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


/*##############################################################################
** Modules check button
*/

void toggle_module(struct UI *ui, int module_id, int state)
{
    if (!ui->image_loaded || !ui->can_modify)
        return;

    // We have to inject it to modules to make reload functions believe
    // it is the right history
    compress_until_selected(ui);

    gtk_list_box_unselect_all(ui->modules->history_list->list);

    hst_enable_last(ui->hist, module_id, state);
    hst_enable_last(ui->compressed_hist, module_id, state);

    reload_images(ui);
}

void bw_cb_toggled(GtkToggleButton *togglebutton,
                            gpointer user_data)
{
    struct UI* ui = user_data;
    int state =  gtk_toggle_button_get_active(togglebutton);
    toggle_module(ui, BW, state);
}

void inv_cb_toggled(GtkToggleButton *togglebutton,
                            gpointer user_data)
{
    struct UI* ui = user_data;
    int state =  gtk_toggle_button_get_active(togglebutton);
    toggle_module(ui, INVERT, state);
}

void ori_cb_toggled(GtkToggleButton *togglebutton,
                            gpointer user_data)
{
    struct UI* ui = user_data;
    int state =  gtk_toggle_button_get_active(togglebutton);
    toggle_module(ui, FLIP, state);
    toggle_module(ui, ROTATION, state);
}

void ces_cb_toggled(GtkToggleButton *togglebutton,
                            gpointer user_data)
{
    struct UI* ui = user_data;
    int state =  gtk_toggle_button_get_active(togglebutton);
    toggle_module(ui, CONTRASTE, state);
    toggle_module(ui, EXPOSURE, state);
    toggle_module(ui, SATURATION, state);
}

void sh_cb_toggled(GtkToggleButton *togglebutton,
                            gpointer user_data)
{
    struct UI* ui = user_data;
    int state =  gtk_toggle_button_get_active(togglebutton);
    toggle_module(ui, SHADOWS, state);
    toggle_module(ui, HIGHLIGHTS, state);
}