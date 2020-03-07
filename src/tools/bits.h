#ifndef BITS_H
#define BITS_H

size_t bits_to_depth(unsigned const char bits_count);

unsigned char depth_to_bits(size_t depth);

unsigned char *to_bytes(size_t original_depth, size_t *array, size_t len)

#endif /* ! BITS_H */
