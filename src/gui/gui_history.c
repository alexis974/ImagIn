#include <gtk/gtk.h>

#include "../imagin.h"

#include "gui.h"
#include "gui_display.h"

#include "gui_widgets/gui_expander.h"

#include "../tools/history/history.h"
#include "../tools/history/truncate.h"
#include "../tools/history/compress.h"
#include "../tools/history/sort.h"
#include "../tools/free.h"

// TODO : Coding style : 4.10  Fct max 25 lines
void reset_widgets(struct history *hist, struct UI *ui)
{
    ui->can_modify = FALSE;
    struct history *compressed = hst_duplicate(hist);
    hst_compress(compressed);
    for (struct history *p = compressed->next; p != NULL; p=p->next)
    {
        switch (p->id)
        {
        case CONTRASTE:
        {
            gdouble *value = p->value;
            gtk_range_set_value(GTK_RANGE(
                    ui->modules->cont_exp_sat->contraste_scale), *value);
            gtk_toggle_button_set_active(
                ui->modules->cont_exp_sat->exp->check_box, p->enable);
            break;
        }
        case EXPOSURE:
        {
            gdouble *value = p->value;
            gtk_range_set_value(GTK_RANGE(
                    ui->modules->cont_exp_sat->exposure_scale), *value);
            gtk_toggle_button_set_active(
                ui->modules->cont_exp_sat->exp->check_box, p->enable);
            break;
        }
        case SATURATION:
        {
            gdouble *value = p->value;
            gtk_range_set_value(GTK_RANGE(
                    ui->modules->cont_exp_sat->saturation_scale), *value);
            gtk_toggle_button_set_active(
                ui->modules->cont_exp_sat->exp->check_box, p->enable);
            break;
        }
        case SHADOWS:
        {
            gdouble *value = p->value;
            gtk_range_set_value(GTK_RANGE(
                ui->modules->shadows_highlights->shadows_scale), *value);
            gtk_toggle_button_set_active(
                ui->modules->shadows_highlights->exp->check_box, p->enable);
            break;
        }
        case HIGHLIGHTS:
        {
            gdouble *value = p->value;
            gtk_range_set_value(GTK_RANGE(
                ui->modules->shadows_highlights->highlights_scale), *value);
            gtk_toggle_button_set_active(
                ui->modules->shadows_highlights->exp->check_box, p->enable);
            break;
        }
        case FLIP:
        {
            int *value = p->value;
            gtk_combo_box_set_active(GTK_COMBO_BOX(
                    ui->modules->orientation->flip_box), *value);
            gtk_toggle_button_set_active(
                ui->modules->orientation->exp->check_box, p->enable);
            break;
        }
        case BW:
        {
            int *value = p->value;
            gtk_switch_set_state(ui->modules->bw_switch, *value);
            gtk_toggle_button_set_active(
                ui->modules->bw_exp->check_box, p->enable);
            break;
        }
        case INVERT:
        {
            int *value = p->value;
            gtk_switch_set_state(ui->modules->invert_switch, *value);
            gtk_toggle_button_set_active(
                ui->modules->invert_exp->check_box, p->enable);
            break;
        }
        default:
            break;
        }
    }
    ui->can_modify = TRUE;
    hst_free_recursively(compressed);
}

void add_module_to_list(struct UI *ui, char *name)
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
        // Set index
        if (strcmp(gtk_widget_get_name(l->data), "hist_index") == 0)
        {
            char index[3] = {0};
            sprintf(index, "%zu", hst_compressed_length(ui->hist));
            gtk_label_set_text(GTK_LABEL(l->data), index);
        }
        else // Set module name
        {
            gtk_label_set_text(GTK_LABEL(l->data), name);
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
        size_t nb_elements = g_list_length(gtk_container_get_children(
            GTK_CONTAINER(ui->modules->history_list->list)));

        int index = nb_elements - (gtk_list_box_row_get_index(
            gtk_list_box_get_selected_row(ui->modules->history_list->list)) + 1);

        for (size_t i = 0; i < nb_elements - (index + 1); i++)
        {
            gtk_widget_destroy(GTK_WIDGET(gtk_list_box_get_row_at_index(
                ui->modules->history_list->list,0)));
        }

        hst_truncate_uncompressed(ui->hist, index);

        hst_free_recursively(ui->compressed_hist);
        ui->compressed_hist = hst_duplicate(ui->hist);
        hst_sort(ui->compressed_hist);
        hst_compress(ui->compressed_hist);
    }
}

// "Compress history" button callback
void compress_history_btn(GtkWidget *button, gpointer user_data)
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

void reset_history_list(struct UI *ui)
{
    GList *children = gtk_container_get_children(
            GTK_CONTAINER(ui->modules->history_list->list));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter))
    {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }

    g_list_free(children);

    add_module_to_list(ui, "Original");
}