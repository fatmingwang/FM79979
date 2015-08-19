#include "stdafx.h"
#include "GameApp.h"
#include "TestPhase.h"

extern void	LoadSample();
extern void	DestorySampleObject();
extern void	SampleUpdate(float e_fElpaseTime);
extern void	SampleRender();
extern void	SampleMouseDown(int e_iPosX,int e_iPosY);
extern void	SampleMouseMove(int e_iPosX,int e_iPosY);
extern void	SampleMouseUp(int e_iPosX,int e_iPosY);
extern void	SampleKeyup(char e_cKey);

#if defined(ANDROID)
cEngineTestApp::cEngineTestApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil ):cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize,e_pNvEGLUtil)
#elif defined(WIN32)
cEngineTestApp::cEngineTestApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cEngineTestApp::cEngineTestApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
	m_bLeave = false;
	m_pPhaseManager = new cPhaseManager();	
#ifdef DEBUG
	this->m_sbSpeedControl = true;
#endif
}

cEngineTestApp::~cEngineTestApp()
{
	SAFE_DELETE(m_pPhaseManager);
	DestorySampleObject();
	Destroy();
}

void	cEngineTestApp::Init()
{
	cGameApp::Init();
	LoadSample();

	cGamePhase*l_pGamePhase = new cGamePhase();
	l_pGamePhase->SetName(L"Test");
	bool	l_b = this->m_pPhaseManager->AddObject(l_pGamePhase);
	if( !l_b )
	{
		assert(0&&"this phase has been added!");
	}
	//let first update is not too big
	this->m_sTimeAndFPS.Update();
}

void	cEngineTestApp::Update(float e_fElpaseTime)
{
    cGameApp::Update(e_fElpaseTime);
	SampleUpdate(e_fElpaseTime);
	this->m_pPhaseManager->Update(e_fElpaseTime);
}

void	cEngineTestApp::Render()
{
	glViewport(0,0,(int)m_svViewPortSize.Width(),(int)m_svViewPortSize.Height());
	//glScissor(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor( 0,0.5,0.5,1 );
	glClearDepth(1.0f);	
	float	l_fVP[16];
	UseShaderProgram();
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,l_fVP);
#ifndef ANDROID
	glEnable(GL_TEXTURE_2D);
#endif
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glAlphaFunc(GL_GREATER,0.001f);	
	cGameApp::Render();
	SampleRender();
	this->m_pPhaseManager->Render();
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,l_fVP);
	if( cGameApp::m_spGlyphFontRender )
	{
	    WCHAR   l_str[MAX_PATH];
	    swprintf(l_str,MAX_PATH,L"MousePos:%d,%d",cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	    cGameApp::m_spGlyphFontRender->RenderFont(0,0,l_str);
	    cGameApp::m_spGlyphFontRender->RenderFont(0,20,UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()));
	}
	glDisable(GL_TEXTURE_2D);
	this->m_pPhaseManager->DebugRender();
	glDisable2D();
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
}

void	cEngineTestApp::MouseDown(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseDown(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	SampleMouseDown(e_iPosX,e_iPosY);
}

void	cEngineTestApp::MouseMove(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseMove(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	SampleMouseMove(e_iPosX,e_iPosY);
}

void	cEngineTestApp::MouseUp(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseUp(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	SampleMouseUp(e_iPosX,e_iPosY);
}

void	cEngineTestApp::KeyDown(char e_char)
{
	 cGameApp::KeyDown(e_char);
}
void	cEngineTestApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
	SampleKeyup(e_char);
}