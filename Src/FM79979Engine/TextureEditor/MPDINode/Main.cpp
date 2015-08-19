#include "stdafx.h"
#include "MPDINode.h"
//#include "vld.h"
#pragma comment(lib, "Vfw32.lib")
#include "../../AllLibInclude.h"
using namespace MPDINodeForm;
//
[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
//	ilInit();
//	alutInit (0,0);
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew MPDINodeEditor());
	//if( args->Length )
	//	Application::Run(gcnew MPDINodeEditor(args[0]));
	//else
	//	Application::Run(gcnew MPDINodeEditor(""));
//	alutExit();
//	ilShutDown();
	NamedTypedObject::DumpUnReleaseInfo();
	_CrtDumpMemoryLeaks();
	return 0;
}
