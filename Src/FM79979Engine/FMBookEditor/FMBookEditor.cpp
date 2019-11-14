// FMBookEditor.cpp

#include "stdafx.h"
#include "Form1.h"
//#ifdef _OGG_
#pragma comment(lib, "../../lib/alut.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "../../lib/glew.lib")
#pragma comment(lib, "../../lib/OpenAL32.lib")
#pragma comment(lib, "../../lib/vorbis.lib")
//#endif
#pragma comment(lib, "Glu32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Psapi.lib")

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Urlmon.lib")
#ifdef _DEBUG
	#pragma comment(lib, "../../lib/Debug/Bullet.lib")
	#pragma comment(lib, "../../lib/Debug/Core.lib")
	#pragma comment(lib, "../../lib/Debug/FatmingCollada.lib")
	#pragma comment(lib, "../../lib/Debug/ogg.lib")
	#pragma comment(lib, "../../lib/Debug/FMEvent.lib")
#else
	#pragma comment(lib, "../../lib/Release/Bullet.lib")
	#pragma comment(lib, "../../lib/Release/Core.lib")
	#pragma comment(lib, "../../lib/Release/FatmingCollada.lib")
	#pragma comment(lib, "../../lib/Release/ogg.lib")
	#pragma comment(lib, "../../lib/Release/FMEvent.lib")
#endif
using namespace FMBookEditor;

[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	cGameApp::m_sbDebugFunctionWorking = true;
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 


	Application::Run(gcnew Form1());
	_CrtDumpMemoryLeaks();
	NamedTypedObject::DumpUnReleaseInfo();
	return 0;
}
