// LevelEditor.cpp: �D�n�M���ɡC

#include "stdafx.h"
#include "LevelEditorForm.h"

using namespace LevelEditor;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// �إߥ��󱱨���e�A���ҥ� Windows XP ��ı�ƮĪG
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// �إߥD�����ð���
	Application::Run(gcnew LevelEditorForm());
	_CrtDumpMemoryLeaks();
	return 0;
}
