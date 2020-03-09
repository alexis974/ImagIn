#ifndef XML_H
#define XML_H

#define MY_ENCODING "UTF-8"

#include <gtk/gtk.h>

#include "../../imagin.h"
#include "../../gui/gui.h"
#include "../../tools/history/history.h"

void create_xmp(const char *uri);

void save_hist(struct history *hist, const char *uri);

struct history *load_hist(const char *uri);

#endif /* ! XML_H */
