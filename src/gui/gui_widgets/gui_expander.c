#include <gtk/gtk.h>

#include "gui_expander.h"

gboolean expander_pressed(GtkWidget *widget,
            GdkEvent *event, gpointer user_data)
{
    (void) widget;
    if (!(gdk_event_get_event_type(event) == GDK_BUTTON_RELEASE))
    {
        return FALSE;
    }

    struct Imagin_expander *expander = user_data;

    if (expander->state)
    {
        gtk_widget_hide (expander->body);
    }
    else
    {
        gtk_widget_show (expander->body);
    }

    expander->state = !expander->state;

    return FALSE;
}

void setup_imagin_expander(GtkBuilder *builder, struct Imagin_expander *exp,
    char* header_name, char* body_name)
{
    exp->header = GTK_EVENT_BOX(
            gtk_builder_get_object(builder, header_name));
    exp->body = GTK_WIDGET(
            gtk_builder_get_object(builder, body_name));
    exp->state = FALSE;
    gtk_widget_hide(exp->body);
    g_signal_connect(exp->header, "button-release-event", G_CALLBACK(expander_pressed), exp);
}