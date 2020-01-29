#include <stdio.h>

#include "history.h"

void init_history(struct module_history *hist)
{
    hist->id = -1;
    hist->next = NULL;
}


int history_is_empty(struct module_history *hist)
{
    return (!hist->next) ?  1 : 0;
}


size_t history_length(struct module_history *hist)
{
    size_t counter = 0;
    while (hist->next)
    {
        hist = hist->next;
        counter++;
    }

    return counter;
}


/*
** history_add does not respect the coding style. Must find a way to pass less
** than 5 paramters.
*/
void history_add(struct module_history *hist, char *module_name, int module_id,
        int enable, float value)
{
    // TODO
}


void history_delete(struct module_history hist, int module_id)
{
    // TODO
}


struct module_history *compress_history(struct module_history *hist)
{
    // TODO
    return hist;
}
