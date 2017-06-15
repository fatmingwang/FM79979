#include "stdafx.h"
#include "SelectMusicPhase.h"


cSongInfoBoardUI::cSongInfoBoardUI(cMPDI*e_pMPDI,cNamedTypedObjectVector<cMusicInfo>*e_pMusicInfoVector)
{
	this->SetName(L"cSongInfoBoardUI");
	m_pMusicInfoVector = e_pMusicInfoVector;
	//this->m_bIgnoreChildrenUpdate = false;
	m_pSongNameFont = nullptr;
	m_pBestScoreFont = nullptr;
	m_pSongInfoFont = nullptr;
	m_pPlayButtonImage = nullptr;
	m_pSelectScroller = nullptr;
	m_pMPDI = new cMPDI(e_pMPDI);
	//for test!
	//cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName("MyFMBook/AnimationDemo/MPDI/endanimation.mpdi",true);
	//m_pMPDI = new cMPDI(l_pMPDIList->GetObject(0));
	m_pMPDI->Init();
}

cSongInfoBoardUI::~cSongInfoBoardUI()
{
	//if you afried something wrong want manual to delete object set this->m_bIgnoreChildrenUpdate = true
	//SAFE_DELETE(m_pMPDI);
	//SAFE_DELETE(m_pSongNameFont);
	//SAFE_DELETE(m_pBestScoreFont);
	//SAFE_DELETE(m_pSongInfoFont);
	//SAFE_DELETE(m_pPlayButtonImage);
	//SAFE_DELETE(this->m_pSelectScroller);
}

void	cSongInfoBoardUI::PlayButtonClick(int e_iPosX,int e_iPosY,cClickBehavior*e_pButton)
{
	
}

bool	cSongInfoBoardUI::CreateMusicList(cNamedTypedObjectVector<cMusicInfo>&e_MusicInfoVector,int e_iRow,int e_iColumn,Vector2 e_vGap)
{
	SAFE_DELETE(this->m_pSelectScroller);
	this->m_pSelectScroller = new cSelectScroller(e_iRow,e_iColumn,e_vGap);
	m_pSelectScroller->SetName(L"m_pSelectScroller");
	const WCHAR*l_strtt = m_pMPDI->GetObject(2)->GetName();

	auto*l_pBGImage = this->m_pMPDI->GetObject(L"slider-shell");
	if( l_pBGImage )
	{
		//l_pBGImage->Init();
		Vector4 l_vCollideRect = l_pBGImage->GetCollideRectByIndex(0);
		l_vCollideRect = l_vCollideRect.ScaleByCenter(0.8f);
		m_pSelectScroller->SetCollisionRange(l_vCollideRect);
	}
	int l_iSize = e_MusicInfoVector.Count();
	for(int i=0;i<l_iSize;++i)
	{
		//cTextButton(Vector2 e_vRenderPos,cGlyphFontRender*e_pGlyphFontRender,Vector4 e_vPressedColor,const wchar_t* e_strText,cBaseImage*e_pConnectRadianImage = 0,cBaseImage*e_pLineImage = 0);
		cTextButton*l_pTextButton = new cTextButton(Vector2::Zero,
													cGameApp::m_spGlyphFontRender,
													Vector4::Blue,
													e_MusicInfoVector[i]->GetName()
													);
		l_pTextButton->SetName(e_MusicInfoVector[i]->GetName());
		this->m_pSelectScroller->AddObject(l_pTextButton);
	}
	//m_pFishCollectionScroller->SetCollisionRange(l_vCollisionRange);
	return true;
}

void	cSongInfoBoardUI::Init()
{
	bool l_b = this->m_bIgnoreChildrenUpdate;
	if( !m_pSongNameFont )
	{
		m_pSongNameFont = new cGlyphFontRender(cGameApp::m_spGlyphFontRender);
		m_pBestScoreFont = new cGlyphFontRender(cGameApp::m_spGlyphFontRender);
		m_pSongInfoFont = new cGlyphFontRender(cGameApp::m_spGlyphFontRender);
		m_pSongNameFont->SetName(L"m_pSongNameFont");
		m_pBestScoreFont->SetName(L"m_pBestScoreFont");
		m_pSongInfoFont->SetName(L"m_pSongInfoFont");

		Vector3 l_vPos;
		m_pMPDI->GetObjectPos(L"SongInfoText",l_vPos,true);
		m_pSongInfoFont->SetLocalPosition(l_vPos);
		m_pMPDI->GetObjectPos(L"SonNameText",l_vPos,true);
		m_pBestScoreFont->SetLocalPosition(l_vPos);
		m_pMPDI->GetObjectPos(L"BestScoreTest",l_vPos,true);
		m_pSongNameFont->SetLocalPosition(l_vPos);
		auto l_pPlayButtonMPDI = m_pMPDI->GetObject(L"PlayButton");
		if( l_pPlayButtonMPDI )
		{
			m_pPlayButtonImage = l_pPlayButtonMPDI->PointDataToBaseImage(0);
			m_pMPDI->RemoveObject(l_pPlayButtonMPDI);
		}
		this->AddChildToLast(m_pMPDI);
		this->AddChildToLast(this->m_pSelectScroller);
		this->AddChildToLast(m_pSongInfoFont);
		this->AddChildToLast(m_pBestScoreFont);
		this->AddChildToLast(m_pSongNameFont);
		if( m_pPlayButtonImage )
			this->AddChildToLast(m_pPlayButtonImage);

	}
	m_pSongNameFont->SetText(L"qoo");
	m_pBestScoreFont->SetText(L"qoo");
	m_pSongInfoFont->SetText(L"qoo");
	if( m_pMPDI )
		m_pMPDI->Init();
	if( m_pSelectScroller )
	{
		m_pSelectScroller->Init();
		m_pSelectScroller->SetCurrentWorkingObject(0);
	}
}
void	cSongInfoBoardUI::Update(float e_fElpaseTime)
{
	//this->UpdateNodes(e_fElpaseTime);
	if( m_pSelectScroller )
	{
		const WCHAR*l_strName = m_pSelectScroller->GetCurrentWorkingObjectName();
		if( l_strName && m_pMusicInfoVector )
		{
			//m_pMusicInfoVector->GetObject()
			m_pSongNameFont->SetText(l_strName);
			m_pBestScoreFont->SetText(l_strName);
			m_pSongInfoFont->SetText(l_strName);
		}
	}
}
void	cSongInfoBoardUI::Render()
{
	//this->RenderNodes();
}
void	cSongInfoBoardUI::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pSelectScroller )
		m_pSelectScroller->MouseDown(e_iPosX,e_iPosY);
}
void	cSongInfoBoardUI::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pSelectScroller )
		m_pSelectScroller->MouseMove(e_iPosX,e_iPosY);
}
void	cSongInfoBoardUI::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pSelectScroller )
		m_pSelectScroller->MouseUp(e_iPosX,e_iPosY);	
}

bool	cSongInfoBoardUI::IsPlayMusic()
{
	return false;
}

const wchar_t*	cSongInfoBoardUI::GetSelectMusicName()
{
	if(this->m_pSelectScroller)
	{
		return this->m_pSelectScroller->GetCurrentWorkingObjectName();
	}
	return nullptr;
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

void	cSelectScroller::SetCurrentWorkingObject(int e_iIndex)
{
	this->m_pCurrentWorkingObject = this->GetObject(0);
}