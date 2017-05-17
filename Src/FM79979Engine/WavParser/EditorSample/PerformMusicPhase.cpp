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
}

cPerformMusicPhase::~cPerformMusicPhase()
{
	SAFE_DELETE(m_pTimeLineRangeChart);
}

void	cPerformMusicPhase::FetchData(const wchar_t*e_strPhaseName,void*e_pData)
{
	m_strMusicFileName = (char*)e_pData;
}

void	cPerformMusicPhase::Init()
{
	this->m_bSatisfiedCondition = false;
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
		cMusicGameApp::m_pSoundFFTCapture->SetFilterStrengthValue(06);
		cMusicGameApp::m_pSoundFFTCapture->SetiFrenquenceFilterEndScaleValue(0.6f);
	}
	if( cMusicGameApp::m_pSoundCapture )
		cMusicGameApp::m_pSoundCapture->StartRecord();

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
		m_pTimeLineRangeChart->Compare(cMusicGameApp::m_pSoundFFTCapture->GetQuickFFTDataFrequencyFinder());
	}
}

void	cPerformMusicPhase::Render()
{
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
	return nullptr;
}