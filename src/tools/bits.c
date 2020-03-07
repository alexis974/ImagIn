#include <stdlib.h>

size_t bits_to_depth(unsigned const char bits_count)
{
    return (8 * bits_count) - 1;
}

unsigned char depth_to_bits(size_t depth)
{
    return (depth + 1) / 8;
}

// Convert an array of size_t of a valid depth into a bytes array
// by mapping values
unsigned char *convert_to_byte_array(size_t original_depth,
    size_t *array, size_t len)
{
    unsigned char *new_array = malloc(sizeof(unsigned char) * len);

    for (size_t i = 0; i < len; i++)
    {
        new_array[i] = (array[i] * 255) / original_depth;
    }

    return  new_array;
}