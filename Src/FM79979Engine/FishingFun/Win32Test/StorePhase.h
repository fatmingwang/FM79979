#ifndef	_IAB_MENU_H_
#define	_IAB_MENU_H_

	class	cIABSetup;
	//
	class	cStorePhase:public ISAXCallback,public cSimplePhase
	{
		std::wstring	m_strSelectedItemName;
		cMPDI*			m_pBGMPDI;
		//
		cPuzzleImage*	m_pIABPI;
		cScroller*		m_pScroller;
		void			ProcessIABMenuData(TiXmlElement*e_pTiXmlElement);
		void			ProcessScrollerData(TiXmlElement*e_pTiXmlElement);
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
		//
		cIABSetup*		m_pIABSetup;
		bool			m_bWaitForRespond;
		void			DisableFirstBuy();
	public:
		cStorePhase();
		~cStorePhase();
        //set data to correct
        void			Init();
		void			SetWaitForRespnd(bool e_bWaitForRespnd);
		bool			IsWaitForRespnd(){ return m_bWaitForRespond; }
        void			Update(float e_fElpaseTime);
        void			Render();
        void			DebugRender();
		//
        void			MouseMove(int e_iPosX,int e_iPosY);
        void			MouseDown(int e_iPosX,int e_iPosY);
        void			MouseUp(int e_iPosX,int e_iPosY);
		bool			IsPurchaseItem();
		//const			WCHAR*GetPurchaseProductName();
		cIABSetup*		GetIABSetup(){return m_pIABSetup;}
		virtual	void	FetchData(const WCHAR*e_strPhaseName,void*e_pData);
		virtual	const   WCHAR*	GetNextPhaseName();
		std::wstring	m_strLastPhaseName;
		void			MakeItemPurchase(const WCHAR*e_strProductName);
		void			MakeItemPurchase();
	};
#endif