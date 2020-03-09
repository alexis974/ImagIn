#include <stdlib.h>

#include <gtk/gtk.h>

#include "../../imagin.h"
#include "../../gui/gui.h"

#include "history.h"
#include "../free.h"

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