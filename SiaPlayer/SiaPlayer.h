#ifndef SIAPLAYER_H
#define SIAPLAYER_H

#ifdef SIAPLAYER_EXPORTS
#define SIAPLAYER_API __declspec(dllexport) 
#else
#define SIAPLAYER_API __declspec(dllimport) 
#endif

extern "C" {
	#include "FrameDecodedCallback.h"

	enum SiaRet {
		OK,
		WRONG_PARAMS,
		FAILED_OPEN_STREAM,
		FAILED_START_THREAD
	};

	/*! Starts an H.264 decoding instance
		\param[in]  RTSP url of the H.264 flux.
		\param[in]  Callback that will be called with the decoded frames
		\param[out] When the return value is SiaRet.OK, it contains the newly created decoder instance id. 
				    This pointer contains implementation specific information. Must not be deleted or de-referenced.
		\warning    This function is not thread-safe.
		\warning    Multiple calls to this function will start separate decoder instances.
	*/
	SIAPLAYER_API SiaRet __stdcall StartDecoding
			(const char* input_filename, FrameDecodedCallback frameCallback, void** decoder_id);

	/*! Stops an H.264 decoding instance
		\param[out] Decoder instance id. This pointer contains implementation specific information.
					Must not be deleted or de-referenced.
		\warning    This function is not thread-safe.
	*/
	SIAPLAYER_API void __stdcall StopDecoding(void** decoder_id);
}

#endif