#ifdef WIN32
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "Glu32.lib")
//#pragma comment(lib, "gdi32.lib")
//#pragma comment(lib, "User32.lib")
//#pragma comment(lib, "vfw32.lib")
//#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "kernel32.lib")
//#pragma comment(lib, "user32.lib")
//#pragma comment(lib, "gdi32.lib")
//#pragma comment(lib, "winspool.lib")
//#pragma comment(lib, "comdlg32.lib")
//#pragma comment(lib, "advapi32.lib")
//#pragma comment(lib, "shell32.lib ")
//#pragma comment(lib, "ole32.lib ")
//#pragma comment(lib, "oleaut32.lib")
//#pragma comment(lib, "uuid.lib ")
//#pragma comment(lib, "odbc32.lib")
//#pragma comment(lib, "odbccp32.lib")
//#pragma comment(lib, "../../../lib/glew.lib"
#ifdef 	FFMPEG
#pragma comment(lib, "../../../lib/ffmpeg/avcodec.lib")
#pragma comment(lib, "../../../lib/ffmpeg/avdevice.lib")
#pragma comment(lib, "../../../lib/ffmpeg/avfilter.lib")
#pragma comment(lib, "../../../lib/ffmpeg/avformat.lib")
#pragma comment(lib, "../../../lib/ffmpeg/avutil.lib")
#pragma comment(lib, "../../../lib/ffmpeg/swresample.lib")
#pragma comment(lib, "../../../lib/ffmpeg/swscale.lib")
#endif
//#pragma comment(lib, "Vfw32.lib")
#pragma comment(lib, "../../../lib/Devil.lib")
#pragma comment(lib, "../../../lib/OpenAL32.lib")
#pragma comment(lib, "../../../lib/alut.lib")
#pragma comment(lib, "../../../lib/glew32.lib")
//#ifdef _OGG_
//#pragma comment(lib, "../../../lib/ogg.lib")
//#pragma comment(lib, "../../../lib/vorbis.lib")
//#endif
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Glu32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Psapi.lib")

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Urlmon.lib")
#ifdef _DEBUG
	//#pragma comment(lib, "../../../lib/Debug/Bullet.lib")
	//#pragma comment(lib, "../../../lib/Debug/Core.lib")
	//#pragma comment(lib, "../../../lib/Debug/FatmingCollada.lib")
	//#pragma comment(lib, "../../lib/Debug/ogg.lib")

#else
	#pragma comment(lib, "../../../lib/Release/Bullet.lib")
	//#pragma comment(lib, "../../../lib/Release/Core.lib")
	//#pragma comment(lib, "../../../lib/Release/FatmingCollada.lib")
	//#pragma comment(lib, "../../lib/Release/ogg.lib")
#endif
//end win32
#endif