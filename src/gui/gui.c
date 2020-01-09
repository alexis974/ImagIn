#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

struct UI
{
    GtkWindow *window;
};

int GUImain(void)
{
    gtk_init(NULL,NULL);
    GtkBuilder *builder = gtk_builder_new();

    GError* error = NULL;
	if (gtk_builder_add_from_file(builder, "src/gui/gui.glade", &error) == 0)
	{
		g_printerr("Error loading file: %s\n", error->message);
		g_clear_error(&error);
		return 1;
	}

	// Gets the widgets
	GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(
				builder, "main_window"));

    /*struct UI ui=
    {
        .window = window,
    };*/
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show(GTK_WIDGET(window));

    gtk_main();
    return 0;
}
