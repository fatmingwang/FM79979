#ifndef	_GAME_RULE_PHASE_H_
#define	_GAME_RULE_PHASE_H_

	class	cGameRulePhase:public ISAXCallback,public cSimplePhase
	{
		cMPDI*				m_pBGMPDI;
		cScroller*			m_pScroller;
		cImageButton*		m_pBackButton;
		std::wstring		m_strInPhaseName;
		cPuzzleImageUnit*	m_pBG;
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	public:
		cGameRulePhase();
		~cGameRulePhase();
        //set data to correct
        void			Init();
        void			Update(float e_fElpaseTime);
        void			Render();
        void			DebugRender();
		//
        void			MouseMove(int e_iPosX,int e_iPosY);
        void			MouseDown(int e_iPosX,int e_iPosY);
        void			MouseUp(int e_iPosX,int e_iPosY);
		virtual	const   WCHAR*	GetNextPhaseName();
		virtual	void	FetchData(const WCHAR*e_strPhaseName,void*e_pData);
	};

#endif