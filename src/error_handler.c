#include "error_handler.h"
#include <err.h>

void throw_error_in_UI(char *source, char *message)
{
    GtkBuilder *builder = gtk_builder_new_from_file("src/gui/gui.glade");
    GtkWidget *error_dialog =
        GTK_WIDGET(gtk_builder_get_object(builder, "error_dialog"));
    gtk_builder_connect_signals(builder, NULL);
    gtk_message_dialog_format_secondary_text(
        GTK_MESSAGE_DIALOG(error_dialog), "%s: %s", source, message
    );
    gtk_widget_show(error_dialog);
    g_object_unref(builder);
}

void throw_error_in_cli(char *source, char *message)
{
    errx(1, "%s : %s\n", source, message);
}

void throw_error(char *source, char* message)
{
    if (g_cli_mode)
        throw_error_in_cli(source, message);
    else
        throw_error_in_UI(source, message);
}