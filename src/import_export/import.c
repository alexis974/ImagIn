#include <err.h>
#include <string.h>
#include "import_ppm.h"
#include "import_tiff.h"

const char *get_filename_ext(const char *filename)
{
    //Gets the last '.' position
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

struct Image *read(const char *filename)
{
    const char* ext = get_filename_ext(filename);
    if(strcmp(ext,"tiff") == 0)
    {
        return readTIFF(filename);
    }
    else if(strcmp(ext,"ppm") == 0)
    {
        return readPPM(filename);
    }
    errx(1,"import.c: Unknown file extension '%s'\n", ext);
}