#ifndef SIAPLAYER_IMPL2_H
#define SIAPLAYER_IMPL2_H

#include "FrameDecodedCallback.h"
#include "VideoState.h"

int stream_open(const char *filename, FrameDecodedCallback frameCallback, VideoState** vs);

int start_video_thread(VideoState* vs);

void stop_video_thread(VideoState* vs);

void set_user_fps(double fps, VideoState* vs);

#endif

