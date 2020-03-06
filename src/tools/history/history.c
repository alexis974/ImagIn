#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <gtk/gtk.h>

#include "../../imagin.h"

#include "../../gui/gui.h"

#include "history.h"
#include "../free.h"

#include "../../modules/user/contrast.h"
#include "../../modules/user/exposure.h"
#include "../../modules/user/flip.h"
#include "../../modules/user/saturation.h"
#include "../../modules/user/shadows_highlights.h"
#include "../../modules/user/black_and_white.h"
#include "../../modules/user/invert.h"

// TODO : Coding style : 4.2 Max 10 fcts per file

struct history *hst_new(void)
{
    struct history *hist = malloc(sizeof(struct history));

    hist->id = -1;
    hist->enable = 0;
    hist->value = 0;
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


// Not counting adjacent modules with same id
size_t hst_compressed_length(struct history *hist)
{
    size_t counter = 0;

    while (hist->next)
    {
        counter++;
        hist = hist->next;

        while (hist->next && hist->id == hist->next->id)
            hist = hist->next;
    }

    return counter;
}

// Returns 1 if new module is different from last one
int hst_append(struct history *hist, int module_id,
        int enable, float value)
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

    free(hist->next);
    hist->next = NULL;

    return return_value;
}

// Inserts or changes value in a sorted chained list
void hst_insert_sort(struct history *hist, int module_id,
        int enable, float value)
{
    while (hist->next && module_id >= hist->next->id)
    {
        hist = hist->next;
    }

    if (hist->id == module_id)
    {
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

    float value = elm1->value;
    elm1->value = elm2->value;
    elm2->value = value;
}


// Sorting the history using the buublesort algorithm
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

//Update the enable value of the last hist element with module_id
void hst_enable_last(struct history *hist, int module_id, int enable)
{
    struct history *last = NULL;

    while (hist)
    {
        if (hist->id == module_id)
            last = hist;

        hist = hist->next;
    }

    if (last != NULL)
        last->enable = enable;
}

// TODO : Coding style : 4.10  Fct max 25 lines
// Applies a history (should be compressed before)
void hst_apply_all(struct history *hist, struct Image *img)
{
    for (struct history *p = hist->next; p != NULL; p=p->next)
    {
        if (!p->enable)
            continue;

        switch (p->id)
        {
        case CONTRASTE:
            contrast(img, p->value + 1);
            break;
        case EXPOSURE:
            exposure(img, p->value);
            break;
        case SATURATION:
            saturation(img, p->value + 1);
            break;
        case FLIP:
            flip(img, p->value);
            break;
        case BW:
            if (p->value)
            {
                simple_BW(img);
            }
            break;
        case INVERT:
            if (p->value)
            {
                invert(img);
            }
            break;
        default:
            break;
        }
    }
}

void hst_compress(struct history *hist)
{
    struct history *old = hist;

    while (hist->next != NULL)
    {
        if (hist->id == hist->next->id)
        {
            old->next = hist->next;
            free(hist);
            hist = old->next;
        }
        else
        {
            old = hist;
            hist = hist->next;
        }
    }
}

// Keep only history until index
void hst_truncate(struct history *hist, size_t index)
{
    for (size_t count = 0; count < index; count++)
    {
        hist = hist->next;
    }

    hst_free_recursively(hist->next);
    hist->next = NULL;
}

/*
** Keep "count" elements but allowing repetitions
** This is useful for going backward in history
** but still being able to undo previous changes
*/
void hst_truncate_uncompressed(struct history *hist, size_t count)
{
    while (hist->next && count > 0)
    {
        count --;

        hist = hist->next;

        // Passing every modules that has same id
        while (hist->next && hist->id == hist->next->id)
            hist = hist->next;
    }

    hst_free_recursively(hist->next);
    hist->next = NULL;
}

struct history *hst_duplicate(struct history *hist)
{
    struct history *new_hist = hst_new();

    hist = hist->next;

    struct history *new= new_hist;
    while (hist)
    {
        new->next = malloc(sizeof(struct history));
        new->next->id = hist->id;
        new->next->enable = hist->enable;
        new->next->value = hist->value;
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
        printf("value: %f\n", hist->value);

        hist = hist->next;
    }
}

void copy_img(struct Image *img_src, struct Image *img_dst)
{
    if (!img_src || !img_dst)
    {
        errx(1, "copy_img: No image found");
    }

    if (img_src->width != img_dst->width || img_src->height != img_dst->height)
    {
        errx(1, "copy_img: images don't have the same size");
    }

    for (size_t i = 0; i < img_src->width * img_src->height; i++)
    {
        img_dst->data[i].red = img_src->data[i].red;
        img_dst->data[i].green = img_src->data[i].green;
        img_dst->data[i].blue = img_src->data[i].blue;
    }
}
