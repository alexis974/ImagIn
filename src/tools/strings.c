#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Return filename's extension
const char *get_filename_ext(const char *filename)
{
    // Get the last '.' position
    const char *dot = strrchr(filename, '.');

    if (!dot || dot == filename)
    {
        return "";
    }

    return dot + 1;
}

// Delete path and returns file name ony
const char *get_filename_from_path(const char *filename)
{
    // Get the last '.' position
    const char *dot = strrchr(filename, '/');

    if (!dot || dot == filename)
    {
        return "";
    }

    return dot + 1;
}


// Input: "AAAA/MM/DD HH/MM/SS"
void format_datetime(char* input, char* output_date, char* output_time)
{
    snprintf(output_date, 11, "%s\n",input);
    snprintf(output_time, 9, "%s\n", input+11);
}
