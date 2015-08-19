#ifndef	_STAGE_SELECT_PHASE_H_
#define	_STAGE_SELECT_PHASE_H_

	class	cScrollableMapAndButton;
	class	cStageInfoDialog;
	class	cSelectFishRodFishThreadPhase;

	class	cStageSelectPhase:public ISAXCallback,public cSimplePhase
	{
		friend class	cSelectFishRodFishThreadPhase;
		virtual	void				HandleElementData(TiXmlElement*e_pTiXmlElement);
		std::wstring				m_strStageName;
		cScrollableMapAndButton*	m_pScrollableMapAndButton;
		cStageInfoDialog*			m_pStageInfoDialog;
		cButtonDialog*				m_pUIButton;
		cMPDI*						m_pUIBGMPDI;
		std::vector<std::wstring>	m_FishThreadsNameVector;
	public:
		cStageSelectPhase();
		~cStageSelectPhase();
		virtual	void	Init();//it would be called if the stage is changed,set m_bSatisfiedCondition as false!?
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
		virtual	void	Destroy();
		virtual	void	DebugRender();
		virtual	void	MouseDown(int e_iPosX,int e_iPosY);
		virtual	void	MouseMove(int e_iPosX,int e_iPosY);
		virtual	void	MouseUp(int e_iPosX,int e_iPosY);
		virtual	void*	GetData();
		virtual	void	FetchData(const WCHAR*e_strPhaseName,void*e_pData);
	};

#endif