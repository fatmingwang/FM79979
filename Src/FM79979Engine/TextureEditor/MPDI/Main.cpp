#include "stdafx.h"
#include "MPDI.h"
//#include "vld.h"
#pragma comment(lib, "Vfw32.lib")
#include "../../AllLibInclude.h"

//#include "../../../include/vld.h"
//#pragma comment(lib, "../../../lib/vld.lib")
using namespace MPDI;
//
[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
//	ilInit();
//	alutInit (0,0);
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	if( args->Length )
		Application::Run(gcnew MPDIEditor(args[0]));
	else
		Application::Run(gcnew MPDIEditor(""));
//	alutExit();
//	ilShutDown();
	NamedTypedObject::DumpUnReleaseInfo();
	_CrtDumpMemoryLeaks();
	return 0;
}
