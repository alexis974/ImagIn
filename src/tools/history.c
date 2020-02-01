#include <stdio.h>
#include <stdlib.h>

#include "history.h"

void init_history(struct module_history *hist)
{
    hist->id = -1;
    hist->enable = 0;
    hist->value = 0;
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


void history_append(struct module_history *hist, int module_id,
        int enable, float value)
{
    struct module_history *new = malloc(sizeof(struct module_history));
    new->id = module_id;
    new->enable = enable;
    new->value = value;
    new->next = NULL;

    while (hist->next)
    {
        hist = hist->next;
    }

    hist->next = new;
}


void swap_module(struct module_history *elm1, struct module_history *elm2)
{
    int id = elm1->id;
    elm1->id = elm2->id;
    elm2->id = id;

    int enable = elm1->enable;
    elm1->enable = elm2->enable;
    elm2->enable = enable;

    float value = elm1->value;
    elm1->value = elm2->value;
    elm2->value = value;
}


// Sorting the history using the buublesort algorithm
void history_sort(struct module_history *hist)
{
    if (hist == NULL)
    {
        return;
    }

    struct module_history *copy = hist;

    int is_sorted = 0;
    while (!is_sorted)
    {
        is_sorted = 1;
        hist = copy;

        while(hist->next != NULL)
        {
            if(hist->id > hist->next->id)
            {
                swap_module(hist, hist->next);
                is_sorted = 0;
            }

            hist = hist->next;
        }
    }
}


void compress_history(struct module_history *hist)
{
    history_sort(hist);
    struct module_history *old = hist;

    while (hist->next != NULL)
    {

        if (hist->id == hist->next->id)
        {

            old->next = hist->next;
            free(hist);
        }

        old = hist;
        hist = hist->next;
    }
}


char *get_name(int id)
{
    char *module_name[] = {"Exposure", "Saturation", "Contraste", "Shadows",
        "Highlights", "Flip"};

    return id < 0 ? "NULL" : module_name[id];
}
