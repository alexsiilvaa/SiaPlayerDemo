#include "SiaPlayer.h"

extern "C" {

#include "SiaPlayerImpl.h"
#include "VideoState.h"

	SIAPLAYER_API SiaRet __stdcall 
		StartDecoding(const char* input_filename, FrameDecodedCallback frameCallback, void** decoder_id)
	{
		VideoState* vs;
		if (NULL == decoder_id)
			return WRONG_PARAMS;
		if (stream_open(input_filename, frameCallback, &vs) < 0) {
			return FAILED_OPEN_STREAM;
		}
		if (start_video_thread(vs) < 0) {
			vs_delete(vs);
			return FAILED_START_THREAD;
		}
		*decoder_id = vs;
		return OK;
	}

	SIAPLAYER_API void _stdcall
		StopDecoding(void** decoder_id)
	{
			if (NULL != decoder_id) {
				VideoState* vs = (VideoState*)*decoder_id;
				if (NULL != vs) {
					stop_video_thread(vs);
					vs_delete(vs);
					*decoder_id = NULL;
				}
			}
	}
}



