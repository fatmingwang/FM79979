#include "stdafx.h"
#include "PerformMusicPhase.h"
#include "SoundTimeLineVisualizer.h"
#include "MusicGameApp.h"
#include "SoundFFTCapture.h"
#include "PerformScoreCalculate.h"
cPerformMusicPhaseUI::cPerformMusicPhaseUI(cPerformMusicPhase*e_pPerformMusicPhase)
{
	this->SetName(L"cPerformMusicPhaseUI");
	m_pPerformMusicPhase = e_pPerformMusicPhase;
	m_pScoreText = nullptr;
	m_pPause = nullptr;
	m_pScore = nullptr;
	m_pBG = nullptr;
}

cPerformMusicPhaseUI::~cPerformMusicPhaseUI()
{
	//SAFE_DELETE(m_pBG);	
	SAFE_DELETE(m_pPause);
	SAFE_DELETE(m_pScore);
}

void	cPerformMusicPhaseUI::GamePause()
{
	if(cMusicGameApp::m_pSoundCapture)
	{
		cMusicGameApp::m_pSoundCapture->PauseRecord(!cMusicGameApp::m_pSoundCapture->IsPause());
	}
	this->m_pPerformMusicPhase->m_bPasue = !this->m_pPerformMusicPhase->m_bPasue;
	if( m_pTimeControlButton )
		m_pTimeControlButton->SetEnable(!m_pTimeControlButton->IsEnable());
	if(m_pPause)
		m_pPause->SetEnable(!m_pPause->IsEnable());
}

bool	cPerformMusicPhaseUI::GenerateResources(cClickBehaviorDispatcher*e_pClickBehaviorDispatcher)
{
	cBasicSound*l_pBasicSound = cGameApp::GetSoundByFileName(L"MusicGame/Piano/rf3.wav");
	cBaseImage*l_pPauseImage = nullptr;
	if( !m_pBG )
	{
		m_pBG = new cBaseImage("MusicGame/Image/Piano.jpg");
		//m_pBG->SetVisible(false);
		//m_pBG->SetWidth((int)cGameApp::m_svGameResolution.x);
		//MusicGame\Music,ChartResolution="1820,600" ChartShowPos="50,0"
		m_pBG->SetWidth(1820);;
		m_pBG->SetPos(Vector2(50,300));
		//m_pBG->SetHeight(cGameApp::m_svGameResolution.y);
		this->AddChildToLast(m_pBG);
		//generate time control
		{
			l_pPauseImage = new cBaseImage("MusicGame/Image/Pause.png");
			if( l_pPauseImage )
			{
				this->AddChildToLast(l_pPauseImage);
				l_pPauseImage->SetPos(Vector2(1750,600));
				cClickBehavior*l_pTimeControlButton = nullptr;
				if( this->m_pPerformMusicPhase->m_pTimeLineRangeChart )
				{
					l_pTimeControlButton = e_pClickBehaviorDispatcher->AddDefaultRenderClickBehaviorButton(this->m_pPerformMusicPhase->m_pTimeLineRangeChart->GetTimeControlImage(),nullptr,nullptr);
					m_pTimeControlButton = l_pTimeControlButton;
					if(l_pTimeControlButton )
					{
						l_pTimeControlButton->SetEnable(false);
						l_pTimeControlButton->SetAllowDrag(true);
						l_pTimeControlButton->SetMouseFunction(
							std::bind(&cTimeLineRangeChart::TimeControlCollision,m_pPerformMusicPhase->m_pTimeLineRangeChart,std::placeholders::_1,std::placeholders::_2),
							std::bind(&cTimeLineRangeChart::TimeControlMouseDown,m_pPerformMusicPhase->m_pTimeLineRangeChart,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3),
							std::bind(&cTimeLineRangeChart::TimeControlMouseMove,m_pPerformMusicPhase->m_pTimeLineRangeChart,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3),
							std::bind(&cTimeLineRangeChart::TimeControlMouseUp,m_pPerformMusicPhase->m_pTimeLineRangeChart,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3),
							nullptr,nullptr);
					}
				}
				e_pClickBehaviorDispatcher->AddDefaultRenderClickBehaviorButton(l_pPauseImage,
					[l_pPauseImage,this,l_pTimeControlButton](int e_iPosX,int e_iPosY,cClickBehavior*ClickBehavior){
						if( !m_pPause->IsEnable() )
							l_pPauseImage->SetColor(Vector4(1,1,0,1));
						else
							l_pPauseImage->SetColor(Vector4(1,1,1,1));
						GamePause();
					}
					,l_pBasicSound);
			}
		}
	}
	if( m_pPause == nullptr )
	{
		m_pScore = new cLazyClickBehaviorAndRenderObject();m_pScore->SetNameToAll(L"ScoreBoard");
		m_pPause = new cLazyClickBehaviorAndRenderObject();m_pPause->SetNameToAll(L"PauseBoard");
		e_pClickBehaviorDispatcher->AddObject(m_pScore->m_pClickBehaviorGroup);
		e_pClickBehaviorDispatcher->AddObject(m_pPause->m_pClickBehaviorGroup);
		this->AddChildToLast(m_pScore->m_pRenderObject);
		this->AddChildToLast(m_pPause->m_pRenderObject);
		cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(L"MusicGame/Image/UI.mpdi");
		if( l_pMPDIList )
		{
			auto l_pPauseMPDI = l_pMPDIList->GetCloneObject(L"PauseBoard");
			auto l_pScoreMPDI = l_pMPDIList->GetCloneObject(L"ScroeBoard");
			if( l_pScoreMPDI )
			{
				auto l_pReplayButtonSubMPDI =  l_pScoreMPDI->GetObject(L"ReplayButton");
				auto l_pLeaveButtonSubMPDI =  l_pScoreMPDI->GetObject(L"LeaveButton");
				auto l_pLeaveImage = m_pScore->m_pClickBehaviorGroup->AddDefaultRenderClickBehaviorButton(l_pLeaveButtonSubMPDI,
					[this](int e_iPosX,int e_iPosY,cClickBehavior*e_pClickBehavior){
						this->m_pPerformMusicPhase->m_bSatisfiedCondition = true;
					},l_pBasicSound);
				auto l_pReplayImage = m_pScore->m_pClickBehaviorGroup->AddDefaultRenderClickBehaviorButton(l_pReplayButtonSubMPDI,
					[this](int e_iPosX,int e_iPosY,cClickBehavior*e_pClickBehavior){
						this->m_pPerformMusicPhase->Init();
					},l_pBasicSound);

				Vector3 l_vPos;
				m_pScoreText = new cGlyphFontRender(cGameApp::m_spGlyphFontRender);
				l_pScoreMPDI->GetObjectPos(L"ScoreText",l_vPos,true);
				m_pScoreText->SetLocalPosition(l_vPos);
				m_pScore->AddChild(l_pScoreMPDI);
				m_pScore->AddChild(std::get<1>(l_pLeaveImage));
				m_pScore->AddChild(std::get<1>(l_pReplayImage));
				m_pScore->AddChild(m_pScoreText);
			}
			if( l_pPauseMPDI )
			{
				auto l_pLeaveButtonSubMPDI =  l_pPauseMPDI->GetObject(L"LeaveButton");
				auto l_pResumeButtonSubMPDI =  l_pPauseMPDI->GetObject(L"ResumeButton");
				auto l_pLeaveImage = m_pPause->m_pClickBehaviorGroup->AddDefaultRenderClickBehaviorButton(l_pLeaveButtonSubMPDI,
					[this](int e_iPosX,int e_iPosY,cClickBehavior*e_pClickBehavior){
						this->m_pPerformMusicPhase->m_bSatisfiedCondition = true;
					},l_pBasicSound);
				auto l_pResumeImage = m_pPause->m_pClickBehaviorGroup->AddDefaultRenderClickBehaviorButton(l_pResumeButtonSubMPDI,
					[this,l_pPauseImage](int e_iPosX,int e_iPosY,cClickBehavior*e_pClickBehavior){
						if( l_pPauseImage )
						{
							if( !m_pPause->IsEnable() )
								l_pPauseImage->SetColor(Vector4(1,1,0,1));
							else
								l_pPauseImage->SetColor(Vector4(1,1,1,1));
						}
						GamePause();
					},l_pBasicSound);
				m_pPause->AddChild(l_pPauseMPDI);
				m_pPause->AddChild(std::get<1>(l_pLeaveImage));
				m_pPause->AddChild(std::get<1>(l_pResumeImage));
			}
		}
	}
	return true;
}

