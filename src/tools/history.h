#ifndef HISTORY_H
#define HISTORY_H

enum ModulesID
{
    EXPOSURE,
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

void apply_history(struct history *hist, struct Images *img);

void init_history(struct history *hist);

int history_is_empty(struct history *hist);

size_t history_length(struct history *hist);

void history_append(struct history *hist, int module_id,
        int enable, float value);

void swap_module(struct history *elm1, struct history *elm2);

void history_sort(struct history *hist);

struct history *duplicate_history(struct history *hist);

void compress_history(struct history *hist);

void truncate_history(struct history *hist, size_t index);

char *get_name(int id);

void print_history(struct history *hist);

void copy_img(struct Image *img_src, struct Image *img_dst);

#endif
