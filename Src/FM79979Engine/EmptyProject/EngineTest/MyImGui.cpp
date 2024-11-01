#include "stdafx.h"
#include "MyImGui.h"
#include "../../imgui/imgui.h"
#include "../../imgui/ImGuiRender.h"
#include "../../imgui/MyGui/MyImGuiUIEditor.h"

cMyImGuiUIEditor* g_pMyImGuiUIEditor = nullptr;
bool g_bUseIMGUI = true;
void cMyImGuiTesting::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
	g_pMyImGuiUIEditor = new cMyImGuiUIEditor();
	//m_pMyImGuiUIEditor = new cMyImGuiUIEditor();
	//class cMyImGuiUIEditor* m_pMyImGuiUIEditor;
}

void cMyImGuiTesting::Update(float e_fElpaseTime)
{
}

void cMyImGuiTesting::Render()
{
	if (g_pMyImGuiUIEditor)
	{
		g_pMyImGuiUIEditor->Render();
	}
}

void cMyImGuiTesting::Destory()
{
	SAFE_DELETE(g_pMyImGuiUIEditor);
}