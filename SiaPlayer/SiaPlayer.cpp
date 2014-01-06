#include "SiaPlayer.h"

extern "C" {

#include "SiaPlayerImpl.h"
#include "VideoState.h"

	SIAPLAYER_API SiaRet __stdcall 
		StartDecoding(const char* input_filename, FrameDecodedCallback frameCallback)
	{
		VideoState* vs;
		if (stream_open(input_filename, frameCallback, &vs) < 0) {
			return FAILED_OPEN_STREAM;
		}
		if (start_video_thread(vs) < 0) {
			vs_delete(vs);
			return FAILED_START_THREAD;
		}
		return OK;
	}

}



