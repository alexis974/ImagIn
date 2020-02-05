#include <gtk/gtk.h>

#include "../imagin.h"

#include "../modules/flip.h"
#include "../modules/contrast.h"
#include "../modules/shadows_highlights.h"
#include "../modules/exposure.h"
#include "../modules/saturation.h"

#include "../import_export/free.h"
#include "../import_export/export.h"

#include "../debug/error_handler.h"

#include "gui.h"
#include "gui_callbacks.h"
#include "gui_display.h"

#include "../tools/history.h"

/*
**  INPUT
*/

gboolean
on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;
    if(event->state & GDK_CONTROL_MASK && event->keyval == GDK_KEY_z
        && ui->image_loaded && !history_is_empty(ui->hist))
    {
        gtk_widget_destroy(GTK_WIDGET(gtk_list_box_get_row_at_index (
            ui->modules->history_list->list,0)));
        history_pop(ui->hist);
        reload_images(ui);
        reset_modules(ui);
        reset_widgets(ui->hist, ui);
    }
    else if(event->state & GDK_CONTROL_MASK && event->keyval == GDK_KEY_o)
    {
        gpointer tmp = ui;
        open_file_chooser(NULL, tmp);
    }
    return FALSE;
}

/*
**  MODULES
*/

void apply_module(struct UI *ui, int module_id, float value)
{
    GtkWidget *label = gtk_label_new(get_name(module_id));
    gtk_list_box_insert(ui->modules->history_list->list,label, 0);
    gtk_widget_show(label);
    history_append(ui->hist, module_id, 1,value);
    reload_images(ui);
}

//Rotate module callback
void rotate_left(GtkWidget *button, gpointer user_data)
{
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return;
    (void) button;
    printf("Rotate left button pressed !\n");
}

//Rotate module callback
void rotate_right(GtkWidget *button, gpointer user_data)
{
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return;
    (void) button;
    printf("Rotate right button pressed !\n");
    print_history(ui->hist);
}

//Flip module callback
void flip_changed(GtkComboBox *box, gpointer user_data)
{
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return;
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
    }
}

gboolean contraste_changed(GtkRange *range, GdkEvent *event, gpointer user_data)
{
    if(!(gdk_event_get_event_type(event) == GDK_BUTTON_RELEASE))
        return FALSE;
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return FALSE;
    apply_module(ui, CONTRASTE, gtk_range_get_value(range));
    return FALSE;
}

gboolean saturation_changed(GtkRange *range, GdkEvent *event,
        gpointer user_data)
{
    if(!(gdk_event_get_event_type(event) == GDK_BUTTON_RELEASE))
        return FALSE;
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return FALSE;
    apply_module(ui, SATURATION, gtk_range_get_value(range));
    return FALSE;
}

gboolean exposure_changed(GtkRange *range, GdkEvent *event, gpointer user_data)
{
    if(!(gdk_event_get_event_type(event) == GDK_BUTTON_RELEASE))
        return FALSE;
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return FALSE;
    (void) event; //Prevent unused warning
    apply_module(ui, EXPOSURE, gtk_range_get_value(range));
    return FALSE;
}

gboolean shadows_changed(GtkRange *range, GdkEvent *event, gpointer user_data)
{
    if(!(gdk_event_get_event_type(event) == GDK_BUTTON_RELEASE))
        return FALSE;
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return FALSE;
    apply_module(ui, SHADOWS, gtk_range_get_value(range));
    return FALSE;
}

gboolean highlights_changed(GtkRange *range, GdkEvent *event,
        gpointer user_data)
{
    if(!(gdk_event_get_event_type(event) == GDK_BUTTON_RELEASE))
        return FALSE;
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return FALSE;
    apply_module(ui, HIGHLIGHTS, gtk_range_get_value(range));
    return FALSE;
}

//Black and white
gboolean bw_changed(GtkSwitch *widget, gboolean state, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return FALSE;
    apply_module(ui, BW, state);
    return FALSE;
}

//Black and white
gboolean invert_changed(GtkSwitch *widget, gboolean state, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;
    //if no image has been opened
    if (!ui->image_loaded)
        return FALSE;
    apply_module(ui, INVERT, state);
    return FALSE;
}
/*
**  WINDOWS & DIALOG
*/

//Called when 'new file' is pressed
void new_menu(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("New button pressed !\n");
}

// Shows an about dialog window
void open_about_window(GtkWidget *widget, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;
    GtkBuilder *builder = gtk_builder_new_from_file("src/gui/gui.glade");
    GtkWidget *window_about =
        GTK_WIDGET(gtk_builder_get_object(builder, "window_about"));
    gtk_builder_connect_signals(builder, NULL);
    gtk_window_set_transient_for(GTK_WINDOW(window_about), ui->window);
    gtk_widget_show(window_about);
    g_object_unref(builder);
}

//Called to open the file chooser
void open_file_chooser(GtkWidget *widget, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    dialog = gtk_file_chooser_dialog_new ("Open File", ui->window, action,
            ("Cancel"), GTK_RESPONSE_CANCEL,
            ("Open"), GTK_RESPONSE_ACCEPT, NULL);

    gint res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        display_images(ui, filename);
        g_free (filename);
    }

    gtk_widget_destroy (dialog);
}

void export_at(struct UI *ui, char* filename)
{
    struct Image *exported = malloc(sizeof(struct Image));
    exported->width = ui->images->full->width;
    exported->height = ui->images->full->height;
    exported->bit_depth = ui->images->full->bit_depth;
    exported->data = malloc(sizeof(struct Pixel) *
        exported->width * exported->height);
    copy_img(ui->images->full, exported);
    apply_history(ui->hist, exported);
    write_image(filename, exported);
    free_image(exported);
}

void open_save_as_window(GtkWidget *widget, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;
    GtkWidget *dialog;

    dialog = gtk_file_chooser_dialog_new("Save Result", GTK_WINDOW(ui->window),
            GTK_FILE_CHOOSER_ACTION_SAVE, "Cancel",
            GTK_RESPONSE_CANCEL, "Save",
            GTK_RESPONSE_ACCEPT, NULL);

    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

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

//Called when window is closed
void quit(GtkWidget *widget, gpointer user_data)
{
    (void) widget;
    struct UI *ui = user_data;
    free(ui->bottom_bar);
    free(ui->display);
    free(ui->menu_bar);
    free(ui->modules);
    if (ui->image_loaded)
        free_images(ui->images);
    free_recursively(ui->hist);
    free(ui);
    gtk_main_quit();
}
