#ifndef HISTORY_H
#define HISTORY_H

struct module_history
{
    char *name;
    int id;
    int enable;
    float value;
    struct module_history *next;
};

#endif
