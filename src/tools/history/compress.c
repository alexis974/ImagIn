#include <stdlib.h>

#include "history.h"

// Not counting adjacent modules with same id
// As if history was compressed before calculating length
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

// Compress adjacent modules by keeping the last one
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