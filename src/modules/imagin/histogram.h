#ifndef HISTOGRAM_H
#define HISTOGRAM_H

struct Histogram
{
    size_t *r;
    size_t *g;
    size_t *b;
    size_t *w;
};

struct Histogram *compute_histogram(struct Image *img);

void free_histogram(struct Histogram *histo);

size_t histo_max_value(struct Histogram *histo, size_t depth);

#endif /* ! HISTOGRAM_H */
