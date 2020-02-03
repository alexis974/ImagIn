#include <gtk/gtk.h>

#include "gui.h"
#include "gui_builder.h"

void build_menu_bar_GUI(GtkBuilder *builder, struct UI *ui)
{
    ui->menu_bar = malloc(sizeof(struct Menu_bar));
    ui->menu_bar->new_button = GTK_MENU_ITEM(gtk_builder_get_object(
                builder, "menu_new_button"));
    ui->menu_bar->open_button = GTK_MENU_ITEM(gtk_builder_get_object(
                builder, "menu_open_button"));
    ui->menu_bar->save_as_button = GTK_MENU_ITEM(gtk_builder_get_object(
                builder, "menu_save_as_button"));
    ui->menu_bar->close_button = GTK_MENU_ITEM(gtk_builder_get_object(
                builder, "menu_close_button"));
    ui->menu_bar->about_button = GTK_MENU_ITEM(gtk_builder_get_object(
                builder, "about_button"));
}

void build_bottom_bar_GUI(GtkBuilder *builder, struct UI *ui)
{
    ui->bottom_bar = malloc(sizeof(struct Bottom_bar));
    ui->bottom_bar->filename_label = GTK_LABEL(gtk_builder_get_object(
                builder, "image_filename"));
}

void build_display_GUI(GtkBuilder *builder, struct UI *ui)
{
    ui->display = malloc(sizeof(struct Display));
    ui->display->display_image = GTK_IMAGE(gtk_builder_get_object(
                builder, "display_image"));
    ui->display->small_image = GTK_IMAGE(gtk_builder_get_object(
                builder, "small_preview"));
    ui->display->histogram_area = GTK_DRAWING_AREA(gtk_builder_get_object(
                builder, "histogram_area"));
    ui->display->middle_area_events = GTK_EVENT_BOX(gtk_builder_get_object(
                builder, "middle_area_events"));
}

void build_image_info(GtkBuilder *builder, struct UI *ui)
{
    ui->image_info = malloc(sizeof(struct ImageInfo));
    ui->image_info->aperture = GTK_LABEL(gtk_builder_get_object(
                builder, "info_aperture"));
    ui->image_info->exposure_time = GTK_LABEL(gtk_builder_get_object(
                builder, "info_exposuretime"));
    ui->image_info->date = GTK_LABEL(gtk_builder_get_object(
                builder, "info_date"));
    ui->image_info->time = GTK_LABEL(gtk_builder_get_object(
                builder, "info_time"));
    ui->image_info->filename = GTK_LABEL(gtk_builder_get_object(
                builder, "info_filename"));
    ui->image_info->focal_length = GTK_LABEL(gtk_builder_get_object(
                builder, "info_focallength"));
    ui->image_info->iso = GTK_LABEL(gtk_builder_get_object(
                builder, "info_iso"));
    ui->image_info->shutter_speed = GTK_LABEL(gtk_builder_get_object(
                builder, "info_shutterspeed"));
}

void build_modules_GUI(GtkBuilder *builder, struct UI *ui)
{
    ui->modules = malloc(sizeof(struct Modules));
    ui->modules->orientation = malloc(sizeof(struct Orientation));

    ui->modules->orientation->rot_l_button = GTK_BUTTON(gtk_builder_get_object(
                builder, "rot_l_button"));
    ui->modules->orientation->rot_r_button = GTK_BUTTON(gtk_builder_get_object(
                builder, "rot_r_button"));

    ui->modules->orientation->flip_box = GTK_COMBO_BOX(gtk_builder_get_object(
                builder, "flip_box"));

    ui->modules->cont_exp_sat = malloc(sizeof(struct Cont_exp_sat));
    ui->modules->cont_exp_sat->contraste_scale = GTK_SCALE(
            gtk_builder_get_object(builder, "contraste_scale"));
    ui->modules->cont_exp_sat->exposure_scale = GTK_SCALE(
            gtk_builder_get_object(builder, "exposure_scale"));
    ui->modules->cont_exp_sat->saturation_scale = GTK_SCALE(
            gtk_builder_get_object(builder, "saturation_scale"));

    ui->modules->shadows_highlights = malloc(sizeof(struct Shadows_highlights));
    ui->modules->shadows_highlights->shadows_scale = GTK_SCALE(
            gtk_builder_get_object(builder, "shadows_scale"));
    ui->modules->shadows_highlights->highlights_scale = GTK_SCALE(
            gtk_builder_get_object(builder, "highlights_scale"));

    ui->modules->history_list = malloc(sizeof(struct History_List));
    ui->modules->history_list->list = GTK_LIST_BOX(
            gtk_builder_get_object(builder, "history_list"));
}

struct UI* build_GUI(char* glade_file_path)
{
    GtkBuilder *builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, glade_file_path, &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return NULL;
    }

    struct UI *ui = malloc(sizeof(struct UI));
    ui->window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    ui->image_loaded = FALSE;
    build_menu_bar_GUI(builder, ui);
    build_modules_GUI(builder, ui);
    build_display_GUI(builder, ui);
    build_bottom_bar_GUI(builder, ui);
    build_image_info(builder, ui);
    g_object_unref(builder);
    return ui;
}