#ifndef SIAPLAYER_H
#define SIAPLAYER_H

#ifdef SIAPLAYER_EXPORTS
#define SIAPLAYER_API __declspec(dllexport) 
#else
#define SIAPLAYER_API __declspec(dllimport) 
#endif

enum SiaRet {
	OK,
	FAILED_OPEN_STREAM
};

SIAPLAYER_API SiaRet __stdcall StartDecoding(const char* input_filename);

#endif