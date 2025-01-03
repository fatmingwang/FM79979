#include "stdafx.h"
#include "MyImGui.h"
#include "../../imgui/imgui.h"
#include "../../imgui/ImGuiRender.h"

#include "../../imgui/MyGui/LazyGUIParser.h"

cLazyGUIParser* g_pLazyGUIParser = nullptr;
bool g_bUseIMGUI = true;
void cMyImGuiTesting::Init()
{
	
	FMLOG("new cLazyGUIParser");
	g_pLazyGUIParser = new cLazyGUIParser();
	FMLOG("g_pLazyGUIParser->Init");
	g_pLazyGUIParser->Init();
	FMLOG("g_pLazyGUIParser->ParseUIFile");
	g_pLazyGUIParser->ParseUIFile("CURLUI.json");
	//m_pMyImGuiUIEditor = new cMyImGuiUIEditor();
	//class cMyImGuiUIEditor* m_pMyImGuiUIEditor;
}

void cMyImGuiTesting::Update(float e_fElpaseTime)
{
	if (g_pLazyGUIParser)
	{
		g_pLazyGUIParser->Update(e_fElpaseTime);
	}
}

void cMyImGuiTesting::Render()
{
	if (g_pLazyGUIParser)
	{
		g_pLazyGUIParser->Render();
	}
}

void cMyImGuiTesting::Destory()
{
	SAFE_DELETE(g_pLazyGUIParser);
}