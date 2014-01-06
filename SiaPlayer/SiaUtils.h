#ifndef SIAUTILS_H
#define SIAUTILS_H

#include "VideoState.h"
#include "libavutil/avutil.h" 
#include "libavcodec/avcodec.h"
#include "libavutil/pixfmt.h"

void print_error(const char *filename, int err);

int utils_img_convert(AVPicture* dst, enum AVPixelFormat dst_pix_fmt, 
	AVPicture* src, enum AVPixelFormat pix_fmt, int width, int height);

static __inline int isnan(float x)
{
    uint32_t v = av_float2int(x);
    if ((v & 0x7f800000) != 0x7f800000)
        return 0;
    return v & 0x007fffff;
}

#endif
