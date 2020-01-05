#include <err.h>
#include <string.h>
#include "import.h"
#include "export_ppm.h"
#include "export_tiff.h"

void write(const char *filename, struct Image *img)
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
    else
    {
        errx(1,"export: Unknown file extension '%s'\n", ext);
    }
}