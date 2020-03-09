#ifndef XML_H
#define XML_H

#define MY_ENCODING "UTF-8"

#include <gtk/gtk.h>

#include "../../imagin.h"
#include "../../gui/gui.h"
#include "../../tools/history/history.h"

void create_xmp(const char *uri);

void save_hist_xml(struct history *hist, const char *uri);

struct history *get_hist_from_xml(const char *uri);

void test_save_hist_xml(const char *uri, const char *uri2);

#endif /* ! XML_H */
