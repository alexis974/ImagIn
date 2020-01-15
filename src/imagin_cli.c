#include <stdlib.h>
#include <stdio.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "imagin.h"
#include "error_handler.h"

#include "import_export/import.h"
#include "import_export/export.h"
#include "import_export/xml.h"
#include "import_export/scale.h"

#include "modules/black_and_white.h"
#include "modules/invert.h"
#include "modules/flip.h"
#include "modules/saturation.h"
#include "modules/exposure.h"

int main(void)
{
    //Set error mode to CLI to have error in text format
    g_cli_mode = FALSE;

    struct Image *image;
    char *img_path = "samples/Canon_90D_ppm/Canon_90D_04.ppm";
    char *xml_path = "samples/Canon_90D_ppm/Canon_90D_04.xml";

    image = read_image(img_path);
    write_image("tmp/tmp0.jpg",image);

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

    //Rescale img
    write_image("tmp/tmp7.jpg", scale_img(image, 900, 600));

    //Add 0.5EV to image
    exposure(image, 0.5);
    write_image("tmp/tmp1.jpg",image);

    //Invert the colors of an image
    invert(image);
    write_image("tmp/tmp2.ppm",image);

    //Turn the image black and white
    simple_BW(image);
    write_image("tmp/tmp3.ppm",image);

    //Flip the image both horizontaly and verticaly
    flip_both_axis(image);
    write_image("tmp/tmp4.ppm",image);

    horizontal_flip(image);
    write_image("tmp/tmp5.ppm",image);

    vertical_flip(image);
    write_image("tmp/tmp6.ppm",image);

    free(image);
    printf("See you soon!\n");

    return 0;
}
