#ifndef _MAGIC_TOWER_MAIN_MENU_PHASE_H_
#define _MAGIC_TOWER_MAIN_MENU_PHASE_H_

//GET_SET_DEC(DoButtonGoal_Callback,m_pDoButtonGoal_Callback,GetDoButtonGoal,SetDoButtonGoal);

class cMainMenuPhase:public cSimplePhase
{
	int					m_iMPDIBGIndex;//0
	int					m_iOptionsCount;//
	std::wstring		m_strMainMenuMPDIFileName;
	int					m_iCurrentSelectedIndex;
	cMPDI**				m_ppSelectionMPDI;
	cMPDI*				m_pBGMPDI;
	std::vector<DoButtonGoal_Callback*>	m_AllButtonFunction;
public:
	cMainMenuPhase(const WCHAR*e_strMPDIListFileName,int e_iOptionsCount,int e_iGCMPDIIndex = 0);
	~cMainMenuPhase();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Init();							//it would be called if the stage is changed.
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseHover(int e_iX,int e_iY);
	void			KeyDown(char e_char);
	virtual	const   WCHAR*	GetNextPhaseName();
	virtual	void*	GetData();
};

#endif