#include "stdafx.h"
#include "MusicGameApp.h"
#include "PerformMusicPhase.h"

cMusicGameApp::
#if defined(ANDROID)
cMusicGameApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil)
:cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize,e_pNvEGLUtil)
#elif defined(WIN32)
cMusicGameApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cMusicGameApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
	SetAcceptRationWithGameresolution((int)e_vViewportSize.x,(int)e_vViewportSize.y,(int)e_vGameResolution.x,(int)e_vGameResolution.y);
	m_pPhaseManager = nullptr;
}

cMusicGameApp::~cMusicGameApp()
{
	SAFE_DELETE(m_pPhaseManager);
}

void	cMusicGameApp::Init()
{
	cGameApp::Init();
	m_pPhaseManager = new cPhaseManager();
	if( m_pPhaseManager )
	{
		cPerformMusicPhase*l_pPerformMusicPhase = new cPerformMusicPhase();
		m_pPhaseManager->AddObjectNeglectExist(l_pPerformMusicPhase);





		//
		m_pPhaseManager->SetCurrentCurrentPhase(l_pPerformMusicPhase->GetName());
	}
	cGameApp::m_sTimeAndFPS.Update();
}


void	cMusicGameApp::Update(float e_fElpaseTime)
{
	cGameApp::Update(e_fElpaseTime);
	if( m_pPhaseManager )
	{
		m_pPhaseManager->Update(e_fElpaseTime);
	}
}

void	cMusicGameApp::Render()
{
	cGameApp::Render();
	if( m_pPhaseManager )
	{
		m_pPhaseManager->Render();
	}
}


void	cMusicGameApp::MouseDown(int e_iPosX,int e_iPosY)
{
	cGameApp::MouseDown(e_iPosX,e_iPosY);
	if( m_pPhaseManager )
	{
		m_pPhaseManager->MouseDown(e_iPosX,e_iPosY);
	}
}

void	cMusicGameApp::MouseMove(int e_iPosX,int e_iPosY)
{
	cGameApp::MouseMove(e_iPosX,e_iPosY);
	if( m_pPhaseManager )
	{
		m_pPhaseManager->MouseMove(e_iPosX,e_iPosY);
	}
}

void	cMusicGameApp::MouseUp(int e_iPosX,int e_iPosY)
{
	cGameApp::MouseUp(e_iPosX,e_iPosY);
	if( m_pPhaseManager )
	{
		m_pPhaseManager->MouseUp(e_iPosX,e_iPosY);
	}
}