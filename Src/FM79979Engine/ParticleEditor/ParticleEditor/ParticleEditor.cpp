// ParticalEditor.cpp: �D�n�M���ɡC

#include "stdafx.h"
#include "Form1.h"

#include "../../AllLibInclude.h"

//#pragma comment(lib, "../../../lib/Devil.lib")
//#pragma comment(lib, "../../../lib/OpenAL32.lib")
//#pragma comment(lib, "../../../lib/alut.lib")
//#pragma comment(lib, "../../../lib/ogg.lib")//I have build my self OGG lib here
//#pragma comment(lib, "../../../lib/glew32.lib") 
//#pragma comment(lib, "Psapi.lib")

using namespace ParticalEditor;

[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	Application::Run(gcnew Form1());
	NamedTypedObject::DumpUnReleaseInfo();
	_CrtDumpMemoryLeaks();
	return 0;
}
