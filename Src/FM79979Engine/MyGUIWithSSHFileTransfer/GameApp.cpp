#include "stdafx.h"
#include "GameApp.h"

#include "../imgui/ImGuiRender.h"
#include "../imgui/imgui.h"

#include "GUIForFileTransfer.h"


cMyApp::cMyApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
{
	this->m_sbDebugFunctionWorking = true;
	this->m_sbSpeedControl = true;
	m_bLeave = false;
#ifdef DEBUG
	this->m_sbSpeedControl = true;
#endif
	//cMyImGuiTesting::Init();
	m_pGUIForFileTransfer = new cGUIForFileTransfer();
}

cMyApp::~cMyApp()
{
	SAFE_DELETE(m_pGUIForFileTransfer);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	Destroy();
}

void	cMyApp::Init()
{
	cGameApp::Init();
	//let first update is not too big
	this->m_sTimeAndFPS.Update();
	if (m_pGUIForFileTransfer)
	{
		m_pGUIForFileTransfer->Init();
	}
}

void	cMyApp::Update(float e_fElpaseTime)
{
    cGameApp::Update(e_fElpaseTime);
	if (m_pGUIForFileTransfer)
	{
		m_pGUIForFileTransfer->Update(e_fElpaseTime);
	}
}

void	cMyApp::Render()
{
	this->m_spOpenGLRender->m_vGameResolution = Vector2(1280, 720);
	this->m_spOpenGLRender->m_vBGColor = Vector4(0.5, 0.5, 0.5, 1);
	cGameApp::Render();
	if (m_pGUIForFileTransfer)
	{
		m_pGUIForFileTransfer->Render();
	}
	//cMyImGuiTesting::Render();
	GLRender::RenderRectangle(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y, cMatrix44::Identity, Vector4::Red);
	cGameApp::ShowInfo();
#ifdef WIN32
	SwapBuffers(cGameApp::m_spOpenGLRender->m_Hdc);
#endif
}

void	cMyApp::MouseDown(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseDown(e_iPosX,e_iPosY);
}

void	cMyApp::MouseMove(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseMove(e_iPosX,e_iPosY);
}

void	cMyApp::MouseUp(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseUp(e_iPosX,e_iPosY);
}

void	cMyApp::KeyDown(char e_char)
{
	 cGameApp::KeyDown(e_char);
}
void	cMyApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
}