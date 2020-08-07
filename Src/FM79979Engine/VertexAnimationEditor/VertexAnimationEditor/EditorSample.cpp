// EditorSample.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"
#include "MyAVIConvert.h"
#include "../../AllLibInclude.h"
//#pragma comment(lib, "../../../lib/opencv_core249.lib")
//#pragma comment(lib, "../../../lib/opencv_highgui249.lib")
//#pragma comment(lib, "../../../lib/opencv_imgproc249.lib")


using namespace EditorSample;
using namespace VertexAnimationEditor;

[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	if( args->Length == 0 )
		Application::Run(gcnew Form1());
	else
		Application::Run(gcnew MyAVIConvert());
	_CrtDumpMemoryLeaks();
	NamedTypedObject::DumpUnReleaseInfo();
	return 0;
}
