#ifndef	_FISH_DETAIL_WINDOW_H_
#define	_FISH_DETAIL_WINDOW_H_


class	cFishDetailWindow
{
	cMPDI*						m_pBGMPDI;
	cRenderObjectVector*		m_pRenderObjectVector;
	cImageButton*				m_pFishDetailButton;
	cImageButton*				m_pCloseButton;
public:
	cFishDetailWindow(TiXmlElement*e_pElement);
	~cFishDetailWindow();
	//
    void    Update(float e_fElpaseTime);
    void    Render();
	//
	void	SetupData(const WCHAR*e_strFishName);
    void    MouseDown(int e_iPosX,int e_iPosY);
    void    MouseMove(int e_iPosX,int e_iPosY);
    void    MouseUp(int e_iPosX,int e_iPosY);
};

#endif