#ifndef HISTOGRAM_H
#define HISTOGRAM_H

struct Histogram
{
    size_t *red;
    size_t *green;
    size_t *blue;
};

struct Histogram *compute_histogram(struct Image *img);

#endif
