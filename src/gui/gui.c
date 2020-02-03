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
#include "gui_display.h"

void css_setup(void)
{
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "src/data/style/imagin_default.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(cssProvider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);
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
    reset_modules(ui);
    //Get monitor size
    GdkRectangle workarea = {0};
    gdk_monitor_get_workarea(gdk_display_get_primary_monitor(
                gdk_display_get_default()), &workarea);

    //Set to full size
    gtk_window_set_default_size(ui->window, workarea.width, workarea.height);

    //Setting default middle image
    gtk_image_set_from_file(ui->display->display_image,"src/data/icons/no_image.png");

    gtk_widget_show(GTK_WIDGET(ui->window));

    connect_signals(ui);

    gtk_main();
    return 0;
}
