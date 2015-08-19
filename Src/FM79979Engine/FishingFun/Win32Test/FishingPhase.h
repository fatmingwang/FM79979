#ifndef	_FISHING_PHASE_H_
#define	_FISHING_PHASE_H_

	//class	cFishBehaviorManager;
	class	cStageData;
	class	cFishThread;
	class	cFishRodData;
	class	cFishingPhase:public ISAXCallback,public cSimplePhase
	{
		std::wstring	m_strStageName;
		cStageData*		m_pStageData;
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	public:
		cFishingPhase();
		~cFishingPhase();
		virtual	void	Init();//it would be called if the stage is changed,set m_bSatisfiedCondition as false!?
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
		virtual	void	Destroy();
		virtual	void	DebugRender();
		virtual	void	KeyUp(char e_cKey);
		virtual	void	KeyDown(char e_cKey);
		virtual	void	KeyPress(char e_cKey);
        virtual void    MouseDown(int e_iPosX,int e_iPosY);
        virtual void    MouseMove(int e_iPosX,int e_iPosY);
        virtual void    MouseUp(int e_iPosX,int e_iPosY);
		virtual	void*	GetData();
		virtual	void	FetchData(const WCHAR*e_strPhaseName,void*e_pData);
	};

#endif