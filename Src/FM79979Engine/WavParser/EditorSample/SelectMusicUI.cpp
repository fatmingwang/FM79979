#include "stdafx.h"
#include "SelectMusicPhase.h"


cSongInfoBoardUI::cSongInfoBoardUI(cMPDI*e_pMPDI)
{
	m_pSongNameFont = nullptr;
	m_pBestScoreFont = nullptr;
	m_pSongInfoFont = nullptr;
	m_pMPDI = nullptr;
	m_pPlayButtonImage = nullptr;
	m_pSelectScroller = nullptr;
}

cSongInfoBoardUI::~cSongInfoBoardUI()
{
	SAFE_DELETE(m_pSongNameFont);
	SAFE_DELETE(m_pBestScoreFont);
	SAFE_DELETE(m_pSongInfoFont);
}

void	cSongInfoBoardUI::PlayButtonClick(int e_iPosX,int e_iPosY,cClickBehavior*e_pButton)
{
	
}

bool	cSongInfoBoardUI::CreateMusicList(std::vector<sMusicInfo>&e_MusicInfoVector,int e_iRow,int e_iColumn,Vector2 e_vGap)
{
	SAFE_DELETE(this->m_pSelectScroller);
	this->m_pSelectScroller = new cSelectScroller(e_iRow,e_iColumn,e_vGap);
	size_t l_uiSize = e_MusicInfoVector.size();
	for(size_t i=0;i<l_uiSize;++i)
	{
		//cTextButton(Vector2 e_vRenderPos,cGlyphFontRender*e_pGlyphFontRender,Vector4 e_vPressedColor,const wchar_t* e_strText,cBaseImage*e_pConnectRadianImage = 0,cBaseImage*e_pLineImage = 0);
		cTextButton*l_pTextButton = new cTextButton(Vector2::Zero,
													cGameApp::m_spGlyphFontRender,
													Vector4::Blue,
													e_MusicInfoVector[i].strSongName.c_str()
													);
		
		this->m_pSelectScroller->AddObject(l_pTextButton);
	}
	//m_pFishCollectionScroller->SetCollisionRange(l_vCollisionRange);
	return true;
}

void	cSongInfoBoardUI::Init()
{
	if( m_pSelectScroller )
	{
		m_pSelectScroller->Init();
	}
	if( !m_pSongNameFont )
	{
		m_pSongNameFont = new cGlyphFontRender(cGameApp::m_spGlyphFontRender);
		m_pBestScoreFont = new cGlyphFontRender(cGameApp::m_spGlyphFontRender);
		m_pSongInfoFont = new cGlyphFontRender(cGameApp::m_spGlyphFontRender);
		this->AddChild(m_pSongInfoFont);
		this->AddChild(m_pBestScoreFont);
		this->AddChild(m_pSongNameFont);
	}
	m_pSongNameFont->SetText(L"");
	m_pBestScoreFont->SetText(L"");
	m_pSongInfoFont->SetText(L"");
}
void	cSongInfoBoardUI::Update(float e_fElpaseTime)
{

}
void	cSongInfoBoardUI::Render()
{

}
void	cSongInfoBoardUI::MouseDown(int e_iPosX,int e_iPosY)
{

}
void	cSongInfoBoardUI::MouseMove(int e_iPosX,int e_iPosY)
{

}
void	cSongInfoBoardUI::MouseUp(int e_iPosX,int e_iPosY)
{
	
}

bool	cSongInfoBoardUI::IsPlayMusic()
{
	return false;
}

std::wstring	cSongInfoBoardUI::GetSelectMusic()
{
	if(this->m_pSelectScroller)
	{
		return this->m_pSelectScroller->GetCurrentWorkingObjectName();
	}
	return L"";
}

cSelectScroller::cSelectScroller(int e_iNumRow,int e_iNumColumn,Vector2 e_vGap)
{
	this->m_eOrientation = eO_HORIZONTAL;
	//this->m_eOrientation = eO_VERTICAL;
	m_iNumRow = e_iNumRow;
	m_iNumColumn = e_iNumColumn;
	m_vGap = e_vGap;
	this->m_bCheckOutOfWorking = false;
}

cSelectScroller::~cSelectScroller()
{

}

void	cSelectScroller::Init()
{
	cClickMouseBehavior*l_pClickMouseBehavior = this->GetObject(0);
	Vector4	l_vOriginalViewRect = this->m_vCollisionRange;
	//this one will auto rest so set it back
    cMouse_TouchWithCancelButton::Init();
	m_vCollisionRange = l_vOriginalViewRect;
	m_bRollBack = false;
	m_fTotalLength = 0.f;
	if( this->m_pBG )
		m_pBG->Init();
    int l_iCOunt = this->Count();
    if( l_iCOunt == 0 )
        return;
    //POINT   l_Size = {*l_pClickMouseBehavior->GetWidth(),*l_pClickMouseBehavior->GetHeight()};
	POINT   l_Size = {(int)l_pClickMouseBehavior->GetCollisionRange().Width(),(int)l_pClickMouseBehavior->GetCollisionRange().Height()};
    Vector3 l_vStargPos = Vector3(m_vCollisionRange.x,m_vCollisionRange.y,0.f);

	for( int i=0;i<m_iNumColumn;++i )
	{
		Vector3 l_vPos;
		l_vPos.x = m_vGap.x*i+l_vStargPos.x;
		for( int j=0;j<m_iNumRow;++j )
		{
			l_vPos.y = m_vGap.y*j+l_vStargPos.y;
			int	l_iIndex = i*m_iNumRow+j;
			l_pClickMouseBehavior = GetObject(l_iIndex);
			if( l_pClickMouseBehavior )
			{
				l_pClickMouseBehavior->SetLocalPosition(l_vPos);
			}
			else
				return;
		}		
	}
}