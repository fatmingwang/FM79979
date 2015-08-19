#ifndef _CHOICE_GIRL_PHASE_H_
#define _CHOICE_GIRL_PHASE_H_

class	cDownloadContentPhase;

class	cChoiceGirlPhase:public cSimplePhase,public ISAXCallback
{
	//
	TiXmlElement*			m_pCheckFileXmlElement;
	TiXmlElement*			m_pDownloadPopupMessageXmlElement;
	cDownloadContentPhase*	m_pDownloadContentPhase;
	bool					DoCheckVideoFile();
	//
	cMPDI*					m_pBGMPDI;
	cMainfunctionButton		m_MainfunctionButton;
	std::wstring			m_strNextPhaseName;
	//
	cResourceStamp			m_ResourceStamp;
	//
	std::wstring			m_strSelectedGirlName;
	cTabPage*				m_pScroller;
	std::string				m_strFileName;
	virtual	void			HandleElementData(TiXmlElement*e_pTiXmlElement);
	cNamedTypedObjectVector<cRenderObject>	m_GirlUnlokImageVector;
public:
	cChoiceGirlPhase(const char*e_strFileName);
	~cChoiceGirlPhase();
	virtual	void			Destroy();
	virtual	void			Update(float e_fElpaseTime);
	virtual	void			Init();							//it would be called if the stage is changed.
	virtual	void			Render();
	virtual	void			DebugRender();
	virtual	void			MouseDown(int e_iX,int e_iY);
	virtual	void			MouseUp(int e_iX,int e_iY);
	virtual	void			MouseMove(int e_iX,int e_iY);
	virtual	const	WCHAR*	GetNextPhaseName();
	//get selected girl name
	virtual	void*			GetData();
	const WCHAR*			CurrentSelectGirlName();
	void					RecheckUnLockGirl();
};

#endif