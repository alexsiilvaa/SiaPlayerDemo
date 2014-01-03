#ifndef VIDEO_STATE_H
#define VIDEO_STATE_H

#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>

#include <SDL.h>
#include <SDL_thread.h>

typedef struct 
{
	AVFormatContext* formatCtx;
	char* fileName;
	int videoStreamIdx;
	AVCodecContext* codecCtx;
	AVCodec* codec;
	AVFrame* yuvFrame; 
	SDL_Thread* videoThread;

} VideoState;

VideoState* vs_create(const char* fileName);
void vs_delete(VideoState* vs);

#endif
