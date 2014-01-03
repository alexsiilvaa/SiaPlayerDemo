#include "SiaPlayer.h"

extern "C" {

#include "SiaPlayerImpl2.h"

	SIAPLAYER_API SiaRet __stdcall StartDecoding(const char* input_filename)
	{
		if (stream_open2(input_filename)<0) {
			return FAILED_OPEN_STREAM;
		}
		return OK;

	/*do_init();
    VideoState* is = stream_open(input_filename, NULL);
    if (!is) {
	    av_log(NULL, AV_LOG_FATAL, "Failed to initialize VideoState!\n");
        do_exit(NULL);
	}
	return OK;
	*/
	}

}



