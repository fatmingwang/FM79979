#ifndef	_STAGE_INFO_DIALOG_H_
#define	_STAGE_INFO_DIALOG_H_

	class	cStageInfoDialog
	{
		cButtonDialog*	m_pButtonDialog;
		//
		std::wstring	m_strPlaceName;
		Vector2			m_vPlaceNameShowPos;
		//
		float			m_fWaterDepth;
		Vector2			m_vWaterDepthShowPos;
		//
		std::wstring	m_strWaterZoneName;
		Vector2			m_vWaterZoneNameShowPos;
		//
		//int				m_iFishCost;
		//Vector2			m_vFishCostShowPos;
		//
		int				m_iFishKind;
		Vector2			m_vFishKindShowPos;
		//
		int				m_iFishesHooked;
		Vector2			m_vFishesHookedShowPos;
		//
		int				m_iStageActiveCost;
		Vector2			m_vStageActiveCostShowPos;
	public:
		cStageInfoDialog(TiXmlElement*e_pElement,TiXmlElement*e_pFishesShowProbabilityElement);
		~cStageInfoDialog();

		void			Update(float e_fElpaseTime);
		void			Render();

		void			MouseDown(int e_iPosX,int e_iPosY);
		void			MouseMove(int e_iPosX,int e_iPosY);
		void			MouseUp(int e_iPosX,int e_iPosY);
		cButtonDialog*	GetButtonDialog(){return m_pButtonDialog;}
	};

#endif