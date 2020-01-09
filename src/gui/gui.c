#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gui_builder.h"

void css_setup(void)
{
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "src/gui/style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                GTK_STYLE_PROVIDER(cssProvider),
                                GTK_STYLE_PROVIDER_PRIORITY_USER);
}

struct UI* build_GUI(char* glade_file_path)
{
    GtkBuilder *builder = gtk_builder_new();
    GError* error = NULL;
	if (gtk_builder_add_from_file(builder, glade_file_path, &error) == 0)
	{
		g_printerr("Error loading file: %s\n", error->message);
		g_clear_error(&error);
		return NULL;
    }

    struct UI *ui = malloc(sizeof(struct UI));
    ui->window = GTK_WINDOW(gtk_builder_get_object(
				    builder, "main_window"));
    build_modules_GUI(builder, ui);
    return ui;
}

void rotate_left(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("Rotate left button pressed !\n");
}

void rotate_right(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("Rotate right button pressed !\n");
}

void flip_hor(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("Flip horizontal button pressed !\n");
}
void flip_ver(GtkWidget *button, gpointer user_data)
{
    (void) button;
    (void) user_data;
    printf("Flip vertical button pressed !\n");
}

int GUI_main(void)
{
    gtk_init(NULL,NULL);

    struct UI *ui = build_GUI("src/gui/gui.glade");
    if (!ui)
        return 1;

    css_setup();

    //Get monitor size
    GdkRectangle workarea = {0};
    gdk_monitor_get_workarea(
        gdk_display_get_primary_monitor(gdk_display_get_default()),
        &workarea);

    //Set to full size
    gtk_window_set_default_size(ui->window, workarea.width, workarea.height);
    gtk_window_set_title(ui->window, "ImagIn");//Set title

    g_signal_connect(ui->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(ui->modules->orientation->rot_l_button, "clicked", G_CALLBACK(rotate_left), NULL);
    g_signal_connect(ui->modules->orientation->rot_r_button, "clicked", G_CALLBACK(rotate_right), NULL);
    g_signal_connect(ui->modules->orientation->flip_v_button, "clicked", G_CALLBACK(flip_ver), NULL);
    g_signal_connect(ui->modules->orientation->flip_h_button, "clicked", G_CALLBACK(flip_hor), NULL);

    gtk_widget_show(GTK_WIDGET(ui->window));

    gtk_main();
    return 0;
}
