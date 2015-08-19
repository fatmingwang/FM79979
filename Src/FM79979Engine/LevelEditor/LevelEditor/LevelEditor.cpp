// LevelEditor.cpp: 主要專案檔。

#include "stdafx.h"
#include "LevelEditorForm.h"
#include "../../AllLibInclude.h"

using namespace LevelEditor;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	g_bSupportNonPowerOfTwoTexture = false;
	// 建立任何控制項之前，先啟用 Windows XP 視覺化效果
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// 建立主視窗並執行
	Application::Run(gcnew LevelEditorForm());
	_CrtDumpMemoryLeaks();
	NamedTypedObject::DumpUnReleaseInfo();	
	return 0;
}
