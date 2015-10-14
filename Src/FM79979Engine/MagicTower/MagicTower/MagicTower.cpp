// MagicTower.cpp: 主要專案檔。

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
using namespace MagicTower;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// 建立主視窗並執行
	Application::Run(gcnew Form1());
	_CrtDumpMemoryLeaks();
	return 0;
}
