// LevelEditor.cpp: �D�n�M���ɡC

#include "stdafx.h"
#include "LevelEditorForm.h"
#include "../../AllLibInclude.h"

using namespace LevelEditor;

[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	g_bSupportNonPowerOfTwoTexture = false;
	// �إߥ��󱱨���e�A���ҥ� Windows XP ��ı�ƮĪG
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// �إߥD�����ð���
	Application::Run(gcnew LevelEditorForm());
	_CrtDumpMemoryLeaks();
	NamedTypedObject::DumpUnReleaseInfo();	
	return 0;
}
