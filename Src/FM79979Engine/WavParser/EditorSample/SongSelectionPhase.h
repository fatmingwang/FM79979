#pragma once

class cSonSelectionPhase
{
	cClickBehaviorDispatcher*		m_pClickBehaviorDispatcher;
public:
	cSonSelectionPhase();
	~cSonSelectionPhase();
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
};