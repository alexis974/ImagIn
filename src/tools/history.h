#ifndef HISTORY_H
#define HISTORY_H

struct module_history
{
    char *name;
    int local_id;
    int enable;
    float value;
    struct module_history *next;
};

void init_history(struct module_history *hist);

int history_is_empty(struct module_history *hist);

size_t history_length(struct module_history *hist);

void history_append(struct module_history *hist, char *module_name,
        int module_id, int enable, float value);

#endif
