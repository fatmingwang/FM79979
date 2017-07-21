#include "stdafx.h"
#include "PerformMusicPhase.h"
#include "SoundTimeLineVisualizer.h"
#include "MusicGameApp.h"
#include "SoundFFTCapture.h"
#include "FFTFrequency.h"
#include "AllPhaseName.h"

cPerformMusicPhase::cPerformMusicPhase()
{
	this->SetName(PERFORM_MUSIC_PHASE);
	m_bPasue = false;
	this->m_strNextPhaseName = SELECT_MUSIC_PHASE;
	m_pTimeLineRangeChart = nullptr;
	m_strMusicFileName = "MusicGame/Music/Test.xml";
	m_pClickBehaviorDispatcher = nullptr;
	m_pPerformMusicPhaseUI = nullptr;
}

cPerformMusicPhase::~cPerformMusicPhase()
{
	Frame::DestoryWithChildren(m_pPerformMusicPhaseUI);
	SAFE_DELETE(m_pClickBehaviorDispatcher);
	SAFE_DELETE(m_pTimeLineRangeChart);
}

void	cPerformMusicPhase::FetchData(const wchar_t*e_strPhaseName,void*e_pData)
{
	std::wstring l_str = *(std::wstring*)e_pData;
	m_strMusicFileName = UT::WcharToChar(l_str.c_str());
}

void	cPerformMusicPhase::GenerateResources()
{
	if( !m_pClickBehaviorDispatcher )
	{
		cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(L"MusicGame/Image/PerformPhaseAnimation.mpdi");
		cMPDI*l_pBG = nullptr;
		if( l_pMPDIList )
		{
			l_pBG = l_pMPDIList->GetObject(PERFORM_PHASE_BG_MPDI_NAME);
		}
		if( m_strMusicFileName.length() )
		{
			//fuck,orientation and chart resolution and else should setup here.
			if( !m_pTimeLineRangeChart )
			{
				m_pTimeLineRangeChart = new cTimeLineRangeChart(l_pBG);
			}
		}
		m_pClickBehaviorDispatcher = new cClickBehaviorDispatcher();
		m_pPerformMusicPhaseUI = new cPerformMusicPhaseUI(this,l_pBG);
	}
}

void	cPerformMusicPhase::Init()
{
	cGameApp::m_svBGColor = Vector4(18/255.f,0,35/255.f,1.f);
	m_bPasue = false;
	this->m_bSatisfiedCondition = false;
	if( cMusicGameApp::m_pSoundCapture )
		cMusicGameApp::m_pSoundCapture->StopRecord();
	GenerateResources();
	if(m_pTimeLineRangeChart->ParseWithMyParse(m_strMusicFileName.c_str()))
	{
		m_pTimeLineRangeChart->Init();
	}
	else
	{
		SAFE_DELETE(m_pTimeLineRangeChart);
	}
	if( m_pPerformMusicPhaseUI )
		this->m_pPerformMusicPhaseUI->InitNodes();
	if( m_pClickBehaviorDispatcher )
		m_pClickBehaviorDispatcher->Init();
	if( cMusicGameApp::m_pSoundCapture )
		cMusicGameApp::m_pSoundCapture->StartRecord();
	cGameApp::m_sTimeAndFPS.Update();
}

void	cPerformMusicPhase::Update(float e_fElpaseTime)
{
	if( cMusicGameApp::m_pSoundFFTCapture )
	{
		cMusicGameApp::m_pSoundFFTCapture->Update(e_fElpaseTime);
	}

	if(m_pTimeLineRangeChart )
	{
		m_pTimeLineRangeChart->Update(!m_bPasue?e_fElpaseTime:0.f);
		if(!m_bPasue)
		{
			auto l_pQuickFFTDataFrequencyFinder = cMusicGameApp::m_pSoundFFTCapture->GetQuickFFTDataFrequencyFinder();
			if( l_pQuickFFTDataFrequencyFinder )
				m_pTimeLineRangeChart->Compare(e_fElpaseTime,l_pQuickFFTDataFrequencyFinder);		
		}
	}
	if( m_pClickBehaviorDispatcher )
		m_pClickBehaviorDispatcher->Update(cGameApp::m_sTimeAndFPS.fElpaseTime);
	if( m_pPerformMusicPhaseUI )
		this->m_pPerformMusicPhaseUI->UpdateNodes(!m_bPasue?e_fElpaseTime:0.0000000001f);
}

void	cPerformMusicPhase::Render()
{
	if( m_pPerformMusicPhaseUI )
	{
		this->m_pPerformMusicPhaseUI->RenderNodes();
	}
	if(m_pTimeLineRangeChart)
	{
		m_pTimeLineRangeChart->Render();
		if( cGameApp::m_sbDebugFunctionWorking )
			m_pTimeLineRangeChart->DebugRender();
	}
}

//
void    cPerformMusicPhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pClickBehaviorDispatcher )
		m_pClickBehaviorDispatcher->MouseDown(e_iPosX,e_iPosY);
}
//
void    cPerformMusicPhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pClickBehaviorDispatcher )
		m_pClickBehaviorDispatcher->MouseMove(e_iPosX,e_iPosY);
}
//
void    cPerformMusicPhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pClickBehaviorDispatcher )
		m_pClickBehaviorDispatcher->MouseUp(e_iPosX,e_iPosY);
	if( m_pPerformMusicPhaseUI )
	{
		//this->m_pPerformMusicPhaseUI->ShotLaser();
	}
}

void	cPerformMusicPhase::Destroy()
{
	if( cMusicGameApp::m_pSoundCapture )
		cMusicGameApp::m_pSoundCapture->StopRecord();
	//m_pClickBehaviorDispatcher->Destroy();
}

void	cPerformMusicPhase::DebugRender()
{
	if(m_pTimeLineRangeChart)
	{
		//m_pTimeLineRangeChart->DebugRender();
	}
	if( cMusicGameApp::m_pSoundFFTCapture )
		cMusicGameApp::m_pSoundFFTCapture->Render();
}

void*	cPerformMusicPhase::GetData()
{
	Destroy();
	return nullptr;
}

void	cPerformMusicPhase::KeyUp(char e_cKey)
{
	if( e_cKey == 'R' || e_cKey == 'r'  )
	{
		Init();
	}
}