void	cPerformMusicPhaseUI::Init()
{
	GenerateResources(m_pPerformMusicPhase->m_pClickBehaviorDispatcher);
	if( m_pScore )
	{
		m_pScore->SetEnable(false);
	}
	if( m_pPause )
	{
		m_pPause->SetEnable(false);
	}
}
void	cPerformMusicPhaseUI::Update(float e_fElpaseTime)
{
	if( m_pScore && this->m_pPerformMusicPhase->m_pTimeLineRangeChart->IsEnd() )
	{
		if( !m_pScore->IsEnable() )
		{
			m_pScore->SetEnable(true);
			if( this->m_pScoreText )
			{
				cPerformScoreCalculate l_cPerformScoreCalculate(this->m_pPerformMusicPhase->m_pTimeLineRangeChart);
				l_cPerformScoreCalculate.CalculateScore();
				std::wstring l_strText = L"Correct:";
				l_strText += ValueToStringW(l_cPerformScoreCalculate.m_iCorrect);
				l_strText += L"\nWrong:";
				l_strText += ValueToStringW(l_cPerformScoreCalculate.m_iWrong);
				l_strText += L"\nScore:";
				l_strText += ValueToStringW(l_cPerformScoreCalculate.m_iScore);
				this->m_pScoreText->SetText(l_strText.c_str());
			}
		}
	}
}
void	cPerformMusicPhaseUI::Render()
{
	//if( m_pBG )
	//{
	//	m_pBG->SetPos(Vector2(100,600));
	//	m_pBG->Render();
	//}
}