#ifndef FRAMEDECODED_CALLBACK_H
#define FRAMEDECODED_CALLBACK_H

typedef void (__stdcall* FrameDecodedCallback)
	(unsigned int width, unsigned int height, unsigned char* frame);

#endif