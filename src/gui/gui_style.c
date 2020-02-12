#include <gtk/gtk.h>
#include <string.h>

void css_setup(char *filename)
{
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, filename, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(css_provider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void switch_css(GtkWidget *button, gpointer user_data)
{
    (void) user_data;
    const gchar *filename = gtk_menu_item_get_label(GTK_MENU_ITEM(button));
    char path[512]  = "data/style/";
    strcat(path, filename);
    css_setup(path);
}
