#include "stdafx.h"
#include "MPDINode.h"
//#include "vld.h"
#pragma comment(lib, "Vfw32.lib")
#include "../../AllLibInclude.h"
using namespace MPDINodeForm;
//
[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	//cMPDINode l_pRoot; l_pRoot.SetName(L"Root");
	//cMPDINode l_p1_1; l_p1_1.SetName(L"l_p1_1");
	//cMPDINode l_p1_2; l_p1_2.SetName(L"l_p1_2");
	//cMPDINode l_p1_1_1; l_p1_1_1.SetName(L"l_p1_1_1");
	//cMPDINode l_p1_1_2; l_p1_1_2.SetName(L"l_p1_1_2");
	//cMPDINode l_p1_2_1; l_p1_2_1.SetName(L"l_p2_1_1");
	//cMPDINode l_p1_2_2; l_p1_2_2.SetName(L"l_p2_1_2");
	//l_pRoot.AddChildToLast(&l_p1_1);
	//l_pRoot.AddChildToLast(&l_p1_2);
	//l_p1_1.AddChildToLast(&l_p1_1_1);
	//l_p1_1.AddChildToLast(&l_p1_1_2);
	//l_p1_2.AddChildToLast(&l_p1_2_1);
	//l_p1_2.AddChildToLast(&l_p1_2_2);
	//l_pRoot.DumpDebugInfo();
	//auto l_pClone = l_pRoot.CloneFrameWithHierarchy();
	//l_pClone->DumpDebugInfo();

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
