#include <err.h>
#include <string.h>
#include "import.h"
#include "ppm.h"
#include "tiff.h"
#include "jpeg.h"

void write_image(const char *filename, struct Image *img)
{
    const char* ext = get_filename_ext(filename);
    if(strcmp(ext,"tiff") == 0 || strcmp(ext,"tif")==0)
    {
        writeTIFF(filename, img);
    }
    else if(strcmp(ext,"ppm") == 0)
    {
        writePPM(filename, img);
    }
    else if(strcmp(ext,"jpeg") == 0 || strcmp(ext,"jpg") == 0)
    {
        writeJPEG(filename, img);
    }
    else
    {
        errx(1,"export: Unknown file extension '%s'\n", ext);
    }
}