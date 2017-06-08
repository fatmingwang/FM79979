#include "stdafx.h"
#include "SelectMusicPhase.h"


cSelectMusicPhase::cSelectMusicPhase()
{
	m_pSongInfoBoardUI = nullptr;
	m_pClickBehaviorDispatcher = nullptr;
	this->SetName(SELECT_MUSIC_PHASE);
	this->m_strNextPhaseName = PERFORM_MUSIC_PHASE;
}

cSelectMusicPhase::~cSelectMusicPhase()
{
	SAFE_DELETE(m_pSongInfoBoardUI);
	SAFE_DELETE(m_pClickBehaviorDispatcher);
}

bool	cSelectMusicPhase::MyParse(TiXmlElement*e_pRoot)
{
	m_MusicInfoVector.clear();
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pRoot)
		COMPARE_TARGET_ELEMENT_VALUE(e_pRoot,"Song")
		{
			sMusicInfo l_MusicInfo;
			PARSE_ELEMENT_START(e_pRoot)
				COMPARE_NAME("FileName")
				{
					l_MusicInfo.strFileName = l_strValue;
				}
				else
				COMPARE_NAME("SongName")
				{
					l_MusicInfo.strSongName = l_strValue;
				}
			TO_NEXT_VALUE
				this->m_MusicInfoVector.push_back(l_MusicInfo);
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pRoot)
	return true;
}

void	cSelectMusicPhase::Init()
{
	if(!this->ParseWithMyParse("MusicGame/Music/MusicList.xml"))
	{
		return;
	}
	if( !m_pClickBehaviorDispatcher )
		m_pClickBehaviorDispatcher = new cClickBehaviorDispatcher();
	if( !m_pSongInfoBoardUI )
	{
		cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName("MusicGame/Image/UI.mpdi",true);
		if( l_pMPDIList )
		{
			m_pSongInfoBoardUI = new cSongInfoBoardUI(l_pMPDIList->GetObject(L"MusicDetail"));
			m_pSongInfoBoardUI->CreateMusicList(m_MusicInfoVector,5,5,Vector2(120,50));
			if( m_pClickBehaviorDispatcher )
			{
				m_pClickBehaviorDispatcher->AddDefaultRenderClickBehaviorButton(m_pSongInfoBoardUI->GetPlayButtonImage(),
					std::bind(&cSelectMusicPhase::PlayButtonClick,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3),nullptr);
			}
		}
	}
	if( m_pClickBehaviorDispatcher )
		m_pClickBehaviorDispatcher->Init();
	if( m_pSongInfoBoardUI )
		m_pSongInfoBoardUI->Init();
}

void	cSelectMusicPhase::Update(float e_fElpaseTime)
{
	if( m_pSongInfoBoardUI )
		m_pSongInfoBoardUI->UpdateNodes(e_fElpaseTime);
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
	return nullptr;
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
	if( m_pSongInfoBoardUI )
		m_pSongInfoBoardUI->MouseDown(e_iPosX,e_iPosY);
}

void    cSelectMusicPhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pSongInfoBoardUI )
		m_pSongInfoBoardUI->MouseMove(e_iPosX,e_iPosY);
}

void    cSelectMusicPhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pSongInfoBoardUI )
		m_pSongInfoBoardUI->MouseUp(e_iPosX,e_iPosY);
}

void	cSelectMusicPhase::PlayButtonClick(int e_iPosX,int e_iPosY,cClickBehavior*e_pButton)
{
	this->m_bSatisfiedCondition = true;
}