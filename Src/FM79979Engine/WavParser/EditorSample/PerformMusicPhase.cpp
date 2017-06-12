#include "stdafx.h"
#include "PerformMusicPhase.h"
#include "SoundTimeLineVisualizer.h"
#include "MusicGameApp.h"
#include "SoundFFTCapture.h"
#include "QuickFFTDataFrequencyFinder.h"

cPerformMusicPhase::cPerformMusicPhase()
{
	this->SetName(PERFORM_MUSIC_PHASE);
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
		m_pClickBehaviorDispatcher = new cClickBehaviorDispatcher();
		m_pPerformMusicPhaseUI = new cPerformMusicPhaseUI(this);
	}
}

void	cPerformMusicPhase::Init()
{
	this->m_bSatisfiedCondition = false;
	if( cMusicGameApp::m_pSoundCapture )
		cMusicGameApp::m_pSoundCapture->StopRecord();
	if( m_strMusicFileName.length() )
	{
		//fuck,orientation and chart resolution and else should setup here.
		if( !m_pTimeLineRangeChart )
			m_pTimeLineRangeChart = new cTimeLineRangeChart();
		if(m_pTimeLineRangeChart->ParseWithMyParse(m_strMusicFileName.c_str()))
		{
			m_pTimeLineRangeChart->Init();
		}
		else
		{
			SAFE_DELETE(m_pTimeLineRangeChart);
		}
	}
	GenerateResources();
	if( m_pPerformMusicPhaseUI )
		this->m_pPerformMusicPhaseUI->InitNodes();
	if( m_pClickBehaviorDispatcher )
		m_pClickBehaviorDispatcher->Init();
	if(cMusicGameApp::m_pSoundFFTCapture)
	{
		cMusicGameApp::m_pSoundFFTCapture->SetFilter(true);
		cMusicGameApp::m_pSoundFFTCapture->SetFilterStrengthValue(80);
		cMusicGameApp::m_pSoundFFTCapture->SetiFrenquenceFilterEndScaleValue(0.8f);
	}
	if( cMusicGameApp::m_pSoundCapture )
		cMusicGameApp::m_pSoundCapture->StartRecord();
	cGameApp::m_sTimeAndFPS.Update();
}

void	cPerformMusicPhase::Update(float e_fElpaseTime)
{
	if( cMusicGameApp::m_pSoundFFTCapture && !cGameApp::m_sbGamePause )
	{
		cMusicGameApp::m_pSoundFFTCapture->Update(e_fElpaseTime);
	}

	if(m_pTimeLineRangeChart)
	{
		m_pTimeLineRangeChart->Update(e_fElpaseTime);
		if( !cGameApp::m_sbGamePause )
		{
			auto l_pQuickFFTDataFrequencyFinder = cMusicGameApp::m_pSoundFFTCapture->GetQuickFFTDataFrequencyFinder();
			m_pTimeLineRangeChart->Compare(e_fElpaseTime,l_pQuickFFTDataFrequencyFinder);
		}
#ifdef PARSE_TEST_SOUND
	{
		std::wstring l_str;
		for(int i=0;i<l_pQuickFFTDataFrequencyFinder->m_iFFTBinCount;++i)
		{
			if(l_pQuickFFTDataFrequencyFinder->GetFFTData()[i]>100)
			{
				int l_iFrequency = (int)l_pQuickFFTDataFrequencyFinder->m_fFrequencyGap*i;
				//if(cGameApp::m_sfDebugValue!=1.f)
				{
					l_str += ValueToStringW(l_iFrequency);
					l_str += L",";
				}
			}
		}
		if( l_str.length() )
		{
			std::wstring l_strInfo = this->GetName();
			l_strInfo += L":";
			l_strInfo += l_str;
			//cGameApp::OutputDebugInfoString(l_strInfo.c_str());
		}
	}
#endif
	}
	if( m_pClickBehaviorDispatcher )
		m_pClickBehaviorDispatcher->Update(cGameApp::m_sTimeAndFPS.fElpaseTime);
	if( m_pPerformMusicPhaseUI )
		this->m_pPerformMusicPhaseUI->UpdateNodes(cGameApp::m_sTimeAndFPS.fElpaseTime);
}

void	cPerformMusicPhase::Render()
{
	DebugRender();
	if( m_pPerformMusicPhaseUI )
		this->m_pPerformMusicPhaseUI->RenderNodes();
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
}

void	cPerformMusicPhase::Destroy()
{
	if( cMusicGameApp::m_pSoundCapture )
		cMusicGameApp::m_pSoundCapture->StopRecord();
	cGameApp::m_sbGamePause = false;
	//m_pClickBehaviorDispatcher->Destroy();
}

void	cPerformMusicPhase::DebugRender()
{
	if(m_pTimeLineRangeChart)
	{
		m_pTimeLineRangeChart->DebugRender();
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