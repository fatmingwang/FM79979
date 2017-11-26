#include "stdafx.h"
#include "PerformMusicPhase.h"
#include "SoundTimeLineVisualizer.h"
#include "MusicGameApp.h"
#include "SoundFFTCapture.h"
#include "PerformScoreCalculate.h"
#include "AllPhaseName.h"
cPerformMusicPhaseUI::cPerformMusicPhaseUI(cPerformMusicPhase*e_pPerformMusicPhase,cMPDI*e_pBGMPDI)
{
	this->SetName(L"cPerformMusicPhaseUI");
	m_pGreatMPDI = nullptr;;
	m_pOopsMPDI = nullptr;;
	m_pBGMPDI = e_pBGMPDI;
	m_pPerformMusicPhase = e_pPerformMusicPhase;
	//m_pScoreText = nullptr;
	m_pPause = nullptr;
	m_pCurrentScoreNumeial = nullptr;
	m_pScore = nullptr;
	m_pBG = nullptr;
	m_pC4Text = nullptr;
	m_pG4Text = nullptr;
	//m_pScoreFont = nullptr;
	m_pTouchMPDI = nullptr;
	m_pScoreNumeial = nullptr;
	m_pPerformScoreCalculate = new cPerformScoreCalculate(this->m_pPerformMusicPhase->m_pTimeLineRangeChart);
	REG_EVENT(TUNE_TIME_OVER_EVENTID,&cPerformMusicPhaseUI::TimeOverEventFire);
	REG_EVENT(TUNE_MATCH_EVENT_ID,&cPerformMusicPhaseUI::TuneMatchedFire);
}

cPerformMusicPhaseUI::~cPerformMusicPhaseUI()
{
	//SAFE_DELETE(m_pBG);
	//SAFE_DELETE(m_pCurrentScoreNumeial);
	SAFE_DELETE(m_pPerformScoreCalculate);
	SAFE_DELETE(m_pTouchMPDI);
	SAFE_DELETE(m_pPause);
	SAFE_DELETE(m_pScore);
//	SAFE_DELETE(m_pScoreNumeial);
	//SAFE_DELETE(m_pScoreFont);	
}


bool	cPerformMusicPhaseUI::TimeOverEventFire(void*e_pData)
{
	cSoundTimeLineData*l_pSoundTimeLineData = (cSoundTimeLineData*)e_pData;
	if( m_pGreatMPDI )
	{
		m_pOopsMPDI->SetPos(l_pSoundTimeLineData->GetNoteShowPos());
		m_pOopsMPDI->Init();
	}
	
	//Justin dont want this.
	return true; 
	m_pTouchMPDI->SetPos(l_pSoundTimeLineData->GetNoteShowPos());
	m_pTouchMPDI->Init();
	m_pTouchMPDI->SetAnimationLoop(false);
	m_pTouchMPDI->SetVisible(true);
	return true;
}

