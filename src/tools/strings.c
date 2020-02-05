#include <string.h>
#include <stdlib.h>

/*
** Return filename's extension
*/
const char *get_filename_ext(const char *filename)
{
    //Get the last '.' position
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename)
    {
        return "";
    }
    return dot + 1;
}

/*
** Delete path and returns file name ony
*/
const char *get_filename_from_path(const char *filename)
{
    //Get the last '.' position
    const char *dot = strrchr(filename, '/');
    if(!dot || dot == filename)
    {
        return "";
    }
    return dot + 1;
}