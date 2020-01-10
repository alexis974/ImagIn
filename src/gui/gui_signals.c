#include "gui_signals.h"
#include "gui_callbacks.h"

void connect_modules(struct UI *ui)
{
    //Orientation modules signals
    g_signal_connect(ui->modules->orientation->rot_l_button, "clicked", G_CALLBACK(rotate_left), NULL);
    g_signal_connect(ui->modules->orientation->rot_r_button, "clicked", G_CALLBACK(rotate_right), NULL);
    g_signal_connect(ui->modules->orientation->flip_v_button, "clicked", G_CALLBACK(flip_ver), NULL);
    g_signal_connect(ui->modules->orientation->flip_h_button, "clicked", G_CALLBACK(flip_hor), NULL);
}

void connect_menu_bar(struct UI *ui)
{
    //File submenu signals
    g_signal_connect(ui->menu_bar->new_button, "activate", G_CALLBACK(new_menu), NULL);
    g_signal_connect(ui->menu_bar->open_button, "activate", G_CALLBACK(open_menu), NULL);
    g_signal_connect(ui->menu_bar->close_button, "activate", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(ui->menu_bar->about_button, "activate", G_CALLBACK(on_menubar_btn_about_activate), ui);
}

void connect_signals(struct UI *ui)
{
    g_signal_connect(ui->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    connect_modules(ui);
    connect_menu_bar(ui);
}
