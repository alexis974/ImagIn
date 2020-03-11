#include <gtk/gtk.h>

#include "gui.h"
#include "gui_modules.h"
#include "gui_display.h"
#include "gui_signals.h"
#include "gui_windows.h"
#include "gui_shortcut.h"
#include "gui_history.h"
#include "gui_histogram.h"

#include "gui_widgets/gui_expander.h"

void connect_check_boxes(struct UI *ui)
{
    g_signal_connect(ui->modules->bw_exp->check_box, "toggled",
            G_CALLBACK(bw_cb_toggled), ui);
    g_signal_connect(ui->modules->invert_exp->check_box, "toggled",
            G_CALLBACK(inv_cb_toggled), ui);
    g_signal_connect(ui->modules->orientation->exp->check_box, "toggled",
            G_CALLBACK(ori_cb_toggled), ui);
    g_signal_connect(ui->modules->cont_exp_sat->exp->check_box, "toggled",
            G_CALLBACK(ces_cb_toggled), ui);
    g_signal_connect(ui->modules->shadows_highlights->exp->check_box,
            "toggled", G_CALLBACK(sh_cb_toggled), ui);
}

void connect_modules(struct UI *ui)
{
    // Orientation modules signals
    g_signal_connect(ui->modules->orientation->rot_l_button, "clicked",
            G_CALLBACK(rotate_left), ui);
    g_signal_connect(ui->modules->orientation->rot_r_button, "clicked",
            G_CALLBACK(rotate_right), ui);
    g_signal_connect(ui->modules->orientation->flip_box, "changed",
            G_CALLBACK(flip_changed), ui);

    // Contraste Exposure Saturation signals
    g_signal_connect(ui->modules->cont_exp_sat->contraste_scale, "event",
            G_CALLBACK(contraste_changed), ui);
    g_signal_connect(ui->modules->cont_exp_sat->exposure_scale, "event",
            G_CALLBACK(exposure_changed), ui);
    g_signal_connect(ui->modules->cont_exp_sat->saturation_scale,
            "event", G_CALLBACK(saturation_changed), ui);

    // Shadow Highlights signals
    g_signal_connect(ui->modules->shadows_highlights->shadows_scale, "event",
            G_CALLBACK(shadows_changed), ui);
    g_signal_connect(ui->modules->shadows_highlights->highlights_scale, "event",
            G_CALLBACK(highlights_changed), ui);

    g_signal_connect(ui->modules->bw_switch, "state_set",
            G_CALLBACK(bw_changed), ui);
    g_signal_connect(ui->modules->invert_switch, "state_set",
            G_CALLBACK(invert_changed), ui);
}

void connect_menu_bar(struct UI *ui)
{
    // File submenu signals
    g_signal_connect(ui->menu_bar->open_button, "activate",
            G_CALLBACK(open_file_chooser), ui);
    g_signal_connect(ui->menu_bar->export_as_button, "activate",
            G_CALLBACK(open_export_as_window), ui);
    g_signal_connect(ui->menu_bar->about_button, "activate",
            G_CALLBACK(open_about_window), ui);
    g_signal_connect(ui->menu_bar->close_button, "activate",
            G_CALLBACK(quit), ui);
    g_signal_connect(ui->menu_bar->undo_button, "activate",
            G_CALLBACK(undo), ui);
}

void connect_display(struct UI *ui)
{
    g_signal_connect(ui->display->histogram_area, "draw",
            G_CALLBACK(draw_histogram), ui);
    g_signal_connect(ui->display->middle_area_events, "button-press-event",
            G_CALLBACK(on_click_image), ui);
    g_signal_connect(ui->display->middle_area_events, "scroll-event",
            G_CALLBACK(on_scroll_image), ui);
    g_signal_connect(ui->display->middle_area_events, "motion-notify-event",
            G_CALLBACK(motion_image), ui);
    g_signal_connect(ui->modules->history_list->list, "row-selected",
            G_CALLBACK(hst_selection_changed), ui);
    g_signal_connect(ui->modules->history_list->compress_button, "clicked",
            G_CALLBACK(compress_history_btn), ui);
    g_signal_connect(ui->display->box, "size-allocate",
            G_CALLBACK(on_center_image_size_change), ui);
    g_signal_connect(ui->display->display_image, "draw",
            G_CALLBACK(draw_image), ui);
}

void connect_signals(struct UI *ui)
{
    g_signal_connect(ui->window, "destroy", G_CALLBACK(quit), ui);
    g_signal_connect(ui->window, "key_press_event",
            G_CALLBACK (on_key_press), ui);
    connect_modules(ui);
    connect_check_boxes(ui);
    connect_menu_bar(ui);
    connect_display(ui);
}
