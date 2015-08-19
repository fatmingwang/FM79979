#ifndef	_HEART_RECOVERY_SELECT_GIRL_PHASE_H_
#define	_HEART_RECOVERY_SELECT_GIRL_PHASE_H_

class	cHeartRecoverSelectGirlPhase:public cSimplePhase,public ISAXCallback
{
	enum	eUpdateType
	{
		eUT_SELECT_GIRL,
		eUT_SELECT_SHAKE_OR_PRATICE,
	};
	cMainfunctionButton								m_MainfunctionButton;
	std::wstring									m_strNextPhaseName;
	//
	cResourceStamp									m_ResourceStamp;
	//
	std::wstring									m_strSelectedGirlName;
	cTabPage*										m_pScroller;
	//
	std::string										m_strFileName;
	virtual	void									HandleElementData(TiXmlElement*e_pTiXmlElement);
	cNamedTypedObjectVector<cRenderObject>	m_GirlUnlokImageVector;
	cMPDI*											m_pBGMPDI;
	cTabPage*										m_pPraticeOrShakeSelection;
	int												m_iHeartLimit;
	//
	eUpdateType										m_eUpdateType;
	void											SelectGirlUpdate(float e_fElpaseTime);
	void											ShakeOrPraticeUpdate(float e_fElpaseTime);
	//true for shake false for pratice
	bool											m_bShakeOrPratice;
public:
	cHeartRecoverSelectGirlPhase(const char*e_strFileName);
	~cHeartRecoverSelectGirlPhase();
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
	bool					IsShake(){ return m_bShakeOrPratice; }
};

#endif