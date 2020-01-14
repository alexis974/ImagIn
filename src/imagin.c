#include <stdio.h>
#include "imagin.h"
#include "gui/gui.h"
#include "error_handler.h"

int main(void)
{
    printf("GUI Mode\n");
    g_cli_mode = FALSE;
    return GUI_main();
}
