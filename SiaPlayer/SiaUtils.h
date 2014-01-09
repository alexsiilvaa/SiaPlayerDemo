#ifndef SIAUTILS_H
#define SIAUTILS_H

#include "VideoState.h"
#include "libavutil/avutil.h" 
#include "libavcodec/avcodec.h"
#include "libavutil/pixfmt.h"

void print_error(const char *filename, int err);

int utils_img_convert(AVPicture* dst, enum AVPixelFormat dst_pix_fmt, 
	AVPicture* src, enum AVPixelFormat pix_fmt, int width, int height);

void utils_compute_estfps(struct FpsState* fpsState, clock_t currTick, const int frames_used_avg);

long utils_fps2ticks(double fps);

#endif
