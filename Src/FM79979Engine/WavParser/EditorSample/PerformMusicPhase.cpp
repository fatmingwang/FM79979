#include "stdafx.h"
#include "PerformMusicPhase.h"
#include "SoundTimeLineVisualizer.h"


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
}

void	cPerformMusicPhase::Update(float e_fElpaseTime)
{
	if(m_pTimeLineRangeChart)
	{
		m_pTimeLineRangeChart->Update(e_fElpaseTime);
	}
}

void	cPerformMusicPhase::Render()
{
	if(m_pTimeLineRangeChart)
	{
		m_pTimeLineRangeChart->Render();
	}
}

void	cPerformMusicPhase::Destroy()
{

}

void	cPerformMusicPhase::DebugRender()
{

}