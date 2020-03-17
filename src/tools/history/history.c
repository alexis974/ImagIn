#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history.h"

struct history *hst_new(void)
{
    struct history *hist = malloc(sizeof(struct history));

    hist->id = -1;
    hist->enable = 0;
    hist->value = NULL;
    hist->next = NULL;

    return hist;
}

// Does not count sentinel
size_t hst_length(struct history *hist)
{
    size_t counter = 0;

    while (hist->next)
    {
        hist = hist->next;
        counter++;
    }

    return counter;
}

// Returns 1 if new module is different from last one
// Value has to be allocated
int hst_append(struct history *hist, int module_id,
        int enable, void *value)
{
    struct history *new = malloc(sizeof(struct history));
    new->id = module_id;
    new->enable = enable;
    new->value = value;
    new->next = NULL;

    while (hist->next)
    {
        hist = hist->next;
    }

    hist->next = new;
    if (hist->id == module_id)
        return 0;
    return 1;
}

// Returns 1 if we last module is different than the module before
int hst_pop(struct history *hist)
{
    if (!hist->next)
    {
        return 1;
    }

    while (hist->next->next)
    {
        hist = hist->next;
    }

    int return_value = hist->id != hist->next->id;
    free(hist->next->value);
    free(hist->next);
    hist->next = NULL;

    return return_value;
}

// When we have to duplicate a void* pointer, we must know the
// size of the data that has to be allocated
size_t get_data_size(int id)
{
    switch(id)
    {
        case CONTRASTE:
        case SATURATION:
        case EXPOSURE:
        case SHADOWS:
        case HIGHLIGHTS:
            return sizeof(float);
        case FLIP:
        case BW:
        case INVERT:
        case ROTATION:
            return sizeof(size_t);
        default:
            return sizeof(float);
    }

    return 0;
}

struct history *hst_duplicate(struct history *hist)
{
    struct history *new_hist = hst_new();

    hist = hist->next;

    struct history *new = new_hist;
    while (hist)
    {
        new->next = malloc(sizeof(struct history));
        new->next->id = hist->id;
        new->next->enable = hist->enable;

        void *new_value = malloc(get_data_size(hist->id));
        memcpy(new_value, hist->value, get_data_size(hist->id));
        new->next->value = new_value;
        new->next->next = NULL;
        new = new->next;
        hist = hist->next;
    }

    return new_hist;
}

char *get_name(int id)
{
    // TODO : Coding style : 5.11  cf 5.11 for details
    char *module_name[] = {"Invert", "Exposure", "Black and white",
        "Saturation", "Contraste", "Shadows", "Highlights",
        "Flip", "Rotation", "Black_and_White"};

    return id < 0 ? "NULL" : module_name[id];
}

void hst_print(struct history *hist)
{
    size_t nb_elm = hst_length(hist);
    for (size_t i = 0; i <= nb_elm; i++)
    {
        printf("------------------------------- Module %ld\n", i);
        printf("name: %s\n", get_name(hist->id));
        printf("id: %d\n", hist->id);
        printf("enable: %d\n", hist->enable);

        if (hist->id >= 0)
        {
            printf("Pointer : %p\n", hist->value);
        }

        hist = hist->next;
    }
}