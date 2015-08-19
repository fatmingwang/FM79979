#ifndef HEART_RECOVER_PHASER_H_
#define HEART_RECOVER_PHASER_H_

namespace FATMING_CORE
{
	class	cShockBoobsPainter;
}

template <class T> class cValueChangeFilter
{
	T*	m_pTarget;
	T	m_CheckPoint;
	UT::sTimeCounter	m_RefreshTimer;
	T	m_ChangedValue;
public:
	cValueChangeFilter(T*e_pValue,float e_fRefreshTime)
	{
		m_pTarget = e_pValue;
		m_RefreshTimer.SetTargetTime(e_fRefreshTime);
		m_RefreshTimer.SetLoop(true);
		m_CheckPoint = *m_pTarget;
	}
	void			Update(float e_felpaseTime)
	{
		m_RefreshTimer.Update(e_felpaseTime);
		if( m_RefreshTimer.bTragetTimrReached )
		{
			m_ChangedValue = m_CheckPoint-*m_pTarget;
			m_CheckPoint = *m_pTarget;
		}
	}
	T		GetChangedValue(){ return m_ChangedValue; }
	bool	IsReachCheckPoint(){ return m_RefreshTimer.bTragetTimrReached; }
};

class	cHeartRecoverPhase:public cSimplePhase,public ISAXCallback
{
	cResourceStamp			m_ResourceStamp;
	//
	virtual	void			HandleElementData(TiXmlElement*e_pTiXmlElement);
	void					ProcessShockParameterData(TiXmlElement*e_pTiXmlElement);
	//to strangle shock time and stretch
	float					m_fLastBigStregth;
	UT::sTimeCounter		m_NexShackTime;
	Vector2					m_vLastTimeDirection;
	//check reach target or not.
	cValueChangeFilter<Vector3>*	m_pShockDirectionValue;
	cValueChangeFilter<float>*		m_pShockForceValue;
	POINT					m_TouchDownPoint;
	float					m_fCurrentAddedStregth;
	float					m_fTargetStregth;
	float					m_fTimeReduceStrgeth;
	float					m_fShockAddStrgeth;
	void					DoShock(int e_iPointX1,int e_iPointY1,int e_iPointX2,int e_iPointY2);
	//
	std::string				m_strSelectedGirlName;
	cShockBoobsPainter*		m_pShockBoobsPainter;
	cImageButton*			m_pBackImageButton;
	cImageButton*			m_pStartConfirmButton;
	//if m_pCurrentMessenger is not null show messenger or play game
	cPhasePopUpMessager*	m_pCurrentMessenger;
	cPhasePopUpMessager*	m_pSuccessMessenger;
	cPhasePopUpMessager*	m_pFailedMessenger;
	cPhasePopUpMessager*	m_pTraningMessenger;
	//
	cMPDI*					m_pShowMPDI;
	cMPDI*					m_pHeartRecoverMPDI;
	UT::sTimeCounter		m_RestTimetoshockTC;
	cNumeralImage*			m_pCountDownNumeralImage;
public:
	cHeartRecoverPhase();
	~cHeartRecoverPhase();
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
	virtual	void	FetchData(const WCHAR*e_strPhaseName,void*e_pData);
	virtual	void*	GetData();
};

#endif