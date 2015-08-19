#ifndef	_GIRL_INTRODUCING_PHASE_
#define	_GIRL_INTRODUCING_PHASE_

class	cGirlIntroducingPhase:public cSimplePhase,public ISAXCallback
{
	virtual	void			HandleElementData(TiXmlElement*e_pTiXmlElement);
	//
	cFMVVideo*				m_pFMPlayer;
	std::wstring			m_strVideoFileName;
	UT::sTimeCounter		m_TimeToHideButton;
	cImageButton*			m_pLeaveButton;
	bool					m_bFirstMouseUpForShowLeaveButton;
public:
	cGirlIntroducingPhase();
	~cGirlIntroducingPhase();
	//it would be called if the stage is changed.
	virtual	void			Init();
	virtual	void			Update(float e_fElpaseTime);
	virtual	void			Render();
	virtual	void			Destroy();
	virtual	void			DebugRender();
	virtual	void			MouseDown(int e_iX,int e_iY);
	virtual	void			MouseUp(int e_iX,int e_iY);
	virtual	void			MouseMove(int e_iX,int e_iY);
	virtual	const			WCHAR*	GetNextPhaseName();
	virtual	void			FetchData(const WCHAR*e_strPhaseName,void*e_pData);
};


#endif