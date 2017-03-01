/**********************************************************************************************
	* Author:	Oso, Oluwafemi Ebenezer
	* Date:		13th of September, 2014
	* Filename:	oggHelper.h

	A wrapper for the OGG Vorbis library
***********************************************************************************************/
#include <stdio.h>
#include <time.h> //Needed just for the time() function
#include "vorbis/vorbisenc.h"
#include "vorbis/codec.h"

#include <math.h> //Needed for the floor function in decode

#include "WaveFileHeader.h"
#include "OggHelper_VorbisSettings.h"


class oggHelper; //oggHelper prototype, needed because of struct StaticParam_t

//Use to hold parameters for the thread(async) function
struct OGG_StaticParam_t
{
	char* pcm;
	char* ogg;
	EncodeSetting settings;
	VorbisComment comments;
	WNDPROC callback_proc;
	oggHelper* oggObj;
};


class oggHelper
{
private:
	char ErrMsg[1000];
	void ErrorReport(char* ErrMsg);
	
	#define READ 4096

	#define OH_STARTED	WM_USER+1
	#define OH_COMPUTED	WM_USER+2
	#define OH_DONE		WM_USER+3
	#define OH_ERROR	WM_USER+4

	#define MAX_THREAD_COUNT 5
	HANDLE hThread[MAX_THREAD_COUNT];
	OGG_StaticParam_t* hSParam[MAX_THREAD_COUNT];

	//The static member from which we can start the thread
	static int encode_s(void* param);
	static int decode_s(void* param);

	//The actual encode function
	BOOL encode_x(char* file_in, char* file_out, EncodeSetting es, VorbisComment ivc, WNDPROC callbackproc);
	//The actual decode function
	BOOL decode_x(char* file_in, char* file_out, WNDPROC callbackproc);
public:
	oggHelper();
	~oggHelper();

	//Encode PCM to OGG
	BOOL Encode(char* file_in, char* file_out);
	BOOL Encode(char* file_in, char* file_out, EncodeSetting es);
	BOOL Encode(char* file_in, char* file_out, EncodeSetting es, VorbisComment ivc);
	BOOL Encode(char* file_in, char* file_out, EncodeSetting es, VorbisComment ivc, WNDPROC callbackproc);
	void* Encode(char* file_in, char* file_out, EncodeSetting es, VorbisComment ivc, WNDPROC callbackproc, BOOL async);

	//Decode OGG to PCM (with a WAVE header)
	BOOL Decode(char* file_in, char* file_out);
	BOOL Decode(char* file_in, char* file_out, WNDPROC callbackproc);
	void* Decode(char* file_in, char* file_out, WNDPROC callbackproc, BOOL async);
};