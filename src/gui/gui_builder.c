#include <gtk/gtk.h>


#include "gui_builder.h"
#include "gui_widgets/gui_expander.h"

#include "gui.h"
#include "gui_style.h"

void build_menu_bar_gui(GtkBuilder *builder, struct UI *ui)
{
    ui->menu_bar = malloc(sizeof(struct Menu_bar));
    ui->menu_bar->open_button = GTK_MENU_ITEM(gtk_builder_get_object(
                builder, "menu_open_button"));
    ui->menu_bar->export_as_button = GTK_MENU_ITEM(gtk_builder_get_object(
                builder, "menu_export_as_button"));
    ui->menu_bar->close_button = GTK_MENU_ITEM(gtk_builder_get_object(
                builder, "menu_close_button"));
    ui->menu_bar->about_button = GTK_MENU_ITEM(gtk_builder_get_object(
                builder, "about_button"));
    ui->menu_bar->undo_button = GTK_MENU_ITEM(gtk_builder_get_object(
                builder, "menu_undo_button"));

    GtkWidget *preferences_menu = GTK_WIDGET(gtk_builder_get_object(
                builder, "preferences_menu"));
    GDir *dir = g_dir_open("data/style", 0, NULL);
    const gchar *file = NULL;

    while ((file = g_dir_read_name(dir)) != NULL)
    {
        GtkWidget *item = gtk_menu_item_new_with_label(file);
        gtk_menu_shell_append(GTK_MENU_SHELL(preferences_menu), item);
        g_signal_connect(item, "activate", G_CALLBACK(switch_css), NULL);
        gtk_widget_show(item);
    }

    g_dir_close(dir);
}

void build_bottom_bar_gui(GtkBuilder *builder, struct UI *ui)
{
    ui->bottom_bar = malloc(sizeof(struct Bottom_bar));
    ui->bottom_bar->filename_label = GTK_LABEL(gtk_builder_get_object(
                builder, "image_filename"));
}

void build_display_gui(GtkBuilder *builder, struct UI *ui)
{
    ui->display = malloc(sizeof(struct Display));
    ui->display->display_image = GTK_DRAWING_AREA(gtk_builder_get_object(
                builder, "display_image"));
    ui->display->small_image = GTK_IMAGE(gtk_builder_get_object(
                builder, "small_preview"));
    ui->display->histogram_area = GTK_DRAWING_AREA(gtk_builder_get_object(
                builder, "histogram_area"));
    ui->display->middle_area_events = GTK_EVENT_BOX(gtk_builder_get_object(
                builder, "middle_area_events"));
    ui->display->box = GTK_WIDGET(gtk_builder_get_object(
                builder, "central_box"));

    //Setting the possibility to scroll on image
    gtk_widget_add_events(GTK_WIDGET(ui->display->middle_area_events),
        GDK_SCROLL_MASK);
    gtk_widget_add_events(GTK_WIDGET(ui->display->middle_area_events),
        GDK_POINTER_MOTION_MASK);
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

void build_custom_expanders(GtkBuilder *builder, struct UI *ui)
{
    ui->modules->bw_exp = malloc(sizeof(struct Imagin_expander));
    ui->modules->cont_exp_sat->exp = malloc(sizeof(struct Imagin_expander));
    ui->modules->invert_exp = malloc(sizeof(struct Imagin_expander));
    ui->modules->orientation->exp = malloc(sizeof(struct Imagin_expander));
    ui->modules->shadows_highlights->exp =
        malloc(sizeof(struct Imagin_expander));

    setup_imagin_expander(builder, ui->modules->bw_exp,
        "bw_event", "bw_cb", "bw_body");
    setup_imagin_expander(builder, ui->modules->cont_exp_sat->exp,
        "ces_event", "ces_cb", "ces_body");
    setup_imagin_expander(builder, ui->modules->invert_exp,
        "inv_event", "inv_cb", "inv_body");
    setup_imagin_expander(builder, ui->modules->orientation->exp,
        "ori_event", "ori_cb", "ori_body");
    setup_imagin_expander(builder, ui->modules->shadows_highlights->exp,
        "sh_event", "sh_cb", "sh_body");
}

void build_hue_gui(GtkBuilder *builder, struct UI *ui)
{
    ui->modules->hue = malloc(sizeof(struct Hue));
    ui->modules->hue->exp = malloc(sizeof(struct Imagin_expander));
    setup_imagin_expander(builder, ui->modules->hue->exp,
        "hue_event", "hue_cb", "hue_body");
}

void build_modules_gui(GtkBuilder *builder, struct UI *ui)
{
    ui->modules = malloc(sizeof(struct Modules));
    ui->modules->crop = malloc(sizeof(struct Crop));
    ui->modules->crop->selected_handle = -1;
    ui->modules->crop->is_active = 0;
    ui->modules->crop->start_btn = GTK_BUTTON(gtk_builder_get_object(
                builder, "start_crop_btn"));
    ui->modules->crop->crop_btn = GTK_BUTTON(gtk_builder_get_object(
                builder, "crop_btn"));

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

    ui->modules->shadows_highlights = malloc(sizeof(
        struct Shadows_highlights));
    ui->modules->shadows_highlights->shadows_scale = GTK_SCALE(
            gtk_builder_get_object(builder, "shadows_scale"));
    ui->modules->shadows_highlights->highlights_scale = GTK_SCALE(
            gtk_builder_get_object(builder, "highlights_scale"));

    ui->modules->history_list = malloc(sizeof(struct History_List));
    ui->modules->history_list->list = GTK_LIST_BOX(
            gtk_builder_get_object(builder, "history_list"));
    ui->modules->history_list->compress_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "compress_hist_button"));

    ui->modules->bw_switch  = GTK_SWITCH(
            gtk_builder_get_object(builder, "bw_switch"));
    ui->modules->invert_switch  = GTK_SWITCH(
            gtk_builder_get_object(builder, "invert_switch"));
}

void build_window_gui(GtkBuilder *builder, struct UI *ui)
{
    ui->window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));

    // Get monitor size
    GdkRectangle workarea = {0};
    gdk_monitor_get_workarea(gdk_display_get_primary_monitor(
                gdk_display_get_default()), &workarea);

    // Set to full size
    gtk_window_set_default_size(ui->window, workarea.width, workarea.height);
}

struct UI *build_gui(char *glade_file_path)
{
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;

    if (gtk_builder_add_from_file(builder, glade_file_path, &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return NULL;
    }

    struct UI *ui = malloc(sizeof(struct UI));
    ui->image_loaded = FALSE;
    ui->can_modify = TRUE;
    build_window_gui(builder, ui);
    build_menu_bar_gui(builder, ui);
    build_modules_gui(builder, ui);
    build_custom_expanders(builder, ui);
    build_hue_gui(builder, ui);
    build_display_gui(builder, ui);
    build_bottom_bar_gui(builder, ui);
    build_image_info(builder, ui);
    g_object_unref(builder);

    return ui;
}
