#include "stdafx.h"
#include "PerformMusicPhase.h"
#include "SoundTimeLineVisualizer.h"
#include "MusicGameApp.h"
#include "SoundFFTCapture.h"
#include "QuickFFTDataFrequencyFinder.h"
cPerformMusicPhase::cPerformMusicPhase()
{
	this->SetName(PERFORM_MUSIC_PHASE);
	m_pTimeLineRangeChart = nullptr;
	m_strMusicFileName = "MusicGame/Music/Test.xml";
	m_pBG = nullptr;
}

cPerformMusicPhase::~cPerformMusicPhase()
{
	SAFE_DELETE(m_pBG);
	SAFE_DELETE(m_pTimeLineRangeChart);
}

void	cPerformMusicPhase::FetchData(const wchar_t*e_strPhaseName,void*e_pData)
{
	m_strMusicFileName = (char*)e_pData;
}

void	cPerformMusicPhase::Init()
{
	this->m_bSatisfiedCondition = false;
	if( cMusicGameApp::m_pSoundCapture )
		cMusicGameApp::m_pSoundCapture->StopRecord();
	if( !m_pBG )
	{
		//m_pBG = new cBaseImage("MusicGame/Image/Piano.jpg");
	}
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
	if( cMusicGameApp::m_pSoundFFTCapture )
	{
		cMusicGameApp::m_pSoundFFTCapture->Update(e_fElpaseTime);
	}

	if(m_pTimeLineRangeChart)
	{
		m_pTimeLineRangeChart->Update(e_fElpaseTime);
		auto l_pQuickFFTDataFrequencyFinder = cMusicGameApp::m_pSoundFFTCapture->GetQuickFFTDataFrequencyFinder();
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
		m_pTimeLineRangeChart->Compare(e_fElpaseTime,l_pQuickFFTDataFrequencyFinder);
	}
}

void	cPerformMusicPhase::Render()
{
	if( m_pBG )
	{
		m_pBG->SetPos(Vector2(100,600));
		m_pBG->Render();
	}
	DebugRender();
}

void	cPerformMusicPhase::Destroy()
{

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
	if( cMusicGameApp::m_pSoundCapture )
		cMusicGameApp::m_pSoundCapture->StopRecord();
	SAFE_DELETE(m_pBG);
	return nullptr;
}

void	cPerformMusicPhase::KeyUp(char e_cKey)
{
	if( e_cKey == 'R' || e_cKey == 'r'  )
	{
		Init();
	}
}