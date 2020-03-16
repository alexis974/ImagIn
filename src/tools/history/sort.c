#include <stdlib.h>

#include "history.h"

// Inserts or just changes the value in a sorted chained list
// In case of module already existing, old value is freed
// But new value has to be allocated
void hst_insert_sort(struct history *hist, int module_id,
        int enable, void *value)
{
    while (hist->next && module_id >= hist->next->id)
    {
        hist = hist->next;
    }

    if (hist->id == module_id)
    {
        free(hist->value);
        hist->value = value;
        hist->enable = enable;
        return;
    }

    struct history *new = malloc(sizeof(struct history));
    new->id = module_id;
    new->enable = enable;
    new->value = value;

    new->next = hist->next;
    hist->next = new;
}

void swap_module(struct history *elm1, struct history *elm2)
{
    int id = elm1->id;
    elm1->id = elm2->id;
    elm2->id = id;

    int enable = elm1->enable;
    elm1->enable = elm2->enable;
    elm2->enable = enable;

    void *value = elm1->value;
    elm1->value = elm2->value;
    elm2->value = value;
}


// Sorting the history using the bubblesort algorithm
void hst_sort(struct history *hist)
{
    if (hist == NULL)
    {
        return;
    }

    int is_sorted = 0;
    struct history *copy = hist;

    while (!is_sorted)
    {
        is_sorted = 1;
        hist = copy;

        while (hist->next != NULL)
        {
            if (hist->id > hist->next->id)
            {
                swap_module(hist, hist->next);
                is_sorted = 0;
            }

            hist = hist->next;
        }
    }
}