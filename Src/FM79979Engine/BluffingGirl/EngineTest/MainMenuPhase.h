#ifndef _MAIN_MENU_PHASE_H_
#define _MAIN_MENU_PHASE_H_

class cMainMenuPhase:public cSimplePhase,public ISAXCallback
{
	cMainfunctionButton						m_MainfunctionButton;
	cResourceStamp							m_ResourceStamp;
	std::wstring							m_strNextPhaseName;
	virtual	void							HandleElementData(TiXmlElement*e_pTiXmlElement);
	void									ProcessPhaseData(TiXmlElement*e_pTiXmlElement);
	//
	cDoAllMouseBehaviorList<cImageButton>	m_SelectionButton;
	cMPDI*									m_pBGMPDI;
	cBasicSound*							m_pClickSound;
	std::string								m_strFileName;
public:
	cMainMenuPhase(const char*e_strMainMenuFileName);
	~cMainMenuPhase();
	virtual	void	Destroy();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Init();							//it would be called if the stage is changed.
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseMove(int e_iX,int e_iY);
	void			KeyDown(char e_char);
	virtual	const   WCHAR*	GetNextPhaseName();
	virtual	void*	GetData();
};

#endif