#include <stdio.h>
#include <gtk/gtk.h>

#include "gui/gui.h"
#include "debug/error_handler.h"
#include "imagin.h"

int main(void)
{
    printf("GUI Mode\n");
    g_cli_mode = FALSE;
    return GUI_main();
}
