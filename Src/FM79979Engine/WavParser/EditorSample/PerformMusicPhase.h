#pragma once

class cTimeLineRangeChart;
class cQuickFFTDataFrequencyFinder;
class cPerformMusicPhaseUI;
class cPerformMusicPhase:public cSimplePhase
{
	friend	class					cPerformMusicPhaseUI;
	cPerformMusicPhaseUI*			m_pPerformMusicPhaseUI;
	//
	cClickBehaviorDispatcher*		m_pClickBehaviorDispatcher;
	cTimeLineRangeChart*			m_pTimeLineRangeChart;
	void							FetchData(const wchar_t*e_strPhaseName,void*e_pData);
	std::string						m_strMusicFileName;
	void							GenerateResources();
//	cRenderObject;
//	cRenderNode;
public:
	cPerformMusicPhase();
	virtual ~cPerformMusicPhase();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	Destroy();
	virtual	void	DebugRender();
	virtual	void*	GetData();
	virtual	void	KeyUp(char e_cKey);
    //first time into
    virtual void    MouseDown(int e_iPosX,int e_iPosY);
    //horver and move
    virtual void    MouseMove(int e_iPosX,int e_iPosY);
    //
    virtual void    MouseUp(int e_iPosX,int e_iPosY);
};


class cPerformMusicPhaseUI:public cRenderObject
{
	cLazyClickBehaviorAndRenderObject*m_pPause;
	cLazyClickBehaviorAndRenderObject*m_pScore;
	cPerformMusicPhase*			m_pPerformMusicPhase;
	cBaseImage*					m_pBG;
	cClickBehavior*				m_pTimeControlButton;
	void						GamePause();
public:
	cPerformMusicPhaseUI(cPerformMusicPhase*e_pPerformMusicPhase);
	~cPerformMusicPhaseUI();
	bool			GenerateResources(cClickBehaviorDispatcher*e_pClickBehaviorDispatcher);
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();

};