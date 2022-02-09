#ifdef WIN32
#ifdef 	FFMPEG
#pragma comment(lib, "../../../lib/ffmpeg/avcodec.lib")
#pragma comment(lib, "../../../lib/ffmpeg/avdevice.lib")
#pragma comment(lib, "../../../lib/ffmpeg/avfilter.lib")
#pragma comment(lib, "../../../lib/ffmpeg/avformat.lib")
#pragma comment(lib, "../../../lib/ffmpeg/avutil.lib")
#pragma comment(lib, "../../../lib/ffmpeg/swresample.lib")
#pragma comment(lib, "../../../lib/ffmpeg/swscale.lib")
#endif
#pragma comment(lib, "../../../lib/OpenAL32.lib")
#pragma comment(lib, "../../../lib/alut.lib")
#ifndef UWP
//#ifdef USE_OPEN_CV
#ifdef DEBUG
#pragma comment(lib, "../../../lib/opencv_world440d")
#else
#pragma comment(lib, "../../../lib/opencv_world440")
#endif
//#endif
#pragma comment(lib, "../../../lib/glew.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Glu32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Psapi.lib")

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Urlmon.lib")
#endif
#ifdef _DEBUG
#ifdef USE_MU_PARSER
	#pragma comment(lib, "../../../lib/Debug/MathParser.lib")
#endif
	#pragma comment(lib, "../../../lib/Debug/Bullet.lib")
#ifndef UWP
	#pragma comment(lib, "../../../lib/Debug/Core.lib")
#endif
	#pragma comment(lib, "../../../lib/Debug/Freetype.lib")
	#pragma comment(lib, "../../../lib/Debug/FatmingCollada.lib")
	#pragma comment(lib, "../../../lib/Debug/ogg.lib")

#else
#ifdef USE_MU_PARSER
	#pragma comment(lib, "../../../lib/Release/MathParser.lib")
#endif
	#pragma comment(lib, "../../../lib/Release/Bullet.lib")
#ifndef UWP
	#pragma comment(lib, "../../../lib/Release/Core.lib")
#endif
	#pragma comment(lib, "../../../lib/Release/FatmingCollada.lib")
	#pragma comment(lib, "../../../lib/Release/ogg.lib")
	#pragma comment(lib, "../../../lib/Release/Freetype.lib")
#endif
//end win32
#endif