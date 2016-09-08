// XMLDataEditor.cpp: 主要專案檔。

#include "stdafx.h"
#include "Form1.h"

using namespace XMLDataEditor;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 


	String^l_str;
	if( args->Length )
		Application::Run(gcnew Form1(args[0]));
	else
		Application::Run(gcnew Form1(l_str));
	_CrtDumpMemoryLeaks();
	NamedTypedObject::DumpUnReleaseInfo();
	return 0;
}
