#include "../error_handler.h"
#include <string.h>
#include "ppm.h"
#include "tiff.h"
#include "jpeg.h"
#include "png.h"

const char *get_filename_ext(const char *filename)
{
    //Gets the last '.' position
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

struct Image *read_image(const char *filename)
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
    else if(strcmp(ext,"png") == 0)
    {
        return readPNG(filename);
    }
    throw_error("import", "Unknown extension.");
    return NULL;
}