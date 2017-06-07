#include "stdafx.h"
#include "SelectMusicPhase.h"


cSelectMusicPhase::cSelectMusicPhase()
{
	m_pSongInfoBoardUI = nullptr;
	m_pClickBehaviorDispatcher = nullptr;
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
	if(this->ParseWithMyParse("MusicGame/Music/MusicList.xml"))
	{
	
	}
	if( !m_pSongInfoBoardUI )
	{
		cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName("MusicGame/Image/UI.mpdi",true);
		m_pSongInfoBoardUI = new cSongInfoBoardUI(l_pMPDIList->GetObject(L"MusicDetail"));
	}
	m_pSongInfoBoardUI->Init();
}

void	cSelectMusicPhase::Update(float e_fElpaseTime)
{
	if( m_pSongInfoBoardUI )
		m_pSongInfoBoardUI->Update(e_fElpaseTime);
}

void	cSelectMusicPhase::Render()
{
	if( m_pSongInfoBoardUI )
		m_pSongInfoBoardUI->Render();
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

}

void    cSelectMusicPhase::MouseDown(int e_iPosX,int e_iPosY)
{

}

void    cSelectMusicPhase::MouseMove(int e_iPosX,int e_iPosY)
{

}

void    cSelectMusicPhase::MouseUp(int e_iPosX,int e_iPosY)
{

}