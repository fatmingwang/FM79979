#pragma once

#include "MusicInfo.h"
class cSongInfoBoardUI;
class cSelectScroller;
//<SongList>
//  <Song FileName="MusicGame/Music/Test.xml" SongName="Test"/>
//</SongList>
class cSelectMusicPhase:public cSimplePhase,public cNodeISAX
{
	std::wstring								m_strSelectedMusicName;
	cNamedTypedObjectVector<cMusicInfo>			m_MusicInfoVector;
	virtual	bool								MyParse(TiXmlElement*e_pRoot);
	//
	cSongInfoBoardUI*							m_pSongInfoBoardUI;
	cClickBehaviorDispatcher*					m_pClickBehaviorDispatcher;
public:
	cSelectMusicPhase();
	~cSelectMusicPhase();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	Destroy();
	virtual	void	DebugRender();
	virtual	void*	GetData();
	virtual	void	KeyUp(char e_cKey);
    virtual void    MouseDown(int e_iPosX,int e_iPosY);
    virtual void    MouseMove(int e_iPosX,int e_iPosY);
    virtual void    MouseUp(int e_iPosX,int e_iPosY);
	void			PlayButtonClick(int e_iPosX,int e_iPosY,cClickBehavior*e_pButton);

};



class cSongInfoBoardUI:public cRenderObject
{
	cGlyphFontRender*						m_pSongNameFont;
	cGlyphFontRender*						m_pBestScoreFont;
	cGlyphFontRender*						m_pSongInfoFont;
	cMPDI*									m_pMPDI;
	cBaseImage*								m_pPlayButtonImage;
	cSelectScroller*						m_pSelectScroller;
	cNamedTypedObjectVector<cMusicInfo>*	m_pMusicInfoVector;
public:
	cSongInfoBoardUI(cMPDI*e_pMPDI,cNamedTypedObjectVector<cMusicInfo>*e_pMusicInfoVector);
	~cSongInfoBoardUI();
	//
	void				PlayButtonClick(int e_iPosX,int e_iPosY,cClickBehavior*e_pButton);
	bool				CreateMusicList(cNamedTypedObjectVector<cMusicInfo>&e_MusicInfoVector,int e_iRow,int e_iColumn,Vector2 e_vGap);
	//
	void				Init();
	void				Update(float e_fElpaseTime);
	void				Render();
    void				MouseDown(int e_iPosX,int e_iPosY);
    void				MouseMove(int e_iPosX,int e_iPosY);
    void				MouseUp(int e_iPosX,int e_iPosY);
	bool				IsPlayMusic();
	const wchar_t*		GetSelectMusicName();
	cBaseImage*			GetPlayButtonImage(){return m_pPlayButtonImage;}
};

class	cSelectScroller:public cScroller//cTextButton
{
	int			m_iNumRow;
	int			m_iNumColumn;
	Vector2		m_vGap;
public:
	cSelectScroller(int e_iNumRow,int e_iNumColumn,Vector2 e_vGap);
	virtual ~cSelectScroller();
	virtual void			Init();
	void		SetCurrentWorkingObject(int e_iIndex);
};