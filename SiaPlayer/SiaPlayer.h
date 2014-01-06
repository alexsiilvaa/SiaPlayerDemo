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
		FAILED_OPEN_STREAM,
		FAILED_START_THREAD
	};

	SIAPLAYER_API SiaRet __stdcall StartDecoding
			(const char* input_filename, FrameDecodedCallback frameCallback);
}

#endif