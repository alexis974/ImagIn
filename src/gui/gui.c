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
#include "gui_style.h"

int GUI_main(void)
{
    gtk_init(NULL,NULL);

    struct UI *ui = build_GUI("src/gui/gui.glade");
    if (!ui)
        return 1;
    ui->hist = new_history();

    css_setup();
    reset_modules(ui);

    gtk_widget_show(GTK_WIDGET(ui->window));

    connect_signals(ui);

    gtk_main();
    return 0;
}
