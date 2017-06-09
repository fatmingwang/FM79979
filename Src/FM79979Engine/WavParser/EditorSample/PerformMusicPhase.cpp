#include "stdafx.h"
#include "PerformMusicPhase.h"
#include "SoundTimeLineVisualizer.h"
#include "MusicGameApp.h"
#include "SoundFFTCapture.h"
#include "QuickFFTDataFrequencyFinder.h"

cBaseImage*g_pEventClickTestImage = nullptr;

cPerformMusicPhase::cPerformMusicPhase()
{
	this->SetName(PERFORM_MUSIC_PHASE);
	m_pTimeLineRangeChart = nullptr;
	m_strMusicFileName = "MusicGame/Music/Test.xml";
	m_pBG = nullptr;
	m_bGamePause = false;
	m_pClickBehaviorDispatcher = nullptr;
}

cPerformMusicPhase::~cPerformMusicPhase()
{
	SAFE_DELETE(m_pClickBehaviorDispatcher);
	SAFE_DELETE(m_pBG);
	SAFE_DELETE(m_pTimeLineRangeChart);
	SAFE_DELETE(g_pEventClickTestImage);
}

void	cPerformMusicPhase::FetchData(const wchar_t*e_strPhaseName,void*e_pData)
{
	std::wstring l_str = *(std::wstring*)e_pData;
	m_strMusicFileName = UT::WcharToChar(l_str.c_str());
}

void	cPerformMusicPhase::GenerateButtons()
{
	if( !m_pClickBehaviorDispatcher )
	{
		m_pClickBehaviorDispatcher = new cClickBehaviorDispatcher();
		cBaseImage*l_pImage = new cBaseImage("MusicGame/Image/Pause.png");
		Vector2 l_vSize(l_pImage->GetWidth(),l_pImage->GetHeight());
		l_vSize /= 2.f;
		RECT l_RECT ={(long)-l_vSize.x,(long)-l_vSize.y,(long)l_vSize.x,(long)l_vSize.y};
		cBound l_Bound(l_RECT);
		l_pImage->SetLocalBound(&l_Bound);
		g_pEventClickTestImage = l_pImage;
		l_pImage->SetPos(Vector2(1750,600));
		cClickBehavior*l_pTimeControlButton = nullptr;
		if( m_pTimeLineRangeChart )
		{
			l_pTimeControlButton = m_pClickBehaviorDispatcher->AddDefaultRenderClickBehaviorButton(m_pTimeLineRangeChart->GetTimeControlImage(),nullptr,nullptr);
			if(l_pTimeControlButton )
			{
				l_pTimeControlButton->SetEnable(false);
				l_pTimeControlButton->SetAllowDrag(true);
				l_pTimeControlButton->SetMouseFunction(
					std::bind(&cTimeLineRangeChart::TimeControlCollision,m_pTimeLineRangeChart,std::placeholders::_1,std::placeholders::_2),
					std::bind(&cTimeLineRangeChart::TimeControlMouseDown,m_pTimeLineRangeChart,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3),
					std::bind(&cTimeLineRangeChart::TimeControlMouseMove,m_pTimeLineRangeChart,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3),
					std::bind(&cTimeLineRangeChart::TimeControlMouseUp,m_pTimeLineRangeChart,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3),
					nullptr,nullptr);
			}
		}
		cBasicSound*l_pBasicSound = cGameApp::GetSoundByFileName(L"MusicGame/Piano/rf3.wav");
		m_pClickBehaviorDispatcher->Init();
		m_pClickBehaviorDispatcher->AddDefaultRenderClickBehaviorButton(l_pImage,
			[l_pImage,this,l_pTimeControlButton](int e_iPosX,int e_iPosY,cClickBehavior*ClickBehavior){
				l_pImage->SetColor(Vector4(1,1,0,1));
				//m_bGamePause = !m_bGamePause;
				cGameApp::m_sbGamePause = !cGameApp::m_sbGamePause;
				if( l_pTimeControlButton )
					l_pTimeControlButton->SetEnable(!l_pTimeControlButton->IsEnable());
			}
			,l_pBasicSound);
	}
}

void	cPerformMusicPhase::Init()
{
	m_bGamePause = false;
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
	GenerateButtons();
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
		//cMusicGameApp::m_pSoundFFTCapture->Update(e_fElpaseTime);
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
}

void	cPerformMusicPhase::Render()
{
	if( m_pBG )
	{
		m_pBG->SetPos(Vector2(100,600));
		m_pBG->Render();
	}
	DebugRender();
	if( g_pEventClickTestImage )
		g_pEventClickTestImage->Render();
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
	SAFE_DELETE(m_pBG);
	m_pClickBehaviorDispatcher->Destroy();
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