#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

struct UI
{
    GtkWindow *window;
};

void CssSetup(void)
{
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "src/gui/style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                GTK_STYLE_PROVIDER(cssProvider),
                                GTK_STYLE_PROVIDER_PRIORITY_USER);
}

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

    CssSetup();

	// Gets the widgets
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(
				builder, "main_window"));

    //Get monitor size
    GdkRectangle workarea = {0};
    gdk_monitor_get_workarea(
        gdk_display_get_primary_monitor(gdk_display_get_default()),
        &workarea);

    //Set to full size
    gtk_window_set_default_size(window, workarea.width, workarea.height);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show(GTK_WIDGET(window));

    gtk_main();
    return 0;
}
