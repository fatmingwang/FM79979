#include "pch.h"
#include "UDPOpenCVApp.h"
#include "UDPClient.h"
#include "UDPServerCamera.h"

bool g_bBeServer = false;
cUDPServerCamera* g_pUDPServerCamera = nullptr;
#ifdef WIN32
cUDPOpenCVApp::cUDPOpenCVApp(HWND e_Hwnd, Vector2 e_vGameResolution, Vector2 e_vViewportSize) : cGameApp(e_Hwnd, e_vGameResolution, e_vViewportSize)
#else
cUDPOpenCVApp::cUDPOpenCVApp(Vector2 e_vGameResolution, Vector2 e_vViewportSize) : cGameApp(e_vGameResolution, e_vViewportSize)
#endif
{
	cGameApp::m_spOpenGLRender->m_vBGColor = Vector4(0.5f, 0.5f, 0.5f, 1.f);
	this->m_sbDebugFunctionWorking = true;
	this->m_sbSpeedControl = true;
	m_pOrthogonalCamera = nullptr;
	//m_pOrthogonalCamera = new cOrthogonalCamera();
	if(g_bBeServer)
		g_pUDPServerCamera = new cUDPServerCamera();
	else
	{
		UDPTestClientInit();
	}
}

cUDPOpenCVApp::~cUDPOpenCVApp()
{
	SAFE_DELETE(m_pOrthogonalCamera);
	Destroy();
	if (g_bBeServer)
	{
		SAFE_DELETE(g_pUDPServerCamera);
	}
	else
	{
		UDPTestClientDestroy();
	}
}

void	cUDPOpenCVApp::Init()
{
	cGameApp::Init();
	this->m_sTimeAndFPS.Update();
}


void	cUDPOpenCVApp::Update(float e_fElpaseTime)
{
	cGameApp::Update(e_fElpaseTime);
	if (g_bBeServer)
	{
	}
	else
	{
		UDPTestClientUpdate();
	}
}

void	cUDPOpenCVApp::Render()
{
	cGameApp::Render();
	//MyGLEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (m_pOrthogonalCamera)
	{
		m_pOrthogonalCamera->Render();
		if (1)
			m_pOrthogonalCamera->DrawGrid(0, 0, Vector4(0, 1, 0, 0.3f));
	}
	glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	if (g_bBeServer)
	{
	}
	else
	{
		UDPTestClientRender();
	}
	MyGlErrorTest("test");
#ifdef WIN32
#endif
}

void	cUDPOpenCVApp::MouseDown(int e_iPosX, int e_iPosY)
{
	cGameApp::MouseDown(e_iPosX, e_iPosY);
}

void	cUDPOpenCVApp::MouseMove(int e_iPosX, int e_iPosY)
{
	cGameApp::MouseMove(e_iPosX, e_iPosY);
}

void	cUDPOpenCVApp::MouseUp(int e_iPosX, int e_iPosY)
{
	cGameApp::MouseUp(e_iPosX, e_iPosY);
}

void	cUDPOpenCVApp::KeyDown(char e_char)
{
	cGameApp::KeyDown(e_char);
}
void	cUDPOpenCVApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
}