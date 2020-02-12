#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <gtk/gtk.h>

#include "../imagin.h"
#include "../gui/gui.h"

#include "history.h"

#include "../modules/contrast.h"
#include "../modules/exposure.h"
#include "../modules/flip.h"
#include "../modules/saturation.h"
#include "../modules/shadows_highlights.h"
#include "../modules/black_and_white.h"
#include "../modules/invert.h"

// TODO : Coding style : 4.2 Max 10 fcts per file

struct history *hst_new(void)
{
    struct history *hist = malloc(sizeof(struct history));
    hst_init(hist);

    return hist;
}

void hst_init(struct history *hist)
{
    hist->id = -1;
    hist->enable = 0;
    hist->value = 0;
    hist->next = NULL;
}

int hst_is_empty(struct history *hist)
{
    return (hist->next) ?  0 : 1;
}

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


//Not counting adjacent modules with same id
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

// TODO : Coding style : 4.10  Fct max 25 lines
void reset_widgets(struct history *hist, struct UI *ui)
{
    ui->can_modify = FALSE;
    struct history *compressed = hst_duplicate(hist);
    hst_compress(compressed);
    for (struct history *p = compressed->next; p != NULL; p=p->next)
    {
        switch (p->id)
        {
        case CONTRASTE:
            gtk_range_set_value(GTK_RANGE(
                    ui->modules->cont_exp_sat->contraste_scale), p->value);
            break;
        case EXPOSURE:
            gtk_range_set_value(GTK_RANGE(
                    ui->modules->cont_exp_sat->exposure_scale), p->value);
            break;
        case SATURATION:
            gtk_range_set_value(GTK_RANGE(
                    ui->modules->cont_exp_sat->saturation_scale), p->value);
            break;
        case SHADOWS:
            // TODO : Coding style : 3.5 Max 80 char per line
            gtk_range_set_value(GTK_RANGE(
                    ui->modules->shadows_highlights->shadows_scale), p->value);
            break;
        case HIGHLIGHTS:
            // TODO : Coding style : 3.5 Max 80 char per line
            gtk_range_set_value(GTK_RANGE(
                    ui->modules->shadows_highlights->highlights_scale), p->value);
            break;
        case FLIP:
            gtk_combo_box_set_active(GTK_COMBO_BOX(
                    ui->modules->orientation->flip_box), p->value);
            break;
        case BW:
            gtk_switch_set_state(ui->modules->bw_switch, p->value);
            break;
        case INVERT:
            gtk_switch_set_state(ui->modules->invert_switch, p->value);
            break;
        default:
            break;
        }
    }
    ui->can_modify = TRUE;
    hst_free_recursively(compressed);
}

// TODO : Coding style : 4.10  Fct max 25 lines
// Applies a history (should be compressed before)
void hst_apply_all(struct history *hist, struct Image *img)
{
    for (struct history *p = hist->next; p != NULL; p=p->next)
    {
        switch (p->id)
        {
        case CONTRASTE:
            contrast(img, p->value);
            break;
        case EXPOSURE:
            exposure(img, p->value);
            break;
        case SATURATION:
            saturation(img, p->value + 1);
            break;
        case FLIP:
            if(p->value == 1)
            {
                vertical_flip(img);
            }
            else if(p->value ==  2)
            {
                horizontal_flip(img);
            }
            else if(p->value == 3)
            {
                flip_both_axis(img);
            }
            break;
        case BW:
            if(p->value)
            {
                simple_BW(img);
            }
            break;
        case INVERT:
            if(p->value)
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
 **  Free from hist till the end (hist included)
 **  Do not use it with hist->next as arguments
 **  because next pointer cannot be set to NULL
 */
void hst_free_recursively(struct history *hist)
{
    if (!hist)
    {
        return;
    }

    hst_free_recursively(hist->next);
    free(hist);
}

struct history *hst_duplicate(struct history *hist)
{
    struct history *new = malloc(sizeof(struct history));
    struct history *new_hist = new;

    hst_init(new);
    hist = hist->next;

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
