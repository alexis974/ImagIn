#include <stdlib.h>
#include <stdio.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <gtk/gtk.h>

#include "imagin.h"

#include "debug/error_handler.h"

#include "import_export/import.h"
#include "import_export/export.h"
#include "import_export/xml/xml.h"

#include "modules/user/black_and_white.h"
#include "modules/user/invert.h"
#include "modules/user/flip.h"
#include "modules/user/saturation.h"
#include "modules/user/exposure.h"
#include "modules/user/contrast.h"

#include "modules/imagin/scale.h"
#include "modules/imagin/histogram.h"

#include "gui/gui.h"

#include "tools/history/history.h"

#include "tools/free.h"

// TODO : Coding style : 4.10 Fct max 25 lines
int main(void)
{
    // Set error mode to CLI to have error in text format
    // TODO : Coding style : 2.5 typedef ?
    g_cli_mode = FALSE;

    // Define paths
    char *img_path = "samples/Canon_90D_png/16bits.png";
    char *xml_path = "samples/Canon_90D_ppm/Canon_90D_03.ppm.xml";


//################################################################### IMPORT ###

    /* try to open file to read */
    FILE *file;
    if ((file = fopen(xml_path, "r")))
    {
        printf("XML file already exist\n");
        fclose(file);
    }
    else
    {
        printf("No XML file found. Creating one...\n");
        create_xmp(xml_path);
    }

    // Init struct Images
    struct Images *images = malloc(sizeof(struct Images));

    // Store all the pixel of the image
    images = read_image(img_path);
    write_image("tmp/Full_image.png", images->full);

    // Scale the image to fit gui window
    images->scale = scale_img(images->full, 1200, 800);
    images->edit = scale_img(images->full, 1200, 800);
    images->small = scale_img(images->full, 225, 150);
    write_image("tmp/Scale_image.png", images->scale);
    write_image("tmp/Small_image.png", images->small);


//################################################################## MODULES ###

    //Modify saturation
    saturation(images->edit, 2);
    write_image("tmp/01_Saturation.png", images->edit);

    //Modify contrast
    contrast(images->edit, 2);
    write_image("tmp/02_Contrast.png", images->edit);

    // Add 0.5EV to image
    exposure(images->edit, 0.5);
    write_image("tmp/03_Exposure.jpg", images->edit);

    // Invert the colors of an image
    invert(images->edit);
    write_image("tmp/04_Invert.png", images->edit);

    // Turn the image black and white
    simple_BW(images->edit);
    write_image("tmp/05_Black_and_white.png", images->edit);

    // Flip the image both horizontaly and verticaly
    flip(images->edit, 3);
    write_image("tmp/06_Flip_both_axis.png", images->edit);

    // Flip the horizontaly
    flip(images->edit, 2);
    write_image("tmp/07_Flip_horizontal.png", images->edit);

    // Flip the image verticaly
    flip(images->edit, 1);
    write_image("tmp/08_Flip_vertiacl.png", images->edit);

    struct Histogram *histogram = compute_histogram(images->edit);

    printf("Max value = %ld\n", histo_max_value(histogram));

    free_images(images);

    printf("See you soon!\n");

    struct Image *img = malloc(sizeof(struct Image));
    img->height = 1;
    img->width = 3;
    img->bit_depth =  65535;
    img->data = malloc(sizeof(struct Pixel) * 3);
    img->data[0].red = 65535;
    img->data[0].blue = 65535;
    img->data[0].green = 65535;
    img->data[2].red = 65535;
    img->data[2].blue = 65535;
    img->data[2].green = 65535;
    img->data[1].red = 0;
    img->data[1].blue = 0;
    img->data[1].green = 0;

    //write_image("tmp/test.png", img);
    write_image("tmp/test.tiff", img);
    //write_image("tmp/test.jpg", img);

    return 0;
}