bool	cPerformMusicPhaseUI::TuneMatchedFire(void*e_pData)
{
	cSoundTimeLineData*l_pSoundTimeLineData = (cSoundTimeLineData*)e_pData;
	m_pTouchMPDI->SetPos(l_pSoundTimeLineData->GetNoteShowPos());
	m_pTouchMPDI->Init();
	m_pTouchMPDI->SetAnimationLoop(false);
	m_pTouchMPDI->SetVisible(true);
	ShotLaser();
	if( m_pPerformScoreCalculate )
		m_pPerformScoreCalculate->CalculateScore();
	if( m_pGreatMPDI )
	{
		m_pGreatMPDI->SetPos(l_pSoundTimeLineData->GetNoteShowPos());
		m_pGreatMPDI->Init();
	}
	return true;
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
				l_pPauseImage->SetPos(Vector2(cGameApp::m_svGameResolution.x-l_pPauseImage->GetWidth()-20,20));
				cClickBehavior*l_pTimeControlButton = nullptr;
				if( this->m_pPerformMusicPhase->m_pTimeLineRangeChart )
				{
					auto l_pTickImage = this->m_pPerformMusicPhase->m_pTimeLineRangeChart->GetTimeControlImage();
					l_pTimeControlButton = e_pClickBehaviorDispatcher->AddDefaultRenderClickBehaviorButton(l_pTickImage,nullptr,nullptr);
					auto l_pLocalBound = l_pTickImage->GetLocalBound();
					RECT l_Rect = l_pLocalBound->GetRect();
					POINT l_Size = {l_Rect.right-l_Rect.left,l_Rect.bottom-l_Rect.top};
					//l_Rect.left -= l_Size.x*10;
					//l_Rect.right += l_Size.x*10;
					l_Rect.left = -128;
					l_Rect.right = 128;
					l_Rect.top = -128;
					l_Rect.bottom = 128;
					cBound l_Bound(l_Rect);
					l_pTickImage->SetLocalBound(&l_Bound);

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
	if(m_LaserMPDILeftVector.Count() == 0)
	{
		cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(L"MusicGame/Image/PerformPhaseAnimation.mpdi");

		auto l_pRightRaser = l_pMPDIList->GetObject(LEFT_LASER_MPDI_NAME);
		auto l_pLeftRaser = l_pMPDIList->GetObject(RIGHT_LASER_MPDI_NAME);
		for( int i=0;i<99;++i )
		{
			wchar_t l_strTemp[MAX_PATH];
			swprintf(l_strTemp,MAX_PATH,L"LaserRight_%d_2208",i);
			auto l_pRightMPDI = l_pMPDIList->GetObject(l_strTemp);
			if( !l_pRightMPDI )
				break;
			cMPDI*l_pRightClone = new cMPDI(l_pRightMPDI);
			l_pRightClone->SetVisible(false);
			m_LaserMPDIRightVector.AddObject(l_pRightClone);
			swprintf(l_strTemp,MAX_PATH,L"LaserLeft_%d_2208",i);
			auto l_pLeftMPDI = l_pMPDIList->GetObject(l_strTemp);
			if( !l_pLeftMPDI )
				break;
			cMPDI*l_pLeftClone = new cMPDI(l_pLeftMPDI);
			l_pLeftClone->SetVisible(false);
			m_LaserMPDILeftVector.AddObject(l_pLeftClone);
		}
	}
	if( !m_pTouchMPDI )
	{
		cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(L"MusicGame/Image/PerformPhaseAnimation.mpdi");
		if( l_pMPDIList )
		{
			 auto l_pNoteTouchMPDI = l_pMPDIList->GetObject(L"NoteTouch");
			 if( l_pNoteTouchMPDI )
			 {
				this->m_pTouchMPDI = new cMPDI(l_pNoteTouchMPDI);
				m_pTouchMPDI->SetVisible(false);
			 }
		}
		auto l_pGreatMPDI = l_pMPDIList->GetObject(L"Great");
		if( l_pGreatMPDI )
			m_pGreatMPDI = new cMPDI(l_pGreatMPDI);
		l_pGreatMPDI = l_pMPDIList->GetObject(L"Oops");
		if( l_pGreatMPDI )
			m_pOopsMPDI = new cMPDI(l_pGreatMPDI);
		if( m_pGreatMPDI )
			this->AddChildToLast(m_pGreatMPDI);
		if( m_pOopsMPDI )
			this->AddChildToLast(m_pOopsMPDI);
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
				//m_pScoreText = new cGlyphFontRender(cGameApp::m_spGlyphFontRender);
				l_pScoreMPDI->GetObjectPos(L"ScoreText",l_vPos,true);
				cPuzzleImage*l_pUIPI = cGameApp::GetPuzzleImageByFileName(L"MusicGame/Image/UI.pi");
				if( l_pUIPI )
				{
					cBaseImage*l_pFirstImage = l_pUIPI->GetObject(L"0");
					cBaseImage*l_pLastImage = l_pUIPI->GetObject(L"9");
					if( l_pFirstImage && l_pLastImage )
					{
						m_pScoreNumeial = new cNumeralImage(l_pFirstImage,l_pLastImage);
						m_pScoreNumeial->SetPos(l_vPos);
						m_pCurrentScoreNumeial = new cNumeralImage(m_pScoreNumeial);
						m_pCurrentScoreNumeial->SetDrawOnCenter(true);
						m_pCurrentScoreNumeial->SetLocalTransform(cMatrix44::ScaleMatrix(Vector3(0.5f,0.5f,0.5f)));
						m_pCurrentScoreNumeial->SetPos(Vector2(100,50));
					}
				}
				//m_pScoreText->SetLocalPosition(l_vPos);
				m_pScore->AddChild(l_pScoreMPDI);
				m_pScore->AddChild(std::get<1>(l_pLeaveImage));
				m_pScore->AddChild(std::get<1>(l_pReplayImage));
				if( m_pScoreNumeial )
					m_pScore->AddChild(m_pScoreNumeial);
				//m_pScore->AddChild(m_pScoreText);
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
							{
								l_pPauseImage->SetColor(Vector4(1,1,0,1));
							}
							else
								l_pPauseImage->SetColor(Vector4(1,1,1,1));
							if( m_pPerformScoreCalculate )
								m_pPerformScoreCalculate->CalculateScore();
						}
						GamePause();
					},l_pBasicSound);
				m_pPause->AddChild(l_pPauseMPDI);
				m_pPause->AddChild(std::get<1>(l_pLeaveImage));
				m_pPause->AddChild(std::get<1>(l_pResumeImage));
			}
		}
	}
	if( !m_pC4Text )
	{
		m_pC4Text  = new cGlyphFontRender("MusicGame/Fonts/MingLiU36");
		m_pC4Text->SetText(L"C4");
		m_pC4Text->SetFontColor(Vector4(0,0,0,1));
		m_pC4Text->SetName(L"m_pC4Text");
		this->AddChildToLast(m_pC4Text);
		m_pC4Text->SetLocalPosition(Vector2(670,1200));
		//m_pC4Text->SetLocalPosition(Vector2(650,550));
		m_pG4Text = new cGlyphFontRender(m_pC4Text);
		m_pG4Text->SetText(L"B4");
		m_pG4Text->SetFontColor(Vector4(0,0,0,1));
		m_pG4Text->SetName(L"m_pG4Text ");
		m_pG4Text->SetLocalPosition(Vector2(1520,1200));

		if( m_pCurrentScoreNumeial )
			this->AddChildToLast(m_pCurrentScoreNumeial);
		this->AddChildToLast(m_pG4Text);
	}
	//if(!m_pScoreFont)
	//{
	//	m_pScoreFont = new cGlyphFontRender("MusicGame/Fonts/Roboto_Slab18B");
	//	m_pScoreFont->SetText(L"Score:0");
	//	this->AddChildToLast(m_pScoreFont);
	//}
	return true;
}

