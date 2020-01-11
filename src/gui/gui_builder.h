#ifndef GUI_BUILDER_H
#define GUI_BUILDER_H

#include "gui.h"
#include <gtk/gtk.h>

void build_menu_bar_GUI(GtkBuilder *builder, struct UI *ui);
void build_modules_GUI(GtkBuilder *builder, struct UI *ui);
void build_display_GUI(GtkBuilder *builder, struct UI *ui);

#endif
