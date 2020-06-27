#include <stdio.h>
#include <gtk/gtk.h>

#include "gui/gui.h"

#include "debug/error_handler.h"

#include "imagin.h"

// TODO : Coding style : 2.5 typedef ?
gboolean g_cli_mode = FALSE;

int main(void)
{
    printf("Launching GUI...\n");

    return gui_main();
}
