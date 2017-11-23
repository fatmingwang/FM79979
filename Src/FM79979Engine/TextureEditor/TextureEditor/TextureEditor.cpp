// TextureEditor.cpp: 主要專案檔。

#include "stdafx.h"
#include "Form1.h"
#include "../../AllLibInclude.h"
#pragma comment(lib, "../../../lib/opencv_core249.lib")
#pragma comment(lib, "../../../lib/opencv_highgui249.lib")
#pragma comment(lib, "../../../lib/opencv_imgproc249.lib")
#pragma comment(lib, "Vfw32.lib")
using namespace TextureEditor;

[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	// 建立任何控制項之前，先啟用 Windows XP 視覺化效果
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// 建立主視窗並執行
	Application::Run(gcnew Form1());
	_CrtDumpMemoryLeaks();
	NamedTypedObject::DumpUnReleaseInfo();
	return 0;
}