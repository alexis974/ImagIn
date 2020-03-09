#include <stdio.h>
#include <err.h>

#include "convert.h"

char *get_name(int id)
{
    // TODO : Coding style : 5.11  cf 5.11 for details
    char *module_name[] = {"Invert", "Exposure", "Black and white",
        "Saturation", "Contraste", "Shadows", "Highlights",
        "Flip", "Rotation"};

    return id < 0 ? "NULL" : module_name[id];
}

int get_id(char *str)
{
    // TODO : Coding style : 5.11  cf 5.11 for details
    char *module_name[] = {"Invert", "Exposure", "Black and white",
        "Saturation", "Contraste", "Shadows", "Highlights",
        "Flip", "Rotation"};
    int nb_module = 9;

    for (int i = 0; i < nb_module; i++)
    {
        if (str == module_name[i])
        {
            return i;
        }
    }

    errx(1, "get_id: enable to find the id from the module name");
}
