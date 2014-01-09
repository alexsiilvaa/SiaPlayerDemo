#ifndef VIDEO_STATE_H
#define VIDEO_STATE_H

#include "FrameDecodedCallback.h"

#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>

#include <SDL.h>
#include <SDL_thread.h>

struct FpsState
{
	long cumulTicks;
	clock_t lastTick;
	long cumulFrames;
	double estFps;
};

struct FpsUserState
{
	long lastDecTick;
	SDL_mutex* userFpsMutex;
	double userFps;
};

typedef struct 
{
	AVFormatContext* formatCtx;
	char* fileName;
	int videoStreamIdx;
	AVCodecContext* codecCtx;
	AVCodec* codec;
	AVFrame* yuvFrame; 
	SDL_Thread* videoThread;
	FrameDecodedCallback frameCallback;
	struct FpsState fpsState;
	struct FpsUserState fpsUserState;


} VideoState;

VideoState* vs_create(const char* fileName);
void vs_delete(VideoState* vs);

#endif
