#ifndef SIAPLAYER_H
#define SIAPLAYER_H

#ifdef SIAPLAYER_EXPORTS
#define SIAPLAYER_API __declspec(dllexport) 
#else
#define SIAPLAYER_API __declspec(dllimport) 
#endif

extern "C" {
	#include "FrameDecodedCallback.h"
	#include "FmtOutType.h"

	enum SiaRet {
		OK,
		WRONG_PARAMS,
		FAILED_OPEN_STREAM,
		FAILED_START_THREAD
	};

	/*! Starts an H.264 decoding instance
		\param[in]  input_filename RTSP url of the H.264 flux.
		\param[in]  fps User configured FPS. Will be used only for subsampling, in other words, if this parameter 
					is higher than then stream FPS, it shall have no effect.
		\param[in]  fmt_out_type The decoded frame's format returned in the callback.
		\param[in]  frame_callback Callback that will be called with the decoded frames.
		\param[out] When the return value is SiaRet.OK, it contains the newly created decoder instance id. 
				    This pointer contains implementation specific information. Must not be deleted or de-referenced.
		\warning    This function is not thread-safe.
		\warning    Multiple calls to this function will start separate decoder instances.
	*/
	SIAPLAYER_API SiaRet __stdcall StartDecoding
		(const char* input_filename, double fps, enum FmtOutType fmt_out_type, 
		FrameDecodedCallback frame_callback, void** decoder_id);

	/*! Stops an H.264 decoding instance
		\param[out] Decoder instance id. This pointer contains implementation specific information.
					Must not be deleted or de-referenced.
		\warning    This function is not thread-safe.
	*/
	SIAPLAYER_API void __stdcall StopDecoding(void** decoder_id);

	/*! Changes the FPS for a decoder instance
		\param[in]  User configured FPS. Will be used only for subsampling, in other words, if this parameter
					is higher than then stream FPS, it shall have no effect.
		\param[in]  Decoder instance id.
	*/
	SIAPLAYER_API SiaRet __stdcall ChangeFps(double fps, void* decoder_id);
}

#endif