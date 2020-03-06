#include <stdlib.h>
#include <gtk/gtk.h>

#include "../imagin.h"

#include "../gui/gui.h"

#include "history/history.h"

#include "free.h"

void free_image(struct Image *img)
{
    free(img->data);
    free(img);

    return;
}

void free_images(struct Images *images)
{
    free_image(images->full);
    free_image(images->scale);
    free_image(images->edit);
    free_image(images->small);
    free(images);

    return;
}

/*
 **  Free from hist till the end (hist included)
 **  Do not use it with hist->next as arguments
 **  because "next" pointer cannot be set to NULL
 **  or set it manually then
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

void free_all(struct UI *ui)
{
    free(ui->modules->bw_exp);
    free(ui->modules->invert_exp);
    free(ui->modules->cont_exp_sat->exp);
    free(ui->modules->orientation->exp);
    free(ui->modules->shadows_highlights->exp);

    free(ui->image_info);

    free(ui->modules->history_list);
    free(ui->modules->shadows_highlights);
    free(ui->modules->orientation);
    free(ui->modules->cont_exp_sat);

    free(ui->bottom_bar);
    free(ui->display);
    free(ui->menu_bar);
    free(ui->modules);

    if (ui->image_loaded)
    {
        free_images(ui->images);
    }

    hst_free_recursively(ui->hist);
    hst_free_recursively(ui->compressed_hist);

    free(ui);
}