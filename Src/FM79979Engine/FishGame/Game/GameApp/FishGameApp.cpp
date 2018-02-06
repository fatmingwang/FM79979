#include "stdafx.h"
#include "FishGameApp.h"

extern void	LoadSample();
extern void	DestorySampleObject();
extern void	SampleUpdate(float e_fElpaseTime);
extern void	SampleRender();
extern void	SampleMouseDown(int e_iPosX,int e_iPosY);
extern void	SampleMouseMove(int e_iPosX,int e_iPosY);
extern void	SampleMouseUp(int e_iPosX,int e_iPosY);
extern void	SampleKeyup(char e_cKey);

#if defined(ANDROID)
cFishGameApp::cFishGameApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil ):cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize,e_pNvEGLUtil)
#elif defined(WIN32)
cFishGameApp::cFishGameApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cFishGameApp::cFishGameApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
	this->m_sbDebugFunctionWorking = true;
	this->m_sbSpeedControl = true;
	m_bLeave = false;
#ifdef DEBUG
	this->m_sbSpeedControl = true;
#endif
}

cFishGameApp::~cFishGameApp()
{
	DestorySampleObject();
	Destroy();
}

void	cFishGameApp::Init()
{
	cGameApp::Init();
	LoadSample();
	//let first update is not too big
	this->m_sTimeAndFPS.Update();
}

void	cFishGameApp::Update(float e_fElpaseTime)
{
    cGameApp::Update(e_fElpaseTime);
	SampleUpdate(e_fElpaseTime);
}

void	cFishGameApp::Render()
{
	glEnable(GL_ALPHA_TEST);
	MyGLEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	cGameApp::Render();
	SampleRender();
	cGameApp::ShowInfo();
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
}

void	cFishGameApp::MouseDown(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseDown(e_iPosX,e_iPosY);
	SampleMouseDown(e_iPosX,e_iPosY);
}

void	cFishGameApp::MouseMove(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseMove(e_iPosX,e_iPosY);
	SampleMouseMove(e_iPosX,e_iPosY);
}

void	cFishGameApp::MouseUp(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseUp(e_iPosX,e_iPosY);
	SampleMouseUp(e_iPosX,e_iPosY);
}

void	cFishGameApp::KeyDown(char e_char)
{
	 cGameApp::KeyDown(e_char);
}
void	cFishGameApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
	SampleKeyup(e_char);
}

bool	cFishGameApp::InitOnceObject()
{

}
bool	cFishGameApp::DeleteOnceObject()
{

}