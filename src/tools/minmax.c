#include <stdio.h>

#include "minmax.h"

float getmin2(float x, float y)
{
    return x < y ? x : y;
}

float getmax2(float x, float y)
{
    return x > y ? x : y;
}

float getmin3(float x, float y, float z)
{
    return getmin2(x, getmin2(y, z));
}

float getmax3(float x, float y, float z)
{
    return getmax2(x, getmax2(y, z));
}
