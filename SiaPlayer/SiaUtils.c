#include "SiaUtils.h"
#include "libavutil\log.h" 
#include "libavutil\error.h" 
#include "libswscale\swscale.h"
#include "libavcodec\avcodec.h"
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

int utils_init_jpegcodec(AVCodecContext** codecCtx, int* pict_size,
	int width, int height, AVRational time_base, const char* filename)
{
	int ret = 0;
	AVCodecContext* pCodecCtx;
	AVCodec* pCodec;

	*pict_size = avpicture_get_size(PIX_FMT_YUVJ420P, width, height);
	HANDLE_ERROR3(pCodec = avcodec_find_encoder(AV_CODEC_ID_MJPEG), "Codec not found\n")
	HANDLE_ERROR3(*codecCtx = avcodec_alloc_context3(pCodec), "Could not allocate video codec context\n")

	pCodecCtx = (*codecCtx);
	pCodecCtx->width = width;
	pCodecCtx->height = height;
	pCodecCtx->time_base = time_base;
	pCodecCtx->pix_fmt = PIX_FMT_YUVJ420P; // planar YUV 4:2:0, 12bpp, full scale (JPEG)
	pCodecCtx->mb_lmin = pCodecCtx->lmin = pCodecCtx->qmin * FF_QP2LAMBDA;
	pCodecCtx->mb_lmax = pCodecCtx->lmax = pCodecCtx->qmax * FF_QP2LAMBDA;
	pCodecCtx->flags = CODEC_FLAG_QSCALE;
	pCodecCtx->global_quality = pCodecCtx->qmin * FF_QP2LAMBDA;

	HANDLE_ERROR(avcodec_open2(pCodecCtx, pCodec, NULL), filename,
		"Failed to open jpeg codec")
fail:
	return ret;
}

int utils_encode_jpeg(AVCodecContext* codecCtx, int pict_size, 
	AVFrame *frame, int8_t** buf)
{
	//FILE                   *JPEGFile;
	int ret = -1;
	
	*buf = (uint8_t *)malloc(pict_size);
	if (*buf == NULL)
		return (-1);
	memset(*buf, 0, pict_size);
	
	ret = avcodec_encode_video(
		codecCtx, *buf, pict_size, frame);

	/*	JPEGFile = fopen("test.jpg", "wb");
	fwrite(*buf, 1, ret, JPEGFile);
	fclose(JPEGFile); */

	return ret;
}




