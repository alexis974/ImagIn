#ifndef HISTORY_H
#define HISTORY_H

struct module_history
{
    int id;
    int enable;
    float value;
    struct module_history *next;
};

void init_history(struct module_history *hist);

int history_is_empty(struct module_history *hist);

size_t history_length(struct module_history *hist);

void history_append(struct module_history *hist, int module_id,
        int enable, float value);

void swap_module(struct module_history *elm1, struct module_history *elm2);

void history_sort(struct module_history *hist);

void compress_history(struct module_history *hist);

char *get_name(int id);

#endif
