#pragma once

class cTimeLineRangeChart;
class cQuickFFTDataFrequencyFinder;
class cPerformMusicPhaseUI;
class cPerformScoreCalculate;
class cPerformMusicPhase:public cSimplePhase
{
	bool							m_bPasue;
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

class cPerformMusicPhaseUI:public cRenderObject,public cMessageSender
{
	cGlyphFontRender*					m_pC4Text;
	cGlyphFontRender*					m_pG4Text;
	//event
	cMPDI*								m_pGreatMPDI;
	cMPDI*								m_pOopsMPDI;
	//now just for test
	cMPDI*								m_pTouchMPDI;
	bool								TimeOverEventFire(void*e_pData);
	bool								TuneMatchedFire(void*e_pData);
//
	//cGlyphFontRender*					m_pScoreFont;
	//int									m_iCurrentScore;
	cNumeralImage*						m_pCurrentScoreNumeial;
	cPerformScoreCalculate*				m_pPerformScoreCalculate;
	cNumeralImage*						m_pScoreNumeial;
	cMPDI*								m_pBGMPDI;
	cBaseImage*							m_pMusicSheetImage;//	SheetMusic.png
	cBehaviorObjectList<cMPDI>			m_LaserMPDILeftVector;
	cBehaviorObjectList<cMPDI>			m_LaserMPDIRightVector;
	//
	cLazyClickBehaviorAndRenderObject*	m_pPauseButton;
	cLazyClickBehaviorAndRenderObject*	m_pScoreButton;
	cPerformMusicPhase*					m_pPerformMusicPhase;
	cRenderObject*						m_pBG;
	//cGlyphFontRender*					m_pScoreText;
	cClickBehavior*						m_pTimeControlButton;
	void								GamePause();
public:
	cPerformMusicPhaseUI(cPerformMusicPhase*e_pPerformMusicPhase,cMPDI*e_pBGMPDI);
	~cPerformMusicPhaseUI();
	bool			GenerateResources(cClickBehaviorDispatcher*e_pClickBehaviorDispatcher);
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	//0,left 1 right
	void			ShotLaser();
//	virtual	void	RenderLaser();

};