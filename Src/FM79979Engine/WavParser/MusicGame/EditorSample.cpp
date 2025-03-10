// EditorSample.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"

//#ifdef VLD_DEBUG
//https://vld.codeplex.com/wikipage?title=Using%20Visual%20Leak%20Detector&referringTitle=Documentation
#include "C:/Program Files (x86)/Visual Leak Detector/include/vld.h"
#pragma comment(lib, "C:/Program Files (x86)/Visual Leak Detector/lib/Win32/vld.lib")
//#endif

using namespace EditorSample;



[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());
	_CrtDumpMemoryLeaks();
	NamedTypedObject::DumpUnReleaseInfo();
	return 0;
}
