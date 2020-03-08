#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "../imagin.h"

#include "../gui/gui.h"
#include "../gui/gui_display.h"
#include "../debug/error_handler.h"
#include "../modules/imagin/scale.h"
#include "../tools/strings.h"
#include "../tools/exif.h"

#include "import.h"

#include "formats/ppm.h"
#include "formats/tiff.h"
#include "formats/jpeg.h"
#include "formats/png.h"


// Return struct Image from an image file and its extension
struct Image *read_ext(const char *filename, const char *ext)
{
    if (strcmp(ext, "tiff") == 0 || strcmp(ext, "tif") == 0
            || strcmp(ext, "TIFF") == 0 || strcmp(ext, "TIF") == 0)
    {
        return read_tiff(filename);
    }

    else if (strcmp(ext, "ppm") == 0 || strcmp(ext, "PPM") == 0)
    {
        return read_ppm(filename);
    }

    else if (strcmp(ext, "jpeg") == 0 || strcmp(ext, "jpg") == 0
            || strcmp(ext, "JPEG") == 0 || strcmp(ext, "JPG") == 0)
    {
        return read_jpeg(filename);
    }

    else if (strcmp(ext, "png") == 0 || strcmp(ext, "PNG") == 0)
    {
        return read_png(filename);
    }

    throw_error("Import", "Unknown extension.");

    return NULL;
}


// TODO : Remove explicit cast
// Return the scaled image given the full image
struct Image *get_scale(struct Image *full_img)
{
    float ratio = (float) full_img->width / full_img->height;
    size_t width = g_maxwidth;
    size_t height = (size_t) width / ratio;

    if (height > g_maxheight)
    {
        height = g_maxheight;
        width = (size_t) height * ratio;
    }

    return scale_img(full_img, width, height);
}


// Return the small image given the scale image
struct Image *get_small(struct Image *scaled_img)
{
    float ratio = (float) scaled_img->width / scaled_img->height;
    size_t width = g_maxwidth_small;
    size_t height = (size_t) width / ratio;

    if (height > g_maxheight_small)
    {
        height = g_maxheight_small;
        width = (size_t) height * ratio;
    }

    return scale_img(scaled_img, width, height);
}


// Return a struct Images from an image file
struct Images *read_image(const char *filename)
{
    struct Images *images = malloc(sizeof(struct Images));

    const char *ext = get_filename_ext(filename);
    images->full = read_ext(filename, ext);

    if (!images->full)
    {
        return NULL;
    }

    images->scale = get_scale(images->full);
    images->edit = get_scale(images->full);
    images->small = get_small(images->scale);

    return images;
}
