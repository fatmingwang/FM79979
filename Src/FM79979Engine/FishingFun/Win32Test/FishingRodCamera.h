#ifndef	_FISH_ROD_CAMERA_H_
#define	_FISH_ROD_CAMERA_H_

	class	cStageData;
	enum	eFishRodStatus
	{
		eFRS_NONE = 0,
		eFRS_SINKING,
		eFRS_FLOATING,
		eFRS_DONE,
	};

	class	cFishingRodCamera
	{
		friend class			cStageData;
		eFishRodStatus			m_eFishRodStatus;
		//
		float					m_fCurrentPos;
		float					m_fTotalDistance;
		Vector2					m_vStartPos;
		//float					m_fDownSpeed;
		//float					m_fUpSpeed;
		sTimeCounter			m_SinkingTime;
		sTimeCounter			m_FloatingTime;
		//
		cOrthogonalCamera*		m_pOrthogonalCamera;
	public:
		cFishingRodCamera(TiXmlElement*e_pTiXmlElement);
		~cFishingRodCamera();
		void	Init();
		void	Update(float e_fElpaseTime);
		void	Render();
		void	DebugRender();
		void	ChangeSinkAndFloatTime(float e_fSink,float e_fFloat);
		bool	IsSinking(){return m_eFishRodStatus ==eFRS_SINKING;}
		void	SetTotalDistance(float e_fDis){m_fTotalDistance = e_fDis;}
		float	GetRestTimeToTop(){ return m_FloatingTime.fRestTime; }
		void	SetStatus(eFishRodStatus e_iStatus);
		int		GetStatus(){ return m_eFishRodStatus; }
		Vector4	GetViewRect();
	};

#endif