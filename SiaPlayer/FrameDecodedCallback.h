#ifndef FRAMEDECODED_CALLBACK_H
#define FRAMEDECODED_CALLBACK_H

/*! Notifies a new video frame
	\param width, height Width and Height of the video frame
	\param frame Frame bytes 
	\param frameFmt Frame's format
	\param size Number of bytes of the output frame
	\param estFps Estimated FPS of the video stream
	\param videoId Video decoder instance id
*/
typedef void (__stdcall* FrameDecodedCallback)
(unsigned int width, unsigned int height, unsigned char* frame, enum FmtOutType frameFmt,
	unsigned int size, double estFps, void* videoId);

#endif