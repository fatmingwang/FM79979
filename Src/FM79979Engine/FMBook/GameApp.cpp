#include "stdafx.h"
#include "GameApp.h"


cFMBookApp*		g_pFMBookApp = 0;
cEditorMode*	g_pEditorMode = 0;
cBookMode*		g_pBookMode = 0;

#if defined(ANDROID)
cFMBookApp::cFMBookApp(int e_iMode,ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize ):cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize)
#elif defined(WIN32)
cFMBookApp::cFMBookApp(int e_iMode,HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cFMBookApp::cFMBookApp(int e_iMode,Vector2 e_vGameResolution,Vector2 e_vViewportSize)
:cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
#ifdef WIN32
	m_sbSpeedControl = true;
#endif
	m_bInitOk = false;
	m_iMode = e_iMode;
	g_pFMBookApp = this;
	//A3 l_cEditorMode22;
	//A3*l_pcEditorMode22 = new A3();
	if( m_iMode == 0 )
	{
		g_pEditorMode = new cEditorMode();
		m_pCurrentMode = g_pEditorMode;
	}
	else
	{
		g_pBookMode = new cBookMode();
		m_pCurrentMode = g_pBookMode;
	}
}

cFMBookApp::~cFMBookApp()
{
	SAFE_DELETE(g_pEditorMode);
	SAFE_DELETE(g_pBookMode);
}

void	cFMBookApp::Init()
{
	cGameApp::Init();
	cGameApp::m_sTimeAndFPS.Update();
	srand(cGameApp::m_sTimeAndFPS.uiCurrentTime);
	m_pCurrentMode->Init();
	if( this->m_iMode == 0 )
	{
	
	}
	else
	{
	
	}
	m_bInitOk = true;
}

void	cFMBookApp::Destroy()
{
	//if( this->m_iMode == 0 )
	{
		 m_spAnimationParser->Destroy();
		 m_spSoundParser->Destroy();
		 m_spPaticleManager->Destroy();
	}
	//else
	{
	
	}

}

void    cFMBookApp::SwitchMode(int e_iMode)
{

}


void	cFMBookApp::Update(float e_fElpaseTime)
{
    cGameApp::Update(e_fElpaseTime);
	m_pCurrentMode->Update(e_fElpaseTime);
}

void	cFMBookApp::Render()
{
	cGameApp::Render();
	//glViewport(0,0,(int)this->m_svViewPortSize.Width(),(int)m_svViewPortSize.Height());
	m_pCurrentMode->Render();
}

void	cFMBookApp::MouseDown(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseDown(e_iPosX,e_iPosY);
	m_pCurrentMode->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
}

void	cFMBookApp::MouseMove(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseMove(e_iPosX,e_iPosY);
	m_pCurrentMode->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
}

void	cFMBookApp::MouseUp(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseUp(e_iPosX,e_iPosY);
	m_pCurrentMode->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
}