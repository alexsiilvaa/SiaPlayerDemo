#include "VideoState.h"
#include "stdlib.h"
#include "string.h"

VideoState* vs_create(const char* fileName)
{
#define SIZE_FILENAME 128
	VideoState* vs = (VideoState*)calloc(1,sizeof(VideoState));
	vs->fileName = (char*)calloc(SIZE_FILENAME,sizeof(char));
	strncpy(vs->fileName, fileName, SIZE_FILENAME);
	vs->fpsUserState.userFpsMutex = SDL_CreateMutex();
	return vs;
}

void vs_delete(VideoState* vs)
{
	if (NULL!=vs) {
		if (NULL!=vs->fileName) {
			free(vs->fileName);
		}
		if (NULL!=vs->yuvFrame) {
			av_free(vs->yuvFrame);
		}
		if (NULL!=vs->codecCtx) {
			avcodec_close(vs->codecCtx);
		}
		if (NULL != vs->codecOCtx) {
			avcodec_close(vs->codecOCtx);
			av_free(vs->codecOCtx);
		}
		if (NULL!=vs->formatCtx) {
			avformat_close_input(&vs->formatCtx);
		}  
		if (NULL != vs->fpsUserState.userFpsMutex) {
			SDL_DestroyMutex(vs->fpsUserState.userFpsMutex);
		}
		free(vs);
	}
}
