#include <err.h>
#include <string.h>
#include "import_ppm.h"
#include "import_tiff.h"
#include "import_jpeg.h"

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
    if(strcmp(ext,"tiff") == 0 || strcmp(ext,"tif") == 0)
    {
        return readTIFF(filename);
    }
    else if(strcmp(ext,"ppm") == 0)
    {
        return readPPM(filename);
    }
    else if(strcmp(ext,"jpeg") == 0 || strcmp(ext,"jpg") == 0)
    {
        return readJPEG(filename);
    }
    errx(1,"import: Unknown file extension '%s'\n", ext);
}