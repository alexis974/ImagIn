#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "../../imagin.h"

#include "../../tools/matrix.h"

#include "sharpness.h"

void blur(struct Image *img, float sigma)
{
    if (!img)
    {
        errx(1, "blur: No image found");
    }

    if (sigma <= 0)
    {
        return;
    }

    struct Matrix kernel;
    initGaussianKernel(kernel, radius, sigma);
