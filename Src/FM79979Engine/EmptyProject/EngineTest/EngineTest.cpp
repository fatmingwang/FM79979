// EngineTest.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"
#include "../../AllLibInclude.h"

#ifdef _DEBUG
//1.
//Visual C++ 2010: Go to View ->Property Manager, select Microsoft.Cpp.Win32.user. Select VC++ Directories and then "Include files" from the tree. Add the include subdirectory from the Visual Leak Detector installation directory. Move it to the bottom of the list. Then select "Library files" from the drop-down menu and add the lib\Win32 subdirectory from the Visual Leak Detector installation directory. Again, move it to the bottom of the list. Repeat for Microsoft.Cpp.x64.user, but select lib\Win64 subdirectory instead.
//Visual C++ 2005 and 2008: Go to Tools -> Options -> Projects and Solutions -> VC++ Directories. Select "Include files" from the "Show Directories For" drop-down menu. Add the include subdirectory from the Visual Leak Detector installation directory. Move it to the bottom of the list. Then select "Library files" from the drop-down menu and add the lib\Win32 subdirectory from the Visual Leak Detector installation directory. Again, move it to the bottom of the list.
//2.
//include the vld.h header file at main.cpp
//#include "vld.h"
#endif

cGameApp*g_pGameApp = 0;
extern UT::sTimeAndFPS g_TimeAndFPS;

using namespace EngineTest;

[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());
	NamedTypedObject::DumpUnReleaseInfo();
	_CrtDumpMemoryLeaks();
	return 0;
}