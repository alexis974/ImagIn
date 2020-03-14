#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "imagin.h"

#include "debug/error_handler.h"

#include "import_export/import.h"
#include "import_export/export.h"
#include "import_export/xml/xml.h"

#include "tools/matrix.h"

#include "modules/imagin/scale.h"

#include "modules/user/black_and_white.h"
#include "modules/user/invert.h"
#include "modules/user/saturation.h"
#include "modules/user/exposure.h"
#include "modules/user/contrast.h"
#include "modules/user/sharpness.h"

int main(void)
{
    // Set error mode to CLI to have error in text format
    g_cli_mode = FALSE;

    // Define paths
    char *img_path = "samples/Canon_90D_ppm/Canon_90D_03.ppm";
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
    write_image("tmp/Full_image.jpg", images->full);
    write_image("tmp/Full_image.ppm", images->full);
    write_image("tmp/Full_image.tiff", images->full);

    // Scale the image to fit gui window
    images->scale = scale_img(images->full, 1200, 800);
    images->edit = scale_img(images->full, 1200, 800);
    images->small = scale_img(images->full, 225, 150);
    write_image("tmp/Scale_image.ppm", images->scale);
    write_image("tmp/Small_image.ppm", images->small);


//################################################################## MODULES ###

    //Blur test
    blur(images->edit, 1);
    write_image("tmp/00_Blur.jpg", images->edit);

    //Modify saturation
    saturation(images->edit, 2);
    write_image("tmp/01_Saturation.jpg", images->edit);

    //Modify contrast
    contrast(images->edit, 2);
    write_image("tmp/02_Contrast.jpg", images->edit);

    // Add 0.5EV to image
    exposure(images->edit, 0.5);
    write_image("tmp/03_Exposure.jpg", images->edit);

    // Invert the colors of an image
    invert(images->edit);
    write_image("tmp/04_Invert.jpg", images->edit);

    // Turn the image black and white
    simple_BW(images->edit);
    write_image("tmp/05_Black_and_white.ppm", images->edit);

    printf("See you soon!\n");

    return 0;
}
