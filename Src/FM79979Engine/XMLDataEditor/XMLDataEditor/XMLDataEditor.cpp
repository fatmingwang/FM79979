// XMLDataEditor.cpp: �D�n�M���ɡC

#include "stdafx.h"
#include "Form1.h"

using namespace XMLDataEditor;

[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	// �إߥ��󱱨���e�A���ҥ� Windows XP ��ı�ƮĪG
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// �إߥD�����ð���
	String^l_str;
	if( args->Length )
		Application::Run(gcnew Form1(args[0]));
	else
		Application::Run(gcnew Form1(l_str));
	return 0;
}
