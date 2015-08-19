#ifndef	_SHOW_FISH_COLLECTION_PHASE_H_
#define	_SHOW_FISH_COLLECTION_PHASE_H_

struct	sFishFileData
{

};

class	cFishCollection:public NamedTypedObject
{
public:
	float	m_fHeight;
	float	m_fWeight;
	int		m_iNumFished;
};


class	cFishCollectionScroller;
class	cFishDetailWindow;
class	cShowFishCollectionPhase:public ISAXCallback,public cSimplePhase
{
	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	void	ProcessFishScrollerData(TiXmlElement*e_pTiXmlElement);
	void	ProcessFishWindowData(TiXmlElement*e_pTiXmlElement);
	enum	eStatus
	{
		eS_COLLECTION_SCROLLER = 0,
		eS_DEATIL_WINDOW,
	};
	eStatus			m_eStatus;
	//
	cNamedTypedObjectVector<cFishCollection>		m_FishCollectionVector;
	cPuzzleImageUnit*								m_pUnknowFishPIUnit;
	cFishCollectionScroller*						m_pFishCollectionScroller;
	cFishDetailWindow*								m_pFishDetailWindow;
	//
	cMPDI*											m_pUIBG;
	cImageButton*									m_pBackButton;
public:
	cShowFishCollectionPhase();
	~cShowFishCollectionPhase();
	virtual	void	Init();//it would be called if the stage is changed,set m_bSatisfiedCondition as false!?
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	Destroy();
	virtual	void	DebugRender();
    virtual void    MouseDown(int e_iPosX,int e_iPosY);
    virtual void    MouseMove(int e_iPosX,int e_iPosY);
    virtual void    MouseUp(int e_iPosX,int e_iPosY);
	virtual	void*	GetData();
	virtual	void	FetchData(const WCHAR*e_strPhaseName,void*e_pData);	
};

#endif