#include "stdafx.h"
#include "GameApp.h"

#include "../imgui/ImGuiRender.h"
#include "../imgui/imgui.h"

#include "GUIForFileTransfer.h"
#include "../imgui/MyGui/MyImGuiUIEditor.h"
#include "CURLUI.h"

cMyImGuiUIEditor* g_pMyImGuiUIEditor = nullptr;
cMyApp::cMyApp(HWND e_Hwnd, Vector2 e_vGameResolution, Vector2 e_vViewportSize) :cGameApp(e_Hwnd, e_vGameResolution, e_vViewportSize)
{
	this->m_sbDebugFunctionWorking = true;
	this->m_sbSpeedControl = true;
	m_bLeave = false;
#ifdef DEBUG
	this->m_sbSpeedControl = true;
#endif
	//m_p2DCamera = new cOrthogonalCamera();
	//m_pCURLUI = new cCURLUI();
	if (!m_pCURLUI)
	{
		//g_pMyImGuiUIEditor = new cMyImGuiUIEditor();
		if (!g_pMyImGuiUIEditor)
		{
			m_pGUIForFileTransfer = new cGUIForFileTransfer();
		}
		if (g_pMyImGuiUIEditor)
		{
			g_pMyImGuiUIEditor->m_p2DCamera = m_p2DCamera;
		}
		if (m_pGUIForFileTransfer)
		{
			m_pGUIForFileTransfer->m_p2DCamera = m_p2DCamera;
		}
	}
}

cMyApp::~cMyApp()
{
	SAFE_DELETE(m_pCURLUI);
	SAFE_DELETE(m_p2DCamera);
	SAFE_DELETE(m_pGUIForFileTransfer);
	SAFE_DELETE(g_pMyImGuiUIEditor);
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
	if (g_pMyImGuiUIEditor)
	{
		g_pMyImGuiUIEditor->Init();
	}
	if (m_pCURLUI)
	{
		m_pCURLUI->Init();
	}
	if (m_p2DCamera)
	{
		RECT clientRect;
		if (GetClientRect((HWND)cGameApp::m_spOpenGLRender->m_Handle, &clientRect))
		{
			
			m_p2DCamera->SetCameraPos(Vector2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top)/2);
		}
		else
		{
			m_p2DCamera->SetCameraPos(Vector2(960, 544));
		}
		
		//m_p2DCamera->SetScale(1.25);
	}
}


void	cMyApp::Update(float e_fElpaseTime)
{
    cGameApp::Update(e_fElpaseTime);
	if (m_p2DCamera)
	{
		if (m_p2DCamera)
		{
			float l_fMoveSpeed = 1.f;
			short l_cMouseWhellDelta = 0;
			if (cGameApp::m_sucKeyData[17] || cGameApp::m_sucKeyData[229])
			{
				l_cMouseWhellDelta = cGameApp::m_sMouseWhellDelta;
				l_fMoveSpeed = 3.f;
			}
			Vector2	l_vViewPort(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
			if (g_pMyImGuiUIEditor)
			{
				m_p2DCamera->CameraUpdateByMouse(cGameApp::m_sbMouseClickStatus[0], cGameApp::m_sbMouseClickStatus[1], l_cMouseWhellDelta, cGameApp::m_sMousePosition.x, cGameApp::m_sMousePosition.y, l_vViewPort, l_fMoveSpeed);
			}
			else
			if(m_pGUIForFileTransfer)
			{
				m_p2DCamera->CameraUpdateByMouse(cGameApp::m_sbMouseClickStatus[0], false, 0, cGameApp::m_sMousePosition.x, cGameApp::m_sMousePosition.y, l_vViewPort, l_fMoveSpeed);
			}
		}
		m_p2DCamera->Update(e_fElpaseTime);
	}
	if (m_pGUIForFileTransfer)
	{
		m_pGUIForFileTransfer->Update(e_fElpaseTime);
	}
	if (g_pMyImGuiUIEditor)
	{
		g_pMyImGuiUIEditor->Update(e_fElpaseTime);
	}
	if (m_pCURLUI)
	{
		m_pCURLUI->Update(e_fElpaseTime);
	}
}

void	cMyApp::Render()
{
	//this->m_spOpenGLRender->m_vGameResolution = Vector2(1920, 1080);
	this->m_spOpenGLRender->m_vBGColor = Vector4(0.5, 0.5, 0.5, 1);
	cGameApp::Render();
	auto l_ViewPortSize = Vector2(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
	if (m_p2DCamera)
	{
		m_p2DCamera->SetResolution(l_ViewPortSize);
		m_p2DCamera->Render();
		m_p2DCamera->DrawGrid(0.f,0.f,Vector4(0.5f, 1.f, 0.f, 0.3f),2.f);
	}
	if (g_pMyImGuiUIEditor)
	{
		g_pMyImGuiUIEditor->Render();
	}
	if (m_pCURLUI)
	{
		m_pCURLUI->Render();
	}
	if (m_pGUIForFileTransfer)
	{
		if (m_p2DCamera)
		{
			//Vector2 l_vSize = l_ViewPortSize;// m_p2DCamera->GetScreenViewPortSize();
			Vector2 l_vSize = m_p2DCamera->GetScreenViewPortSize();
			m_pGUIForFileTransfer->Render(m_p2DCamera->GetProjectionMatrix(), l_vSize);
		}
		else
		{
			m_pGUIForFileTransfer->Render();
		}
		
	}
	//cMyImGuiTesting::Render();
	//GLRender::RenderRectangle(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y, cMatrix44::Identity, Vector4::Red);
	std::wstring l_strExtraInfo;
	if (m_p2DCamera)
	{
		auto l_vPos = m_p2DCamera->GetMouseWorldPos();
		l_strExtraInfo = UT::ComposeMsgByFormat(L"MouseWorldPos:%d,%d", (int)l_vPos.x, (int)l_vPos.y);
	}
	cGameApp::ShowInfo(l_strExtraInfo.c_str());
	RenderShowInfoOnScreen();
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