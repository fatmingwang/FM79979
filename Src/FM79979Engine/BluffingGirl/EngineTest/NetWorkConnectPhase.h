#ifndef	_NETWORK_CONNECT_PHASE_H_
#define	_NETWORK_CONNECT_PHASE_H_
	
	class	cNetWorkConnectPhase:public cSimplePhase,public ISAXCallback
	{
		friend class									cBluffingDiceMultiPlayerGame;
		friend void										NewPlayerAdded(void*e_pData);
		//
		void											ProcessConnectTypeData(TiXmlElement*e_pTiXmlElement);
		void											ProcessServerData(TiXmlElement*e_pTiXmlElement);
		void											ProcessUIData(TiXmlElement*e_pTiXmlElement);
		virtual	void									HandleElementData(TiXmlElement*e_pTiXmlElement);
		//step1
		std::string										m_strShowSelectionMesasage;
		Vector2											m_vShowSelectionMesasage;

		cButtonDialog*									m_pCreateModeDialog;
		cButtonDialog*									m_pSelectPlayerCountDialog;
		cMPDI*											m_pWaitConnectMPDI;


		bool											m_bIAmServer;//false is client
		//
		std::string										m_strShowWaitingMesasage;
		Vector2											m_vShowWaitingMesasagePos;
		//
		void											WakeIPInput();
		bool											GetIP();
		//if choice as client
		std::string										m_strTargetIP;
		std::string										m_strMyIP;
		int												m_iTotalPlayer;
		int												m_iConnectedPlayerCount;
		int												m_iPlayerIDIndex;
		bool											m_bDoPlayerDisconnect;
		//
		enum	eConnectStep
		{
			eCS_SELECT_SERVER_OR_CLIENT,
			eCS_SELECT_PLAYER_COUNT,
			eCS_WAIT_FOR_CLIENTS,
			eCS_CONNECT_TO_SERVER,
			eCS_NETWORK_ERROR,
			eCS_INPUT_SERVER_IP,
			eCS_GO_TO_NEXT_PHASE,
		};
		eConnectStep			m_eConnectStep;
		void					ProcessMessage(float e_fElpaseTime);
		//
		cImageButton*			m_pBackButon;
	public:
		cNetWorkConnectPhase();
		~cNetWorkConnectPhase();

		virtual	void			Init();
		virtual	void			Destroy();
		virtual	void			Update(float e_fElpaseTime);
		virtual	void			Render();
		virtual	void			DebugRender();
		virtual	void			MouseDown(int e_iX,int e_iY);
		virtual	void			MouseUp(int e_iX,int e_iY);
		virtual	void			MouseMove(int e_iX,int e_iY);
		virtual	void			KeyUp(char e_cKey);
		virtual	void			KeyDown(char e_cKey);
		virtual	void			KeyPress(char e_cKey);
		virtual	const	WCHAR*	GetNextPhaseName();
		int						GetConnectedPlayerCount(){return m_iConnectedPlayerCount;}
		void					SetConnectedPlayerCount(int e_iCount){m_iConnectedPlayerCount = e_iCount;}
		void					DoPlayerDisconnect();
		virtual	void*			GetData();
		void					SetDoPlayerDisconnect(){ m_bDoPlayerDisconnect = true; }
		bool					IsDoPlayerDisconnect(){ return m_bDoPlayerDisconnect; }
	};

#endif