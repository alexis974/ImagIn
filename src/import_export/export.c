#include <string.h>

#include "ppm.h"
#include "tiff.h"
#include "jpeg.h"
#include "png.h"

#include "../error_handler.h"
#include "../imagin.h"

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
** Write image file given an image and a file name
*/
void write_ext(const char *filename, const char *ext, struct Image *img)
{
    if(strcmp(ext, "tiff") == 0 || strcmp(ext, "tif") == 0)
    {
        writeTIFF(filename, img);
    }
    else if(strcmp(ext, "ppm") == 0)
    {
        writePPM(filename, img);
    }
    else if(strcmp(ext, "jpeg") == 0 || strcmp(ext, "jpg") == 0)
    {
        writeJPEG(filename, img);
    }
    else
    {
        throw_error("export", "Unknown file extension.");
    }
    return;
}

void write_image(const char *filename, struct Images *images)
{
    const char* ext = get_filename_ext(filename);
    write_ext(filename, ext, images->full);
    return;
}
