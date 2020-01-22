#include <string.h>

#include "../imagin.h"
#include "../error_handler.h"

#include "import.h"
#include "ppm.h"
#include "tiff.h"
#include "jpeg.h"
#include "png.h"

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

void write_image(const char *filename, struct Image *image)
{
    const char* ext = get_filename_ext(filename);
    write_ext(filename, ext, image);
    return;
}
