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

#include "tools/free.h"
#include "tools/history/history.h"

// TODO : Coding style : 4.10 Fct max 25 lines
int main(void)
{
    // Set error mode to CLI to have error in text format
    // TODO : Coding style : 2.5 typedef ?
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

//################################################################# XML_TEST ###

    char *xml2_path = "samples/Canon_90D_ppm/Canon_90D_03_test1.xml";
    //char *xml3_path = "samples/Canon_90D_ppm/Canon_90D_03_test2.xml";

    FILE *file2;
    if ((file2 = fopen(xml2_path, "r")))
    {
        printf("XML2 file already exist\n");
        fclose(file);
    }
    else
    {
        printf("No XML2 file found. Creating one...\n\n");

    //////////////////////////////////////////////////////////////
    struct history *hist = hst_new();

    hst_append(hist, 1, 0, 5);
    hst_append(hist, 2, 1, 6.89);
    hst_append(hist, 3, 0, 22.1);
    hst_append(hist, 4, 1, 5.1);
    hst_append(hist, 5, 0, 77.77);

    save_hist(hist, xml2_path);
    //hst_free_recursively(hist);
    printf("history has been saved in xml2\n");

    /*
    struct history *hist2 = get_hist_from_xml(uri);
    if (hist2 == NULL)
        printf("test fct 2 case NULL\n");
    else
        printf("test fct 2 case ok\n");

    save_hist_xml(hist2, uri2);
    //hst_free_recursively(hist2);
    printf("test save hist2 xml done, go check .xml\n");
*/
        printf("----------------------------------------done\n");
    }

//################################################################## MODULES ###

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

    return 0;
}
