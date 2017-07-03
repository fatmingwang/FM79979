#include "stdafx.h"
#include "MusicGameApp.h"
#include "PerformMusicPhase.h"
#include "SelectMusicPhase.h"
#include "SoundFFTCapture.h"
#include "SoundTimeLineData.h"
#include "Parameters.h"

//#include "Sound/SoundFile.h"

cSoundCapture*		cMusicGameApp::m_pSoundCapture = nullptr;
cSoundFFTCapture*	cMusicGameApp::m_pSoundFFTCapture = nullptr;

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
	Vector4*l_pViewPort = &this->m_svViewPortSize;
	*this->m_psstrGameAppName = "MusicGame";
	m_pPhaseManager = nullptr;
	m_svBGColor = Vector4(18/255.f,0,35/255.f,1.f);
}

cMusicGameApp::~cMusicGameApp()
{
	SAFE_DELETE(m_pPhaseManager);
	SAFE_DELETE(m_pSoundCapture);
	m_pSoundFFTCapture = nullptr;
}

void	cMusicGameApp::Init()
{
	
	//FATMING_CORE::cSoundFile* l_SoundFile = new FATMING_CORE::cSoundFile();
	//l_SoundFile->WavToOggFile("Sound/2owl.wav","t1.ogg");

	cGameApp::Init();
	//sound setup
	m_pSoundFFTCapture = new cSoundFFTCapture();
	//int l_iFrequence = cSoundCompareParameter::m_siRecordFrequency;
	int l_iFrequence = 44100;
	m_pSoundCapture = new cSoundCapture(l_iFrequence,AL_FORMAT_MONO16,m_pSoundFFTCapture->GetOpanalCaptureBufferSize(ONE_FRAME_NEED_NUM_FFT_DATA_COUNT,l_iFrequence,true,AL_FORMAT_MONO16));
	m_pSoundCapture->AddObject(m_pSoundFFTCapture);
	//m_pSoundCapture->AddSoundRecord("Test.ogg",eCaptureSoundFileFormat::eCSFF_OGG);
	//m_pSoundCapture->AddSoundRecord("Test.wav",eCaptureSoundFileFormat::eCSFF_WAV);
	//
	m_pPhaseManager = new cPhaseManager();
	if( m_pPhaseManager )
	{
		cPerformMusicPhase*l_pPerformMusicPhase = new cPerformMusicPhase();
		m_pPhaseManager->AddObjectNeglectExist(l_pPerformMusicPhase);
		//
		cSelectMusicPhase*l_pSelectMusicPhase = new cSelectMusicPhase();
		m_pPhaseManager->AddObjectNeglectExist(l_pSelectMusicPhase);
		//
		m_pPhaseManager->SetCurrentCurrentPhase(l_pSelectMusicPhase->GetName());
		//m_pPhaseManager->SetCurrentCurrentPhase(l_pPerformMusicPhase->GetName());
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
	//
	//
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	cGameApp::Render();
	if( m_pPhaseManager )
	{
		m_pPhaseManager->Render();
		if( m_sbDebugFunctionWorking )
			m_pPhaseManager->DebugRender();
		
	}
#ifdef DEBUG
	GLRender::RenderRectangle(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,cMatrix44::Identity,Vector4(1,1,0,1));
	//cGameApp::ShowInfo();
#endif
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
	glFlush();
}


void	cMusicGameApp::MouseDown(int e_iPosX,int e_iPosY)
{
	cGameApp::MouseDown(e_iPosX,e_iPosY);
	if( m_pPhaseManager )
	{
		m_pPhaseManager->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
}

void	cMusicGameApp::MouseMove(int e_iPosX,int e_iPosY)
{
	cGameApp::MouseMove(e_iPosX,e_iPosY);
	if( m_pPhaseManager )
	{
		m_pPhaseManager->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
}

void	cMusicGameApp::MouseUp(int e_iPosX,int e_iPosY)
{
	cGameApp::MouseUp(e_iPosX,e_iPosY);
	if( m_pPhaseManager )
	{
		m_pPhaseManager->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
}


void	cMusicGameApp::KeyPress(char e_char)
{
	cGameApp::KeyPress(e_char);
	if( m_pPhaseManager )
	{
		m_pPhaseManager->KeyPress(e_char);
	}
}

void	cMusicGameApp::KeyDown(char e_char)
{
	cGameApp::KeyDown(e_char);
	if( m_pPhaseManager )
	{
		m_pPhaseManager->KeyDown(e_char);
	}
}

void	cMusicGameApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
	if( m_pPhaseManager )
	{
		m_pPhaseManager->KeyUp(e_char);
	}
}

void	cMusicGameApp::EditorInit()
{
	cGameApp::Init();
	//sound setup
	m_pSoundFFTCapture = new cSoundFFTCapture();
	int l_iFrequence = 44100;
	m_pSoundCapture = new cSoundCapture(l_iFrequence,AL_FORMAT_MONO16,m_pSoundFFTCapture->GetOpanalCaptureBufferSize(ONE_FRAME_NEED_NUM_FFT_DATA_COUNT,l_iFrequence,true,AL_FORMAT_MONO16));
	m_pSoundCapture->AddObject(m_pSoundFFTCapture);
	//m_pSoundCapture->AddSoundRecord("Test.ogg",eCaptureSoundFileFormat::eCSFF_OGG);
	//m_pSoundCapture->AddSoundRecord("Test.wav",eCaptureSoundFileFormat::eCSFF_WAV);
	//
	cGameApp::m_sTimeAndFPS.Update();
}

void	cMusicGameApp::EditorUpdate(float e_fElpaseTime)
{
	cGameApp::Update(e_fElpaseTime);
	if( m_pPhaseManager )
	{
		m_pPhaseManager->Update(e_fElpaseTime);
	}
	if( m_pSoundFFTCapture )
		m_pSoundFFTCapture->Update(e_fElpaseTime);
}

void	cMusicGameApp::EditorRender()
{
	//
	//
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	cGameApp::Render();
	if( m_pSoundFFTCapture )
		m_pSoundFFTCapture->Render();
}


void	cMusicGameApp::EditorRun()
{
	    cTexture::m_suiLastUsingImageIndex = -1;
		m_sTimeAndFPS.Update();
		float	l_fElpaseTime = m_sTimeAndFPS.fElpaseTime;
#ifdef DEBUG
		if( m_sbGamePause )
			l_fElpaseTime = 0.f;
#endif
		EditorUpdate(m_sbSpeedControl?l_fElpaseTime*this->m_sfDebugValue:l_fElpaseTime);
		EditorRender();
		cGameApp::m_sMouseWhellDelta = 0;
}