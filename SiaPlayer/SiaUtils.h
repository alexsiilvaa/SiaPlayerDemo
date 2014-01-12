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

/*! Initialises jpeg codec
	\param[out] codecCtx The jpeg encoding context
	\param[out] pict_size Size of the input picture in bytes
	\param[in] width
	\param[in] height
	\param[in] time_base
	\param[in] filename The path to the bitstream
	\return 0 if success, -1 if error
*/
int utils_init_jpegcodec(AVCodecContext** codecCtx, int* pict_size,
	int width, int height, AVRational time_base, const char* filename);

/*! Encodes an input frame into jpeg
	\param[in] codecCtx The jpeg encoding context
	\param[in] pict_size Size of the input picture in bytes
	\param[in] frame The input frame
	\param[out] buf the output buffer with the bitstream of encoded frame
	\return On error a negative value is returned, on success zero or the number
			of bytes used from the output buffer.
*/
int utils_encode_jpeg(AVCodecContext* codecCtx, int pict_size,
	AVFrame *frame, int8_t** buf); 

#define HANDLE_ERROR(f,filename,msg) { int err; if ((err=f)<0) { \
	print_error(filename, err); \
	fprintf(stderr, msg); \
	ret = -1; \
	goto fail; \
} }

#define HANDLE_ERROR2(f,msg) { if ((f<0)) { \
	fprintf(stderr, msg); \
	ret = -1; \
	goto fail; \
} }

#define HANDLE_ERROR3(f,msg) { if (!(f)) { \
	fprintf(stderr, msg); \
	ret = -1; \
	goto fail; \
} }


#endif
