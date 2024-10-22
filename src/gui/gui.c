#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "../imagin.h"

#include "gui.h"
#include "gui_builder.h"
#include "gui_signals.h"
#include "gui_modules.h"
#include "gui_display.h"
#include "gui_style.h"

#include "../tools/history/history.h"

int gui_main(void)
{
    gtk_init(NULL,NULL);

    struct UI *ui = build_gui("data/glade/gui.glade");

    if (!ui)
    {
        return 1;
    }

    ui->hist = hst_new();
    ui->compressed_hist = hst_new();

    css_setup("data/style/imagin_default.css");
    reset_modules(ui);
    reset_image_info(ui);

    gtk_widget_show(GTK_WIDGET(ui->window));

    connect_signals(ui);

    gtk_main();

    return 0;
}
