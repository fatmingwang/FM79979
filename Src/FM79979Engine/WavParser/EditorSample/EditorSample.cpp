// EditorSample.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"

//#ifdef VLD_DEBUG
//https://vld.codeplex.com/wikipage?title=Using%20Visual%20Leak%20Detector&referringTitle=Documentation
#include "../../../include/vld.h"
#pragma comment(lib, "../../../lib/vld.lib")
//#endif
#include "../../AllLibInclude.h"
using namespace EditorSample;



[STAThreadAttribute]
int main(cli::array<System::String^> ^args)
{
	char l_c = -1;
	char l_c2 = -2;
	int a = l_c;
	int a2 = l_c2;
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());
	_CrtDumpMemoryLeaks();
	NamedTypedObject::DumpUnReleaseInfo();
	return 0;
}
