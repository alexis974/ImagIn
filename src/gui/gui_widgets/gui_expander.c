#include <gtk/gtk.h>

#include "../gui.h"
#include "gui_expander.h"

// Force set expander check box
void set_expander_active(struct UI *ui, struct Imagin_expander *exp, int state)
{
    //Prevents signal from calling toggle btns callback
    gboolean tmp = ui->can_modify;
    ui->can_modify = FALSE;
    gtk_toggle_button_set_active(exp->check_box, state);
    ui->can_modify = tmp;
}

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

    gtk_widget_queue_draw(expander->right_panel);

    expander->state = !expander->state;

    return FALSE;
}

void setup_imagin_expander(GtkBuilder *builder, struct Imagin_expander *exp,
    char* header_name, char* cb_name, char* body_name)
{
    exp->header = GTK_EVENT_BOX(
            gtk_builder_get_object(builder, header_name));
    exp->check_box = GTK_TOGGLE_BUTTON(
            gtk_builder_get_object(builder, cb_name));
    exp->body = GTK_WIDGET(
            gtk_builder_get_object(builder, body_name));
    exp->state = FALSE;
    gtk_widget_hide(exp->body);

    /* QUICK FIX */
    exp->right_panel = GTK_WIDGET(
            gtk_builder_get_object(builder, "right_panel"));

    g_signal_connect(exp->header, "button-release-event", G_CALLBACK(expander_pressed), exp);
}