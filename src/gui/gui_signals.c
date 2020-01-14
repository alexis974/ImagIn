#include "gui_signals.h"
#include "gui_callbacks.h"

void connect_modules(struct UI *ui)
{
    //Orientation modules signals
    g_signal_connect(ui->modules->orientation->rot_l_button, "clicked", G_CALLBACK(rotate_left), ui);
    g_signal_connect(ui->modules->orientation->rot_r_button, "clicked", G_CALLBACK(rotate_right), ui);
    g_signal_connect(ui->modules->orientation->flip_box, "changed", G_CALLBACK(flip_changed), ui);

    //Contraste Exposure Saturation signals
    g_signal_connect(ui->modules->cont_exp_sat->contraste_scale, "event", G_CALLBACK(contraste_changed), ui);
    g_signal_connect(ui->modules->cont_exp_sat->exposure_scale, "event", G_CALLBACK(exposure_changed), ui);
    g_signal_connect(ui->modules->cont_exp_sat->saturation_scale, "event", G_CALLBACK(saturation_changed), ui);
    //Shadow Highlights signals
    g_signal_connect(ui->modules->shadows_highlights->shadows_scale, "event", G_CALLBACK(shadows_changed), ui);
    g_signal_connect(ui->modules->shadows_highlights->shadows_scale, "event", G_CALLBACK(highlights_changed), ui);
}

void connect_menu_bar(struct UI *ui)
{
    //File submenu signals
    g_signal_connect(ui->menu_bar->new_button, "activate", G_CALLBACK(new_menu), NULL);
    g_signal_connect(ui->menu_bar->open_button, "activate", G_CALLBACK(open_file_chooser), ui);
    g_signal_connect(ui->menu_bar->save_as_button, "activate", G_CALLBACK(open_save_as_window), ui);
    g_signal_connect(ui->menu_bar->about_button, "activate", G_CALLBACK(open_about_window), ui);
    g_signal_connect(ui->menu_bar->close_button, "activate", G_CALLBACK(gtk_main_quit), NULL);
}

void connect_signals(struct UI *ui)
{
    g_signal_connect(ui->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    connect_modules(ui);
    connect_menu_bar(ui);
}
