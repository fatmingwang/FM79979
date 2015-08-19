// Sound.cpp : main project file.

#include "stdafx.h"
#include "Sound.h"
#include "../../AllLibInclude.h"
using namespace Sound;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	alutInit (0,0);
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Sound::SoundObject());
	alutExit();
	_CrtDumpMemoryLeaks();
	NamedTypedObject::DumpUnReleaseInfo();
	return 0;
}
