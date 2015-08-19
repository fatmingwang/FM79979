#ifndef	_SCROLLABLE_MAP_AND_BUTTON_H_
#define	_SCROLLABLE_MAP_AND_BUTTON_H_

	class	cScrollableMapAndButton
	{
		cOrthogonalCamera*								m_pOrthogonalCamera;
		cRenderObject*							m_pMapBGMPDI;
		cClickMouseBehaviorVector<cClickMouseBehavior>*	m_pStagesButton;
		Vector4											m_vViewableRect;
		POINT											m_MouseDownPos;
	public:
		cScrollableMapAndButton(TiXmlElement*e_pElement);
		~cScrollableMapAndButton();
		void	Init();
		void	Update(float e_fElpaseTime);
		void	Render();
		void	MouseDown(int e_iPosX,int e_iPosY);
		void	MouseMove(int e_iPosX,int e_iPosY);
		void	MouseUp(int e_iPosX,int e_iPosY);
		const WCHAR*GetSelectStageName();
	};

#endif