// TextureEditor.cpp: �D�n�M���ɡC

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
	// �إߥ��󱱨���e�A���ҥ� Windows XP ��ı�ƮĪG
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// �إߥD�����ð���
	Application::Run(gcnew Form1());
	_CrtDumpMemoryLeaks();
	NamedTypedObject::DumpUnReleaseInfo();
	return 0;
}