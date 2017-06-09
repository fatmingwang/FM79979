#pragma once

class cTimeLineRangeChart;
class cQuickFFTDataFrequencyFinder;
class cPerformMusicPhase:public cSimplePhase
{
	cClickBehaviorDispatcher*		m_pClickBehaviorDispatcher;
	cTimeLineRangeChart*			m_pTimeLineRangeChart;
	void							FetchData(const wchar_t*e_strPhaseName,void*e_pData);
	std::string						m_strMusicFileName;
	cBaseImage*						m_pBG;
	bool							m_bGamePause;
	void							GenerateButtons();
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
	friend class cPerformMusicPhase;
	cMPDI*m_PauseMPDI;
	cMPDI*m_ScoreMPDI;
	cClickBehavior*m_pPauseClickBehavior;
	cClickBehavior*m_pScoreClickBehavior;
public:
	cPerformMusicPhaseUI();
	~cPerformMusicPhaseUI();
	bool			GenerateButtons(cClickBehaviorDispatcher*e_pClickBehaviorDispatcher);
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
};