#include "gui_builder.h"

void build_modules_GUI(GtkBuilder *builder, struct UI *ui)
{
    ui->modules = malloc(sizeof(struct Modules));
    ui->modules->orientation = malloc(sizeof(struct Orientation));

    ui->modules->orientation->rot_l_button = GTK_BUTTON(gtk_builder_get_object(
				    builder, "rot_l_button"));
    ui->modules->orientation->rot_r_button = GTK_BUTTON(gtk_builder_get_object(
				    builder, "rot_r_button"));

    ui->modules->orientation->flip_h_button = GTK_BUTTON(gtk_builder_get_object(
				    builder, "flip_h_button"));
    ui->modules->orientation->flip_v_button = GTK_BUTTON(gtk_builder_get_object(
				    builder, "flip_v_button"));
}
