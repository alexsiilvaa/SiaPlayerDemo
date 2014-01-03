#include "SiaPlayerImpl2.h"
#include "SiaUtils.h"
#include "VideoState.h"

#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>

#define HANDLE_ERROR(f,filename,msg) if ((f)<0) { \
			fprintf(stderr, msg, filename); \
			ret = -1; \
			goto fail; \
		}

static int retrieve_video_stream(VideoState* vs)
{
	int ret = -1;
	if(av_find_stream_info(vs->formatCtx)>=0) {
		int i, videoStream;
		AVCodecContext *pCodecCtx;
		av_dump_format(vs->formatCtx, 0, vs->fileName, 0);		
		// Find the first video stream
		vs->videoStreamIdx = -1;
		for(i=0; i<(vs->formatCtx->nb_streams); i++)
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

static int video_thread(void *arg)
{
	AVPacket packet;
	VideoState* vs = (VideoState*)arg;
	int i = 0, frameFinished;
	while(av_read_frame(vs->formatCtx, &packet)>=0) {
		// Is this a packet from the video stream?
		if(packet.stream_index==vs->videoStreamIdx) {
			// Decode video frame
			avcodec_decode_video2(vs->codecCtx, vs->yuvFrame, &frameFinished, &packet);
			// Did we get a video frame?
			if(frameFinished) {
			  int a =0;
			}
		}
	    // Free the packet that was allocated by av_read_frame
	    av_free_packet(&packet);
	}    
}

int stream_open2(const char *filename)
{
	VideoState* vs = vs_create(filename);
	int ret = -1, err, i;

	av_register_all();

	vs->formatCtx = avformat_alloc_context();
	err = avformat_open_input(&vs->formatCtx, filename, NULL, NULL);
    if (err < 0) {
        print_error(filename, err);
        ret = -1;
        goto fail;
    }
	
	HANDLE_ERROR(retrieve_video_stream(vs),filename,"%s: Could not retrieve video\n" \
			"stream information\n");
	HANDLE_ERROR(open_video_codec(vs),filename,"%s: Could not open video codec\n" \
			"stream information\n");

	vs->yuvFrame = avcodec_alloc_frame();
	ret = 0;

	vs->videoThread = SDL_CreateThread(video_thread, vs);

fail:
	return ret;
}


