#include "SiaPlayer.h"

extern "C" {

#include "SiaPlayerImpl.h"
#include "VideoState.h"

	SIAPLAYER_API SiaRet __stdcall 
		StartDecoding(const char* input_filename, double fps,
		enum FmtOutType fmt_out_type, FrameDecodedCallback frame_callback, void** decoder_id)
	{
		VideoState* vs;
		if (NULL == decoder_id || fps <= 0)
			return WRONG_PARAMS;
		if (stream_open(input_filename, frame_callback, &vs) < 0) {
			return FAILED_OPEN_STREAM;
		}
		set_user_fps(fps, vs);
		vs->fmt_out_type = fmt_out_type;
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

	SIAPLAYER_API SiaRet __stdcall ChangeFps(double fps, void* decoder_id)
	{
		if (NULL == decoder_id || fps <= 0)
			return WRONG_PARAMS;
		set_user_fps(fps, (VideoState*)decoder_id);
		return OK;
	}

}



