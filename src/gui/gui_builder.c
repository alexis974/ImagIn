#include "gui_builder.h"

void build_menu_bar_GUI(GtkBuilder *builder, struct UI *ui)
{
    ui->menu_bar = malloc(sizeof(struct Menu_bar));
    ui->menu_bar->new_button = GTK_MENU_ITEM(gtk_builder_get_object(
                builder, "menu_new_button"));
    ui->menu_bar->open_button = GTK_MENU_ITEM(gtk_builder_get_object(
                builder, "menu_open_button"));
    ui->menu_bar->close_button = GTK_MENU_ITEM(gtk_builder_get_object(
                builder, "menu_close_button"));
    ui->menu_bar->about_button = GTK_MENU_ITEM(gtk_builder_get_object(
                builder, "about_button"));
}

void build_modules_GUI(GtkBuilder *builder, struct UI *ui)
{
    ui->modules = malloc(sizeof(struct Modules));
    ui->modules->orientation = malloc(sizeof(struct Orientation));

    ui->modules->orientation->rot_l_button = GTK_BUTTON(gtk_builder_get_object(
                builder, "rot_l_button"));
    ui->modules->orientation->rot_r_button = GTK_BUTTON(gtk_builder_get_object(
                builder, "rot_r_button"));

    ui->modules->orientation->flip_box = GTK_COMBO_BOX(gtk_builder_get_object(
                builder, "flip_box"));
}
