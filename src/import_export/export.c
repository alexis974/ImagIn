#include <string.h>
#include <gtk/gtk.h>

#include "ppm.h"
#include "tiff.h"
#include "jpeg.h"
#include "png.h"

#include "../imagin.h"
#include "../debug/error_handler.h"
#include "../tools/strings.h"


// Write image file given an image and a file name
void write_ext(const char *filename, const char *ext, struct Image *img)
{
    // TODO : Coding style : 5.21 Space after keyword (here: if)
    if(strcmp(ext, "tiff") == 0 || strcmp(ext, "tif") == 0
        || strcmp(ext, "TIFF") == 0 || strcmp(ext, "TIF") == 0)
    {
        writeTIFF(filename, img);
    }

    // TODO : Coding style : 5.21 Space after keyword (here: if)
    else if(strcmp(ext, "ppm") == 0 || strcmp(ext, "PPM") == 0)
    {
        writePPM(filename, img);
    }

    // TODO : Coding style : 5.21 Space after keyword (here: if)
    else if(strcmp(ext, "jpeg") == 0 || strcmp(ext, "jpg") == 0
        || strcmp(ext, "JPEG") == 0 || strcmp(ext, "JPG") == 0)
    {
        writeJPEG(filename, img);
    }

    // TODO : Coding style : 5.21 Space after keyword (here: if)
    else if(strcmp(ext, "png") == 0 || strcmp(ext, "PNG") == 0)
    {
        writePNG(filename, img);
    }

    else
    {
        throw_error("export", "Unknown file extension.");
    }

    return;
}

void write_image(const char *filename, struct Image *image)
{
    // TODO : Coding style : 5.4 '*' with name, not type
    const char* ext = get_filename_ext(filename);
    write_ext(filename, ext, image);

    return;
}
