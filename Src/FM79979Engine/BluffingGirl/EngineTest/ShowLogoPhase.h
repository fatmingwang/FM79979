#ifndef _SHOW_LOGO_PHASE_H_
#define _SHOW_LOGO_PHASE_H_

class cUnpackExpansionPack;

class cShowLogoPhase:public cSimplePhase,public ISAXCallback
{
	cUnpackExpansionPack*	m_pUnpackExpansionPack;
	cFUThreadPool			m_ThreadPool;
	cMPDI*					m_pBGMPDI;
	cResourceStamp			m_ResourceStamp;
public:
	cShowLogoPhase();
	~cShowLogoPhase();
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
	bool			m_bGotoMainPhase;
};

#endif