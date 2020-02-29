#ifndef GUI_EXPANDER_H
#define GUI_EXPANDER_H

struct Imagin_expander
{
    GtkEventBox *header;
    GtkToggleButton *check_box;
    GtkWidget *body;
    GtkWidget *right_panel;
    gboolean state; // 0: closed | 1: open

};

void set_expander_active(struct UI *ui, struct Imagin_expander *exp,
    int state);

void setup_imagin_expander(GtkBuilder *builder, struct Imagin_expander *exp,
    char* header_name, char* cb_name, char* body_name);

#endif /* ! GUI_EXPANDER_H */