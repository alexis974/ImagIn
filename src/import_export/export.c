#include <string.h>
#include <gtk/gtk.h>

#include "formats/ppm.h"
#include "formats/tiff.h"
#include "formats/jpeg.h"
#include "formats/png.h"

#include "../imagin.h"
#include "../debug/error_handler.h"
#include "../tools/strings.h"

#include "export.h"

// Write image file given an image and a file name
void write_ext(const char *filename, const char *ext, struct Image *img)
{
    if (strcmp(ext, "tiff") == 0 || strcmp(ext, "tif") == 0
        || strcmp(ext, "TIFF") == 0 || strcmp(ext, "TIF") == 0)
    {
        write_tiff(filename, img);
    }

    else if (strcmp(ext, "ppm") == 0 || strcmp(ext, "PPM") == 0)
    {
        write_ppm(filename, img);
    }

    else if (strcmp(ext, "jpeg") == 0 || strcmp(ext, "jpg") == 0
        || strcmp(ext, "JPEG") == 0 || strcmp(ext, "JPG") == 0)
    {
        write_jpeg(filename, img);
    }

    else if (strcmp(ext, "png") == 0 || strcmp(ext, "PNG") == 0)
    {
        write_png(filename, img);
    }

    else
    {
        throw_error("export", "Unknown file extension.");
    }

    return;
}

void write_image(const char *filename, struct Image *image)
{
    const char *ext = get_filename_ext(filename);
    write_ext(filename, ext, image);

    return;
}
