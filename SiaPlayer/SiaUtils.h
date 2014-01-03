#ifndef SIAUTILS_H
#define SIAUTILS_H

#include "libavutil/avutil.h" 

void print_error(const char *filename, int err);

static __inline int isnan(float x)
{
    uint32_t v = av_float2int(x);
    if ((v & 0x7f800000) != 0x7f800000)
        return 0;
    return v & 0x007fffff;
}

#endif
