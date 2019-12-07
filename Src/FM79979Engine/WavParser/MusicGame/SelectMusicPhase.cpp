#include "stdafx.h"
#include "SelectMusicPhase.h"
#include "AllPhaseName.h"
#include "Parameters.h"

cSelectMusicPhase::cSelectMusicPhase()
{
	m_pSongInfoBoardUI = nullptr;
	m_pClickBehaviorDispatcher = nullptr;
	this->SetName(SELECT_MUSIC_PHASE);
	this->m_strNextPhaseName = PERFORM_MUSIC_PHASE;
}

cSelectMusicPhase::~cSelectMusicPhase()
{
	Frame::DestoryWithChildren(m_pSongInfoBoardUI);
	SAFE_DELETE(m_pClickBehaviorDispatcher);
}

bool	cSelectMusicPhase::MyParse(TiXmlElement*e_pRoot)
{
	m_MusicInfoVector.Destroy();
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pRoot)
		COMPARE_TARGET_ELEMENT_VALUE(e_pRoot,"Song")
		{
			cMusicInfo*l_pMusicInfo = new cMusicInfo();
			PARSE_ELEMENT_START(e_pRoot)
				COMPARE_NAME("FileName")
				{
					l_pMusicInfo->m_strFileName = l_strValue;
				}
				else
				COMPARE_NAME("SongName")
				{
					l_pMusicInfo->SetName(l_strValue);
				}
				else
				COMPARE_NAME("Info")
				{
					l_pMusicInfo->m_strInfo = l_strValue;
				}
			TO_NEXT_VALUE
			if(!this->m_MusicInfoVector.AddObjectWarningIfExists(l_pMusicInfo))
			{
				delete l_pMusicInfo;
			}
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pRoot)
	return true;
}

void	cSelectMusicPhase::Init()
{
	cGameApp::m_spOpenGLRender->m_vBGColor = Vector4(249/255.f,223/255.f,104/255.f,1.f);
	m_strSelectedMusicName = L"";
	if(!this->ParseWithMyParse("MusicGame/Music/MusicList.xml"))
	{
		return;
	}
	if( !m_pSongInfoBoardUI )
	{
		cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName("MusicGame/Image/UI.mpdi",true);
		if( l_pMPDIList )
		{
			m_pSongInfoBoardUI = new cSongInfoBoardUI(l_pMPDIList->GetObject(L"MusicDetail"),&this->m_MusicInfoVector);
			//m_pSongInfoBoardUI = new cSongInfoBoardUI(l_pMPDIList->GetObject(L"MusicDetail2"),&this->m_MusicInfoVector);
			m_pSongInfoBoardUI->CreateMusicList(m_MusicInfoVector,3,5,Vector2(200,60));
		}
	}
	if( m_pSongInfoBoardUI )
		m_pSongInfoBoardUI->Init();
	if(! m_pClickBehaviorDispatcher )
	{
		m_pClickBehaviorDispatcher = new cClickBehaviorDispatcher();
		m_pClickBehaviorDispatcher->AddDefaultRenderClickBehaviorButton(m_pSongInfoBoardUI->GetPlayButtonImage(),
			std::bind(&cSelectMusicPhase::PlayButtonClick,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3),nullptr);
	}
	if( m_pClickBehaviorDispatcher )
		m_pClickBehaviorDispatcher->Init();
}

void	cSelectMusicPhase::Update(float e_fElpaseTime)
{
	if( m_pSongInfoBoardUI )
		m_pSongInfoBoardUI->UpdateNodes(e_fElpaseTime);
	if( m_pClickBehaviorDispatcher )
		m_pClickBehaviorDispatcher->Update(e_fElpaseTime);
}

void	cSelectMusicPhase::Render()
{
	if( m_pSongInfoBoardUI )
		m_pSongInfoBoardUI->RenderNodes();
}
void	cSelectMusicPhase::Destroy()
{

}
void	cSelectMusicPhase::DebugRender()
{

}
void*	cSelectMusicPhase::GetData()
{

	//m_strSelectedMusicName = L"MusicGame/Music/Test.xml";
	m_strSelectedMusicName = L"MusicGame/Music/KateTwinkleTwinkle.xml";
	
	return (void*)&m_strSelectedMusicName;
}

void	cSelectMusicPhase::KeyUp(char e_cKey)
{
	if( e_cKey == 'R' || e_cKey == 'r'  )
	{
		Init();
	}
}

void    cSelectMusicPhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pSongInfoBoardUI )m_pSongInfoBoardUI->MouseDown(e_iPosX,e_iPosY);
	if(m_pClickBehaviorDispatcher)m_pClickBehaviorDispatcher->MouseDown(e_iPosX,e_iPosY);
}

void    cSelectMusicPhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pSongInfoBoardUI )m_pSongInfoBoardUI->MouseMove(e_iPosX,e_iPosY);
	if(m_pClickBehaviorDispatcher)m_pClickBehaviorDispatcher->MouseMove(e_iPosX,e_iPosY);
}

void    cSelectMusicPhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pSongInfoBoardUI )	m_pSongInfoBoardUI->MouseUp(e_iPosX,e_iPosY);
	if(m_pClickBehaviorDispatcher)m_pClickBehaviorDispatcher->MouseUp(e_iPosX,e_iPosY);
}

void	cSelectMusicPhase::PlayButtonClick(int e_iPosX,int e_iPosY,cClickBehavior*e_pButton)
{
	if( m_pSongInfoBoardUI )
	{
		auto l_pData  =this->m_MusicInfoVector.GetObject(m_pSongInfoBoardUI->GetSelectMusicName());
		if( l_pData  )
		{
			m_strSelectedMusicName = l_pData->m_strFileName;
			this->m_bSatisfiedCondition = true;
		}
	}
}