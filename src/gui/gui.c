#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "../imagin.h"

#include "../tools/history.h"

#include "gui.h"
#include "gui_builder.h"
#include "gui_signals.h"
#include "gui_callbacks.h"

void css_setup(void)
{
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "src/data/style/imagin_default.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(cssProvider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);
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
    g_object_unref(builder);
    return ui;
}

int GUI_main(void)
{
    gtk_init(NULL,NULL);

    struct UI *ui = build_GUI("src/gui/gui.glade");
    if (!ui)
        return 1;
    ui->hist = malloc(sizeof(struct history));
    init_history(ui->hist);

    css_setup();

    //Get monitor size
    GdkRectangle workarea = {0};
    gdk_monitor_get_workarea(gdk_display_get_primary_monitor(
                gdk_display_get_default()), &workarea);

    //Set to full size
    gtk_window_set_default_size(ui->window, workarea.width, workarea.height);

    //Setting default middle image
    gtk_image_set_from_file(ui->display->display_image,"src/data/no_image.png");

    gtk_widget_show(GTK_WIDGET(ui->window));

    connect_signals(ui);

    gtk_main();
    return 0;
}
