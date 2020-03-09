#ifndef HISTORY_H
#define HISTORY_H
// TODO : Coding style : 4.1 max 5 fcts
enum ModulesID
{
    INVERT,
    EXPOSURE,
    BW,
    SATURATION,
    CONTRASTE,
    SHADOWS,
    HIGHLIGHTS,
    FLIP,
    ROTATION
};

struct history
{
    int id;
    int enable;
    float value;
    struct history *next;
};

struct history *hst_new(void);

size_t hst_length(struct history *hist);

int hst_append(struct history *hist, int module_id,
        int enable, float value);

int hst_pop(struct history *hist);

struct history *hst_duplicate(struct history *hist);

char *get_name(int id);

void hst_print(struct history *hist);

#endif /* !  HISTORY_H */