void	cPerformMusicPhaseUI::Init()
{
	GenerateResources(m_pPerformMusicPhase->m_pClickBehaviorDispatcher);
	//m_LaserMPDIRightVector.Init();
	int l_iCount = m_LaserMPDILeftVector.Count();
	for(int i=0;i<l_iCount;++i)
	{
		m_LaserMPDILeftVector[i]->SetAnimationDone(true);
	}
	l_iCount = m_LaserMPDIRightVector.Count();
	for(int i=0;i<l_iCount;++i)
	{
		m_LaserMPDIRightVector[i]->SetAnimationDone(true);
	}

	if( m_pScore )
	{
		m_pScore->SetEnable(false);
	}
	if( m_pPause )
	{
		m_pPause->SetEnable(false);
	}
	if( m_pPerformScoreCalculate )
		m_pPerformScoreCalculate->CalculateScore();
}

void	cPerformMusicPhaseUI::Update(float e_fElpaseTime)
{
	if( m_pCurrentScoreNumeial )
		m_pCurrentScoreNumeial->SetValue(m_pPerformScoreCalculate->m_iScore);
	if( m_pScore && this->m_pPerformMusicPhase->m_pTimeLineRangeChart->IsEnd() )
	{
		if( !m_pScore->IsEnable() )
		{
			m_pScore->SetEnable(true);
			if( this->m_pScoreNumeial )
			{
				if( m_pPerformScoreCalculate )
				{
					m_pPerformScoreCalculate->CalculateScore();	
					this->m_pScoreNumeial->SetValue(m_pPerformScoreCalculate->m_iScore);
				}
				this->m_pScoreNumeial->SetDrawOnCenter(true);
			}
			//if( this->m_pScoreText )
			//{
			//	cPerformScoreCalculate l_cPerformScoreCalculate(this->m_pPerformMusicPhase->m_pTimeLineRangeChart);
			//	l_cPerformScoreCalculate.CalculateScore();
			//	std::wstring l_strText = L"Correct:";
			//	l_strText += ValueToStringW(l_cPerformScoreCalculate.m_iCorrect);
			//	l_strText += L"\nWrong:";
			//	l_strText += ValueToStringW(l_cPerformScoreCalculate.m_iWrong);
			//	l_strText += L"\nScore:";
			//	l_strText += ValueToStringW(l_cPerformScoreCalculate.m_iScore);
			//	this->m_pScoreText->SetText(l_strText.c_str());
			//}
		}
	}
	int l_iCount = m_LaserMPDILeftVector.Count();
	for(int i=0;i<l_iCount;++i)
	{
		m_LaserMPDILeftVector[i]->Update(e_fElpaseTime);
	}
	l_iCount = m_LaserMPDIRightVector.Count();
	for(int i=0;i<l_iCount;++i)
	{
		m_LaserMPDIRightVector[i]->Update(e_fElpaseTime);
	}
	if( m_pTouchMPDI && m_pTouchMPDI->IsVisible() )
	{
		m_pTouchMPDI->Update(e_fElpaseTime);
		if(m_pTouchMPDI->IsAnimationDone())
			m_pTouchMPDI->SetVisible(false);
	}
}

void	cPerformMusicPhaseUI::Render()
{
	int l_iCount = m_LaserMPDILeftVector.Count();
	for(int i=0;i<l_iCount;++i)
	{
		m_LaserMPDILeftVector[i]->Render();
	}
	l_iCount = m_LaserMPDIRightVector.Count();
	for(int i=0;i<l_iCount;++i)
	{
		m_LaserMPDIRightVector[i]->Render();
	}
	if( m_pTouchMPDI && m_pTouchMPDI->IsVisible() )
		m_pTouchMPDI->Render();
}

void	cPerformMusicPhaseUI::ShotLaser()
{
	static int l_i = 0;
	int l_iTotalCount = m_LaserMPDIRightVector.Count()+m_LaserMPDILeftVector.Count();
	int l_iIndex = l_i%l_iTotalCount;
	int l_iResult = l_iIndex/2;
	if( l_iIndex%2 )
	{
		m_LaserMPDIRightVector[l_iResult]->Init();
	}
	else
	{
		m_LaserMPDILeftVector[l_iResult]->Init();
	}
	++l_i;
}