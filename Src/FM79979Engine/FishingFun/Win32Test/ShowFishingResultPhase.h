#ifndef	_SHOW_FISHING_RESULT_H_
#define	_SHOW_FISHING_RESULT_H_

	class	cFishTag;
	class	cFishBehavior;
	class	cShowFishingResultPhase:public ISAXCallback,public cSimplePhase
	{
		class	cCaughtFishData:public NamedTypedObject
		{
		public:
			int				m_iNumCaught;
			float			m_fLength;
			int				m_iTotalCost;
			cAnimationMesh*	m_pFishModel;
		};
		cNamedTypedObjectVector<cCaughtFishData>			m_CaughtFishDataVector;
		TiXmlElement*										m_pTemplateElement;
		cScroller*											m_pScroller;
		cMPDI*												m_pUIBGMPDI;
		cClickMouseBehaviorVector<cImageButton>*			m_pUIButton;
	public:
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	public:
		cShowFishingResultPhase();
		~cShowFishingResultPhase();
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