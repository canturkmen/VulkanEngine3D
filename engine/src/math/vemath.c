#include "vemath.h"
#include "platform/platform.h"

#include <math.h>
#include <stdlib.h>

static b8 rand_seeded = false;

/**
 * Note that these are here in order to prevent having to import the
 * entire <math.h> everywhere.
 */

f32 vesin(f32 x)
{
    return sinf(x);
}

f32 vecos(f32 x)
{
    return cosf(x);
}

f32 vetan(f32 x)
{
    return tanf(x);
}

f32 veacos(f32 x)
{
    return acosf(x);
}

f32 vesqrt(f32 x)
{
    return sqrtf(x);
}

f32 veabs(f32 x)
{
    return fabs(x);  
}

i32 verandom()
{
    if(!rand_seeded)
    {
        srand((u32)platform_get_absolute_time());
        rand_seeded = true;
    }

    return rand();
}

i32 random_in_range(i32 min, i32 max)
{
    if(!rand_seeded)
    {
        srand((u32)platform_get_absolute_time());
        rand_seeded = true;
    }

    return (rand() % (max - min + 1)) + min;
}

f32 fverandom() 
{
    return (float)verandom() / (f32)RAND_MAX;
}

f32 fverandom_in_range(f32 min, f32 max) 
{
    return min + ((float)verandom() / ((f32)RAND_MAX / (max - min)));
}