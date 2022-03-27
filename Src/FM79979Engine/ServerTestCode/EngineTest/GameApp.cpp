#include "stdafx.h"
#include "GameApp.h"
#include "NetworkSample.h"

cNetworkSample* g_pNetworkSample = nullptr;
#if defined(ANDROID)
cEngineTestApp::cEngineTestApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil ):cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize,e_pNvEGLUtil)
#elif defined(WIN32)
cEngineTestApp::cEngineTestApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cEngineTestApp::cEngineTestApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
	this->m_sbDebugFunctionWorking = true;
	this->m_sbSpeedControl = true;
	m_bLeave = false;
	m_pPhaseManager = new cPhaseManager();	
	g_pNetworkSample = new cNetworkSample();
#ifdef DEBUG
	this->m_sbSpeedControl = true;
#endif
}

cEngineTestApp::~cEngineTestApp()
{
	SAFE_DELETE(m_pPhaseManager);
	Destroy();
}

void	cEngineTestApp::Init()
{
	g_pNetworkSample->Init();
	cGameApp::Init();

	//let first update is not too big
	this->m_sTimeAndFPS.Update();
}

void	cEngineTestApp::Update(float e_fElpaseTime)
{
	g_pNetworkSample->Update(e_fElpaseTime);
    cGameApp::Update(e_fElpaseTime);

	this->m_pPhaseManager->Update(e_fElpaseTime);
}

void	cEngineTestApp::Render()
{
	//glEnable(GL_ALPHA_TEST);
	MyGLEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	cGameApp::Render();

	//this->m_pPhaseManager->Render();
	cGameApp::ShowInfo();
	//this->m_pPhaseManager->DebugRender();

#ifdef WIN32
	SwapBuffers(cGameApp::m_spOpenGLRender->m_Hdc);
#endif
}

void	cEngineTestApp::MouseDown(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseDown(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);

}

void	cEngineTestApp::MouseMove(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseMove(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);

}

void	cEngineTestApp::MouseUp(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseUp(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);

}

void	cEngineTestApp::KeyDown(char e_char)
{
	 cGameApp::KeyDown(e_char);
}
void	cEngineTestApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);

}