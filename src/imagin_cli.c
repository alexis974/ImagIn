#include <stdlib.h>
#include <stdio.h>

#include "imagin.h"
#include "import_export/import.h"
#include "import_export/export.h"
#include "modules/black_and_white.h"
#include "modules/invert.h"
#include "modules/flip.h"
#include "modules/saturation.h"

int main(void)
{
    struct Image *image;
    image = read_image("samples/Canon_90D_ppm/Canon_90D_03.ppm");
    write_image("tmp/tmp0.tiff",image);

    //Decrease the saturation by a factor of 2
    //	saturation(image, 0.5);
    //  writePPM("tmp/tmp1.ppm",image);

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