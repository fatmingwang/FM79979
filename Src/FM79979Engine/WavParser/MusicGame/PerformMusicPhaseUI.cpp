#include "stdafx.h"
#include "PerformMusicPhase.h"
#include "SoundTimeLineVisualizer.h"
#include "MusicGameApp.h"
#include "SoundFFTCapture.h"
#include "PerformScoreCalculate.h"
cPerformMusicPhaseUI::cPerformMusicPhaseUI(cPerformMusicPhase*e_pPerformMusicPhase,cMPDI*e_pBGMPDI)
{
	this->SetName(L"cPerformMusicPhaseUI");
	m_pBGMPDI = e_pBGMPDI;
	m_pPerformMusicPhase = e_pPerformMusicPhase;
	m_pScoreText = nullptr;
	m_pPause = nullptr;
	m_pScore = nullptr;
	m_pBG = nullptr;
	m_pScoreFont = nullptr;
	m_pLaserMPDILeft = nullptr;;
	m_pLaserMPDIRight = nullptr;
}

cPerformMusicPhaseUI::~cPerformMusicPhaseUI()
{
	//SAFE_DELETE(m_pBG);
	SAFE_DELETE(m_pPause);
	SAFE_DELETE(m_pScore);
	SAFE_DELETE(m_pScoreFont);
	SAFE_DELETE(m_pLaserMPDILeft);
	SAFE_DELETE(m_pLaserMPDIRight);
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
		if( m_pBGMPDI )
		{
			cMPDI*l_pCloneBG = new cMPDI(m_pBGMPDI);
			auto l_pNote = l_pCloneBG->GetObject(L"Note");
			m_pBG = l_pCloneBG;
		}
		this->AddChildToLast(m_pBG);
		//generate time control
		{
			auto l_pPI = cGameApp::GetPuzzleImageByFileName(L"MusicGame/Image/UI.pi");
			l_pPauseImage = new cBaseImage(l_pPI->GetObject(L"Pause"));
			if( l_pPauseImage )
			{
				this->AddChildToLast(l_pPauseImage);
				l_pPauseImage->SetPos(Vector2(1480,20));
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
	if(!m_pLaserMPDILeft)
	{
		cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(L"MusicGame/Image/PerformPhaseAnimation.mpdi");
		auto l_pRightRaser = l_pMPDIList->GetObject(L"LaserRight");
		auto l_pLeftRaser = l_pMPDIList->GetObject(L"LaserLeft");
		if( l_pRightRaser )
		{
			m_pLaserMPDIRight = new cMPDI(l_pRightRaser);
			m_pLaserMPDIRight->SetVisible(false);
		}
		if( l_pLeftRaser )
		{
			m_pLaserMPDILeft = new cMPDI(l_pLeftRaser);
			m_pLaserMPDILeft->SetVisible(false);
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
	if(!m_pScoreFont)
	{
		m_pScoreFont = new cGlyphFontRender("MusicGame/Fonts/Roboto_Slab18B");
		m_pScoreFont->SetText(L"Score:0");
		this->AddChildToLast(m_pScoreFont);
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
	if(m_pLaserMPDIRight && m_pLaserMPDIRight->IsVisible())
	{
		m_pLaserMPDIRight->Update(e_fElpaseTime);
	}
	if(m_pLaserMPDILeft && m_pLaserMPDILeft->IsVisible())
	{
		m_pLaserMPDILeft->Update(e_fElpaseTime);
	}
}

void	cPerformMusicPhaseUI::Render()
{
	if(m_pLaserMPDIRight && m_pLaserMPDIRight->IsVisible())
	{
		m_pLaserMPDIRight->Render();
	}
	if(m_pLaserMPDILeft && m_pLaserMPDILeft->IsVisible())
	{
		m_pLaserMPDILeft->Render();
	}
}

void	cPerformMusicPhaseUI::ShotLaser(int e_iChannel)
{
	if(m_pLaserMPDIRight && e_iChannel == 0)
	{
		m_pLaserMPDIRight->Init();
		m_pLaserMPDIRight->SetVisible(true);
	}
	if(m_pLaserMPDILeft && e_iChannel == 1)
	{
		m_pLaserMPDILeft->Init();
		m_pLaserMPDILeft->SetVisible(true);
	}
}