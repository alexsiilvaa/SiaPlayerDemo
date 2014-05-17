#include "SiaPlayerImpl.h"
#include "SiaUtils.h"
#include "VideoState.h"
#include "SiaRgbFrame.h"
#include "assert.h"

#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>

#define SAFE_OP(f, mutex) { if (SDL_LockMutex(mutex) == 0) { \
				f; \
				SDL_UnlockMutex(mutex); \
			} else { fprintf(stderr, "[%p] Couldn't lock mutex\n", mutex); \
			}}

static int retrieve_video_stream(VideoState* vs)
{
	int ret = -1;

	// The default value is too high, it may take too much time to choose the AVFormatContext
	// fields. By setting a smaller value we limit the worst-case scenario and force the system
	// to work with less information.
	vs->formatCtx->max_analyze_duration = 50000;
	if (avformat_find_stream_info(vs->formatCtx, NULL) >= 0) {
		int i;
		av_dump_format(vs->formatCtx, 0, vs->fileName, 0);		
		// Find the first video stream
		vs->videoStreamIdx = -1;
		for(i=0; i<(int)(vs->formatCtx->nb_streams); i++)
			if(AVMEDIA_TYPE_VIDEO == vs->formatCtx->streams[i]->codec->codec_type) {
			vs->videoStreamIdx=i;
			break;
		}
		if (-1!=vs->videoStreamIdx) {
			vs->codecCtx = vs->formatCtx->streams[vs->videoStreamIdx]->codec;
			ret = 0;
		} else {
			fprintf(stderr, "%s: Didn't find a video stream\n", vs->fileName);
		}
	} else {
		fprintf(stderr, "%s: Could not find stream information\n", vs->fileName);
	}
	return ret;
}

static int open_video_codec(VideoState* vs) 
{
	int ret = -1;
	// Find the decoder for the video stream
	vs->codec = avcodec_find_decoder(vs->codecCtx->codec_id);
	if(vs->codec==NULL) {
		fprintf(stderr, "Unsupported codec!\n");
	} else {
		if(avcodec_open2(vs->codecCtx, vs->codec, NULL)<0) {
			fprintf(stderr, "Could not open codec!\n");
		}
		ret = 0;
	}
	return ret;
}

static char drop_packet(struct FpsUserState* fpsUserState, clock_t currTick)
{
	char drop = 0;
	long ticks = currTick - fpsUserState->lastDecTick;
	double userFps;
	SAFE_OP(userFps = fpsUserState->userFps, fpsUserState->userFpsMutex);
	if (utils_fps2ticks(userFps) > ticks) {
		return 1;
	} 
	fpsUserState->lastDecTick = currTick;
	return 0;
}

static int notify_callback(VideoState* vs, RgbFrame* rgbFrame)
{
	uint8_t* buf = NULL;
	unsigned int size;
	int ret = 0;
	switch (vs->fmt_out_type) {
	case FMT_BGR_24BPP:
		// Convert the image from its native format to RGB
		utils_img_convert((AVPicture*)rgbFrame->frame, PIX_FMT_BGR24,
			(AVPicture*)vs->yuvFrame, vs->codecCtx->pix_fmt, vs->codecCtx->width,
			vs->codecCtx->height);
		vs->frameCallback(vs->yuvFrame->width, vs->yuvFrame->height,
			rgbFrame->frame->data[0], vs->fmt_out_type, vs->yuvFrame->width*vs->yuvFrame->height * 3,
			vs->fpsState.estFps, vs);
		break;
	case FMT_JPEG:
		HANDLE_ERROR2(size = utils_encode_jpeg(vs->codecOCtx, vs->pict_size, vs->yuvFrame, &buf),
			"Failed to encode jpeg picture");
		vs->frameCallback(vs->yuvFrame->width, vs->yuvFrame->height,
				buf, vs->fmt_out_type, size, vs->fpsState.estFps, vs);
		break;
	default:
		fprintf(stderr, "Unknown format specified: %d\n", vs->fmt_out_type);
		assert(0);
		break;
	}
fail:
	if (NULL != buf) {
		free(buf);
	}
	return ret;
}

static int video_thread(void *arg)
{
#define FRAMES_FPS_AVG 50
	AVPacket packet;
	VideoState* vs = (VideoState*)arg;
	int frameFinished;
	RgbFrame* rgbFrame = NULL;

	if (FMT_BGR_24BPP == vs->fmt_out_type) {
		rgbFrame = rgb_create_frame(vs->codecCtx->width, vs->codecCtx->height);
	}

	while(av_read_frame(vs->formatCtx, &packet)>=0) {
		// Is this a packet from the video stream?
		if(packet.stream_index==vs->videoStreamIdx) {
			// Decode video frame
			const int processedLength = 
				avcodec_decode_video2(vs->codecCtx, vs->yuvFrame, &frameFinished, &packet);
			if (0 > processedLength) {
				fprintf(stderr, "%s: Error while processing video stream", vs->fileName);
			} else { 
				if(frameFinished) {	
					clock_t currTick = clock();
					utils_compute_estfps(&vs->fpsState, currTick, FRAMES_FPS_AVG);
					if (!drop_packet(&vs->fpsUserState, currTick)) {
						notify_callback(vs, rgbFrame);						
					}
				} 
				// If frameFinished == 0, the packet didn't have enough data to read
				// one frame, we need to read another packet ... 
			}
		}
	    // Free the packet that was allocated by av_read_frame
	    av_free_packet(&packet);
	}    
	// We reached EOF 

	if (FMT_BGR_24BPP == vs->fmt_out_type) {
		rgb_delete_frame(&rgbFrame);
	}
	return 0;
}

int stream_open(const char *filename, FrameDecodedCallback frameCallback, VideoState** vs)
{
	int ret = 0;
 	*vs = vs_create(filename);
	(*vs)->frameCallback = frameCallback;
	av_register_all();
    avcodec_register_all();

	(*vs)->formatCtx = avformat_alloc_context();

	HANDLE_ERROR(avformat_open_input(&(*vs)->formatCtx, filename, NULL, NULL),filename,
		"Failed to open AVFormat\n");
	HANDLE_ERROR(retrieve_video_stream(*vs),filename,"Could not retrieve video " \
			"stream information\n");
	HANDLE_ERROR(open_video_codec(*vs),filename,"Could not open video codec " \
			"stream information\n");
	HANDLE_ERROR2(utils_init_jpegcodec(&(*vs)->codecOCtx, &(*vs)->pict_size,
		(*vs)->codecCtx->width, (*vs)->codecCtx->height, (*vs)->codecCtx->time_base, filename),
		"Could not initialise jpeg codec");

	(*vs)->yuvFrame = av_frame_alloc();
fail:
	if (0 != ret) {
		vs_delete(*vs);
		*vs = NULL;
	}
	return ret;
}

int start_video_thread(VideoState* vs)
{
	vs->videoThread = SDL_CreateThread(video_thread, vs);
	if( NULL == vs->videoThread ) {
		fprintf(stderr, "SDL_CreateThread failed: %s\n", SDL_GetError()); 
		return -1;
	} 
	return 0;
}

void stop_video_thread(VideoState* vs)
{
	SDL_KillThread(vs->videoThread);
}

void set_user_fps(double fps, VideoState* vs)
{
	SAFE_OP(vs->fpsUserState.userFps = fps, vs->fpsUserState.userFpsMutex);
}
     




