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
** history_append does not respect the coding style. Must find a way to pass
** less than 5 paramters.
*/
void history_append(struct module_history *hist, char *module_name,
        int module_id, int enable, float value)
{
    // TODO
}


void history_delete(struct module_history hist, int module_id)
{
    // TODO
}


// Sorting the history using the insert sort algorithm
void history_sort(struct module_history *hist)
{
    // TODO
}


struct module_history *compress_history(struct module_history *hist)
{
    // TODO
    return hist;
}
