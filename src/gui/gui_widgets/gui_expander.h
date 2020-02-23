#ifndef GUI_EXPANDER_H
#define GUI_EXPANDER_H

struct Imagin_expander
{
    gboolean state; // 0: closed | 1: open
    GtkEventBox *header;
    GtkWidget *body;
};

void setup_imagin_expander(GtkBuilder *builder, struct Imagin_expander *exp,
    char* header_name, char* body_name);

#endif /* ! GUI_EXPANDER_H */