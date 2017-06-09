#include "stdafx.h"
#include "PerformMusicPhase.h"

cPerformMusicPhaseUI::cPerformMusicPhaseUI()
{
	m_PauseMPDI = nullptr;
	m_ScoreMPDI = nullptr;
	m_pPauseClickBehavior = nullptr;
	m_pScoreClickBehavior = nullptr;
}

cPerformMusicPhaseUI::~cPerformMusicPhaseUI()
{
	
}

bool	cPerformMusicPhaseUI::GenerateButtons(cClickBehaviorDispatcher*e_pClickBehaviorDispatcher)
{
	if( m_PauseMPDI == nullptr )
	{
		cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(L"MusicGame/Image/UI.mpdi");
		m_PauseMPDI = l_pMPDIList->GetObject(L"PauseBoard");
		m_ScoreMPDI = l_pMPDIList->GetObject(L"ScroeBoard");
		{
			auto l_pReplayButtonSubMPDI =  m_ScoreMPDI->GetObject(L"ReplayButton");
			auto l_pLeaveButtonSubMPDI =  m_ScoreMPDI->GetObject(L"LeaveButton");
			//l_pReplayButtonSubMPDI;
			//cClickBehaviorGroup*l_pBGClickBehaviorGroup = new cClickBehaviorGroup();
			//l_pBGClickBehaviorGroup->CreateFullScreenCollide();
			//l_pBGClickBehaviorGroup->AddDefaultRenderClickBehaviorButton();
		}
		{

			auto l_pLeaveButtonSubMPDI =  m_PauseMPDI->GetObject(L"LeaveButton");
			auto l_pResumeButtonSubMPDI =  m_PauseMPDI->GetObject(L"ResumeButton");
		}
		return true;
	}
	return true;
}

void	cPerformMusicPhaseUI::Init()
{
	if( m_PauseMPDI == nullptr )
	{
		cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(L"MusicGame/Image/UI.mpdi");
		m_PauseMPDI = l_pMPDIList->GetObject(L"PauseBoard");
		m_ScoreMPDI = l_pMPDIList->GetObject(L"ScroeBoard");
		{
			auto l_pReplayButtonSubMPDI =  m_ScoreMPDI->GetObject(L"ReplayButton");
			auto l_pLeaveButtonSubMPDI =  m_ScoreMPDI->GetObject(L"LeaveButton");
		}
		{

			auto l_pLeaveButtonSubMPDI =  m_PauseMPDI->GetObject(L"LeaveButton");
			auto l_pResumeButtonSubMPDI =  m_PauseMPDI->GetObject(L"ResumeButton");
		}
		
	}
}
void	cPerformMusicPhaseUI::Update(float e_fElpaseTime)
{
	
}
void	cPerformMusicPhaseUI::Render()
{
	
}