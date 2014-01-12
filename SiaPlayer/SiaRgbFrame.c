#include "SiaRgbFrame.h"
#include "SiaUtils.h"
#include "stdlib.h"
#include "assert.h"

#define CHECK_FAIL(condition) { if ((condition)) { \
	fprintf(stderr, "Failure at [%s] on line [%d]\n", __FILE__, __LINE__); \
	rgb_delete_frame(&rgbFrame); \
	goto fail; \
} }

#define CHECK_FAIL2(condition, errorCode) { \
	print_error2(errorCode); \
	CHECK_FAIL(condition) \
}

RgbFrame* rgb_create_frame(int width, int height)
{
	int numBytes;
	RgbFrame* rgbFrame = (RgbFrame*)calloc(1, sizeof(RgbFrame));
	assert(NULL != rgbFrame);
	rgbFrame->frame = av_frame_alloc();
	CHECK_FAIL(NULL == rgbFrame->frame);
	// Determine required buffer size and allocate buffer
	numBytes = avpicture_get_size(PIX_FMT_RGB24, width, height);
	CHECK_FAIL2(0 > numBytes, numBytes);
	rgbFrame->buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
	assert(NULL != rgbFrame->buffer);
	numBytes = avpicture_fill((AVPicture *)rgbFrame->frame, rgbFrame->buffer, 
		PIX_FMT_BGR24, width, height);
	CHECK_FAIL2(0 > numBytes, numBytes);
fail:
	return rgbFrame;
}
	
void rgb_delete_frame(RgbFrame** rgbFrame)
{
	if (NULL != *rgbFrame) {
		if (NULL != (*rgbFrame)->buffer) {
			av_free((*rgbFrame)->buffer);
		}
		if (NULL != (*rgbFrame)->frame) {
			av_frame_free(&(*rgbFrame)->frame);
		}
		free(*rgbFrame);
		*rgbFrame = NULL;
	}
}


