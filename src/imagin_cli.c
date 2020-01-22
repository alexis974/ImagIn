#include <stdlib.h>
#include <stdio.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "imagin.h"
#include "debug/error_handler.h"

#include "import_export/import.h"
#include "import_export/export.h"
#include "import_export/free.h"
#include "import_export/xml/xml.h"

#include "modules/black_and_white.h"
#include "modules/invert.h"
#include "modules/flip.h"
#include "modules/saturation.h"
#include "modules/exposure.h"

#include "tools/scale.h"

int main(void)
{
    // Set error mode to CLI to have error in text format
    g_cli_mode = FALSE;

    // Define paths
    char *img_path = "samples/Canon_90D_ppm/Canon_90D_03.ppm";
    char *xml_path = "samples/Canon_90D_ppm/Canon_90D_03.ppm.xml";


//##############################################################################
//                               ### IMPORT ###                              ###
//##############################################################################

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
    struct Images *images;
    images = malloc(sizeof(struct Images));

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


//##############################################################################
//                               ### MODULES ###                             ###
//##############################################################################

    //Add 0.5EV to image
    exposure(images->edit, 0.5);
    write_image("tmp/01_Exposure.jpg", images->edit);

    //Invert the colors of an image
    invert(images->edit);
    write_image("tmp/02_Invert.jpg", images->edit);

    //Turn the image black and white
    simple_BW(images->edit);
    write_image("tmp/03_Black_and_white.ppm", images->edit);

    //Flip the image both horizontaly and verticaly
    flip_both_axis(images->edit);
    write_image("tmp/04_Flip_both_axis.tiff", images->edit);

    //Flip the horizontaly
    horizontal_flip(images->edit);
    write_image("tmp/05_Flip_horizontal.jpg", images->edit);

    //Flip the image verticaly
    vertical_flip(images->edit);
    write_image("tmp/06_Flip_vertiacl.ppm", images->edit);

    free_images(images);

    printf("See you soon!\n");

    return 0;
}
