#include <stdlib.h>
#include "history.h"

#include "../../imagin.h"

#include "../../modules/user/contrast.h"
#include "../../modules/user/exposure.h"
#include "../../modules/user/flip.h"
#include "../../modules/user/saturation.h"
#include "../../modules/user/shadows_highlights.h"
#include "../../modules/user/black_and_white.h"
#include "../../modules/user/invert.h"

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
        {
            float *value = p->value;
            contrast(img, *value + 1);
            break;
        }
        case EXPOSURE:
        {
            float *value = p->value;
            exposure(img, *value);
            break;
        }
        case SATURATION:
        {
            float *value = p->value;
            saturation(img, *value + 1);
            break;
        }
        case FLIP:
        {
            size_t *value = p->value;
            flip(img, *value);
            break;
        }
        case BW:
        {
            size_t *value = p->value;
            if (*value)
            {
                simple_BW(img);
            }
            break;
        }
        case INVERT:
        {
            size_t *value = p->value;
            if (*value)
            {
                invert(img);
            }
            break;
        }
        default:
            break;
        }
    }
}

// Updates the enable value of the last hist element with module_id
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