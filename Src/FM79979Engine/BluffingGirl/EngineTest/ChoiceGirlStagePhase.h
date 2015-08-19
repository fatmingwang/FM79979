#ifndef _CHOICE_GIRL_STAGE_DATA_H_
#define _CHOICE_GIRL_STAGE_DATA_H_
	
//after select girl into this menu to select which stage is going to play
class	cChoiceGirlStagePhase:public cSimplePhase,public ISAXCallback
{
	enum	eSelectStageTYpe
	{
		eSST_SELECT_STAGE = 0,
		eSST_SELECT_VIDEO_OR_GAME,
		eSST_INTO_GAME,
	};
	eSelectStageTYpe				m_eSelectStageTYpe;
	cPhasePopUpMessager*			m_pIntoGamePhasePopUpMessager;
	cImageButton*					m_pPlayGameButton;
	cImageButton*					m_pWatchVideoButton;
	bool							m_bCurrentVideoCouldBeWatch;
	bool							m_bCurrentGameCouldBePlay;
	int								m_iLatestGameCouldBePlay;
public:
	class	cStageData:public NamedTypedObject
	{
	public:
		int				m_iTotalRound;
		int				m_iWinCount;
		std::wstring	m_strStageMame;
		std::wstring	m_strBGImage;
		std::wstring	m_strVideoName;
		std::wstring	m_strGameSetupFileName;
		//video
		std::wstring	m_strGirlIntroducingVideoFileName;
		std::wstring	m_strWinVideoFileName;
		std::wstring	m_strLoseVideoFileName;
		std::vector<std::wstring>m_strShakeDiceVideoFileNameVector;
		bool			m_bLastStage;//if true,write a all stage clear data
		cStageData(){ m_iTotalRound = 3; m_iWinCount = 2; }
		~cStageData(){}
		void			Assign(cStageData*e_pStageData)
		{
			e_pStageData->m_strStageMame = m_strStageMame;
			e_pStageData->m_strBGImage = m_strBGImage;
			e_pStageData->m_strVideoName = m_strVideoName;
			e_pStageData->m_strGameSetupFileName = m_strGameSetupFileName;
			e_pStageData->m_bLastStage = m_bLastStage;//if true,write a all stage clear data
			e_pStageData->m_strGirlIntroducingVideoFileName = m_strGirlIntroducingVideoFileName;
			e_pStageData->m_strWinVideoFileName = m_strWinVideoFileName;
			e_pStageData->m_strLoseVideoFileName = m_strLoseVideoFileName;
			e_pStageData->m_strShakeDiceVideoFileNameVector = m_strShakeDiceVideoFileNameVector;
			e_pStageData->m_iTotalRound = m_iTotalRound;
			e_pStageData->m_iWinCount = m_iWinCount;
		}
	};
protected:
	//
	cMPDI*					m_pBGMPDI;
	cMainfunctionButton		m_MainfunctionButton;
	//
	cNamedTypedObjectVector<cStageData>				m_StageDataVector;
	cClickMouseBehaviorVector<cImageButton>			m_SelectedGirlImage;
	cSubMPDI*										m_pGirlImageCover;
	std::wstring									m_strNextPhaseName;
	//
	cResourceStamp									m_ResourceStamp;
	//
	cStageData*										m_pSelectedStageData;
	cPuzzleImage*									m_pChoiceGirlStagePI;
	cTabPage*										m_pScroller;
	std::vector<bool>								m_bStagePlayable;
	cNamedTypedObjectVector<cRenderObject>			m_GirlUnlokImageVector;
	//
	std::string										m_strFileName;
	void											ProcessStageData(TiXmlElement*e_pTiXmlElement);
	void											ProcessSelectedGirlImageSubMPDIData(TiXmlElement*e_pTiXmlElement);
	virtual	void									HandleElementData(TiXmlElement*e_pTiXmlElement);
	//
	void											ParseGirlStagePhseData(const char*e_strFileName);
	void											ParseGameVideoButtonData(TiXmlElement*e_pElement);
	//
	void											UpdateSelectStage(float e_fElpaseTime);
	void											UpdateIntoGame(float e_fElpaseTime);
public:
	cChoiceGirlStagePhase();
	~cChoiceGirlStagePhase();

	virtual	void	Destroy();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Init();							//it would be called if the stage is changed.
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseMove(int e_iX,int e_iY);
	virtual	const   WCHAR*	GetNextPhaseName();
	//return cBluffingDiceGame data file name.
	virtual	void*	GetData();
	virtual	void	FetchData(const WCHAR*e_strPhaseName,void*e_pData);

	std::wstring	m_strCurrentSelectedGirlNameAndStageName;
	std::wstring	m_strCurrentSelectedGirlName;
	cStageData*		GetCurrentStageData();
};

#endif