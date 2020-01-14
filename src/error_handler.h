#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <gtk/gtk.h>

gboolean g_cli_mode;

void throw_error(char *source, char* message);

#endif