#ifndef	_MAIN_FUNCTION_BUTTON_H_
#define	_MAIN_FUNCTION_BUTTON_H_

class	cMainfunctionButton:public cSimplePhase
{
	std::wstring							m_strNextPhaseName;
	//
	cClickMouseBehaviorVector<cImageButton>	m_SelectionButton;
	cBasicSound*							m_pClickSound;
	std::string								m_strFileName;
public:
	cMainfunctionButton();
	~cMainfunctionButton();
	virtual	void	Destroy();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Init();
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseMove(int e_iX,int e_iY);
	virtual	const   WCHAR*	GetNextPhaseName();
	virtual	void	AddButton(TiXmlElement*e_pTiXmlElement);
	bool			IsCallDestroy();
};

#endif