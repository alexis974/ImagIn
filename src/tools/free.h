#ifndef FREE_H
#define FREE_H

void free_image(struct Image *img);

void free_images(struct Images *images);

void hst_free_recursively(struct history *hist);

void free_all(struct UI *ui);

#endif /* ! FREE_H */
