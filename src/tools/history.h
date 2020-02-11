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

void reset_widgets(struct history *hist, struct UI *ui);

void hst_apply_all(struct history *hist, struct Image *img);

struct history *hst_new(void);

void hst_init(struct history *hist);

int hst_is_empty(struct history *hist);

size_t hst_length(struct history *hist);

int hst_append(struct history *hist, int module_id,
        int enable, float value);

int hst_pop(struct history *hist);

void hst_insert_sort(struct history *hist, int module_id,
        int enable, float value);

void hst_sort(struct history *hist);

size_t hst_compressed_length(struct history *hist);

void hst_free_recursively(struct history *hist);

struct history *hst_duplicate(struct history *hist);

void hst_compress(struct history *hist);

void hst_truncate(struct history *hist, size_t index);

char *get_name(int id);

void hst_print(struct history *hist);

void copy_img(struct Image *img_src, struct Image *img_dst);

#endif /* !  HISTORY_H */
