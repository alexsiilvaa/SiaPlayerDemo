#include "SiaUtils.h"
#include "libavutil\log.h" 
#include "libavutil\error.h" 
#include <libswscale\swscale.h>
#include <string.h>

void print_error(const char *filename, int err)
{
    char errbuf[128];
    const char *errbuf_ptr = errbuf;
    if (av_strerror(err, errbuf, sizeof(errbuf)) < 0)
        errbuf_ptr = strerror(AVUNERROR(err));
    av_log(NULL, AV_LOG_ERROR, "%s: %s\n", filename, errbuf_ptr);
}

int utils_img_convert(AVPicture* dst, enum AVPixelFormat dst_pix_fmt, 
	AVPicture* src, enum AVPixelFormat pix_fmt, int width, int height)
{
   struct SwsContext *img_convert_ctx = sws_getContext(width, height, pix_fmt, width, height, dst_pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);
   int result = sws_scale(img_convert_ctx, src->data, src->linesize, 0, height, dst->data, dst->linesize);
   sws_freeContext(img_convert_ctx);
   return result;
}

void utils_compute_estfps(struct FpsState* fpsState, clock_t currTick, const int frames_used_avg)
{
	long diff = currTick - fpsState->lastTick;
	fpsState->lastTick = currTick;
	fpsState->cumulTicks += diff;
	fpsState->cumulFrames++;
	if (frames_used_avg == fpsState->cumulFrames) {
		fpsState->estFps = (double)fpsState->cumulFrames / fpsState->cumulTicks * CLOCKS_PER_SEC;
		fpsState->cumulTicks = 0;
		fpsState->cumulFrames = 0;
	}
}

long utils_fps2ticks(double fps) 
{
	return (long)((1. / fps) * CLOCKS_PER_SEC);
}



