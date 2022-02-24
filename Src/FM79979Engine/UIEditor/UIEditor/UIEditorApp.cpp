#include "pch.h"
#include "UIEditorApp.h"

cUIEditorApp::cUIEditorApp(HWND e_Hwnd, Vector2 e_vGameResolution, Vector2 e_vViewportSize)
:cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
{
	m_pOrthogonalCamera = nullptr;
}

cUIEditorApp::~cUIEditorApp()
{
}

void cUIEditorApp::Init()
{
	cGameApp::Init();
}

void cUIEditorApp::Update(float e_fElpaseTime)
{
	cGameApp::Update(e_fElpaseTime);
}

void cUIEditorApp::Render()
{
	cGameApp::Render();
	glEnable2D(1920, 1020);
	cGameApp::ShowInfo();
}

void cUIEditorApp::Resize()
{
}

void cUIEditorApp::MouseDown(int e_iPosX, int e_iPosY)
{
	cGameApp::MouseDown(e_iPosX, e_iPosY);
}

void cUIEditorApp::MouseMove(int e_iPosX, int e_iPosY)
{
	cGameApp::MouseMove(e_iPosX, e_iPosY);
}

void cUIEditorApp::MouseUp(int e_iPosX, int e_iPosY)
{
	cGameApp::MouseUp(e_iPosX, e_iPosY);
}

void cUIEditorApp::KeyDown(char e_char)
{
	cGameApp::KeyDown(e_char);
}

void cUIEditorApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
}
