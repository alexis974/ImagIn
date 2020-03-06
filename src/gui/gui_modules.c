#include <gtk/gtk.h>

#include "../imagin.h"

#include "gui.h"
#include "gui_modules.h"
#include "gui_display.h"
#include "gui_history.h"
#include "gui_widgets/gui_expander.h"

#include "../modules/user/flip.h"
#include "../modules/user/contrast.h"
#include "../modules/user/shadows_highlights.h"
#include "../modules/user/exposure.h"
#include "../modules/user/saturation.h"

#include "../import_export/export.h"

#include "../debug/error_handler.h"

#include "../tools/history/history.h"
#include "../tools/history/image_handler.h"
#include "../tools/free.h"

// TODO : Coding style : 4.2 Max 10 fct
//##############################################################################

/*##############################################################################
** MODULES
*/

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