// MagicTower.cpp: �D�n�M���ɡC

#include "stdafx.h"
#include "Form1.h"

//#pragma comment(lib, "../../../lib/unicode/Devil.lib")
//#pragma comment(lib, "../../../lib/OpenAL32.lib")
//#pragma comment(lib, "../../../lib/alut.lib")
//#pragma comment(lib, "../../../lib/ogg.lib")//I have build my self OGG lib here
//#pragma comment(lib, "../../../lib/glew32.lib")
//#pragma comment(lib, "OpenGL32.lib")
//#pragma comment(lib, "odbc32.lib")
//#pragma comment(lib, "odbccp32.lib")
//#pragma comment(lib, "User32.lib")
//#pragma comment(lib, "gdi32.lib")
//#pragma comment(lib, "Vfw32.lib")
#include "../../AllLibInclude.h"
#ifdef DEBUG
#pragma comment(lib, "../../../lib/Debug/FatmingAI.lib")
#else
#endif
using namespace MagicTower;

[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// �إߥD�����ð���
	Application::Run(gcnew Form1());
	_CrtDumpMemoryLeaks();
	return 0;
}
