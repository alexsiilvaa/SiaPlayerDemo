#include "SiaPlayer.h"

extern "C" {
#include "SiaPlayerImpl.h"
}

SIAPLAYER_API SiaRet __stdcall StartDecoding(const char* input_filename)
{
    VideoState* is = stream_open(input_filename, NULL);
    if (!is) {
	    av_log(NULL, AV_LOG_FATAL, "Failed to initialize VideoState!\n");
        do_exit(NULL);
	}
	return OK;
}



