#ifndef SIA_RGBFRAME_H
#define SIA_RGBFRAME_H

#include <libavformat\avformat.h>

typedef struct
{
	AVFrame* frame; /* The RGB picture */
	uint8_t* buffer; /* Buffer where the image data is stored */
} RgbFrame;

RgbFrame* rgb_create_frame(int width, int height);
void rgb_delete_frame(RgbFrame** rgbFrame);

#endif
