//#ifndef _CAMERA_BEHAVIOR_H_
//#define _CAMERA_BEHAVIOR_H_
//
//
//template<class T>
//struct	sTwoPointTime
//{
//	T		Value1;
//	T		Value2;
//	Vector2	vPointTime;
//	float	GetLERP(float e_fTime)
//	{
//		float	l_fLength = vPointTime.x-vPointTime.x;
//		float	l_fProgress = (e_fTime-vPointTime.x);
//		return l_fProgress/l_fLength;
//	}
//	T	GetLertValue(float e_fTime)
//	{
//		return LERP<T>(Value1,Value2,GetLERP(e_fTime));
//	}
//};
//
//template<class T>
//struct	sLinerDataList
//{
//	int								UpdateCurrentWorkingIndex(float e_fWorkingTime)
//	{
//		size_t	l_iSize = m_DataList.size();
//		for( size_t i=0;i<l_iSize;++i )
//		{
//			if( e_fWorkingTime <=m_DataList[i].vPointTime.y )
//			{
//				m_iCurrentWorkingIndex = i;
//				return;
//			}
//		}
//		m_iCurrentWorkingIndex = -1;
//	}
//
//	std::vector<sTwoPointTime<T>>	m_DataList;
//	T								m_CurrentData;
//	float							m_fStartTime;
//	float							m_fEndTimeTime;
//	int								m_iCurrentWorkingIndex;
//	sLinerDataList(){ m_iCurrentWorkingIndex = -1; }
//	~sLinerDataList(){}
//	//check time,and set start time and end time
//	void	AddDataToBack(sTwoPointTime<T> e_Data)
//	{
//		m_DataList.push_back(e_Data);
//		if(!TimeCheck())
//		{
//			assert(0&&"TimeCheck error");
//		}
//	}
//	//check valdiate and reset start and end time
//	bool	TimeCheck()
//	{
//		float	l_fMin = 0.f;
//		float	l_fMax = 0.f;
//		size_t	l_iSize = m_DataList.size();
//		if( l_iSize )
//		{
//			l_fMin = m_DataList[0].vPointTime.x;
//			l_fMax = m_DataList[m_DataList.size()-1].vPointTime.y;
//			for( size_t i=1;i<l_iSize;++i )
//			{
//				if( l_fMin>m_DataList[i].vPointTime.x )
//					return false;
//				if( l_fMax<m_DataList[i].vPointTime.y )
//					return false;
//			}
//		}
//		return true;
//	}
//	void	Start(float e_fStartTiem = 0.f)
//	{
//		m_iCurrentWorkingIndex = -1;
//		Update(e_fWorkingTime);
//		if( m_DataList.size() )
//			m_CurrentData = m_DataList[0].Value1;
//	}
//	void	UpdateByGlobalTime(float e_fWorkingTime)
//	{
//		UpdateCurrentWorkingIndex(e_fWorkingTime);
//		if( m_iCurrentWorkingIndex != -1 )
//			m_CurrentData = m_DataList[m_iCurrentWorkingIndex].GetLertValue(e_fWorkingTime);
//	}
//	void	Update(float e_fWorkingTime)
//	{
//		//cLinerDataList<float>	l_fTTest;
//		if( m_fStartTime>e_fWorkingTime||m_fEndTimeTime<e_fWorkingTime )
//			return;
//		if( m_iCurrentWorkingIndex == -1 )
//		{
//			UpdateCurrentWorkingIndex(e_fWorkingTime);
//			if( m_iCurrentWorkingIndex == -1 )
//				return;
//		}
//		else
//		if( m_DataList[m_iCurrentWorkingIndex].vPointTime.y>= e_fWorkingTime)
//		{
//			++m_iCurrentWorkingIndex;
//		}
//		m_CurrentData = m_DataList[m_iCurrentWorkingIndex].GetLertValue(e_fWorkingTime);
//	}
//};
//
//enum	eCameraType
//{
//	eCT_PESPECTIVE = 0,
//	eCT_ORTHGNAL,
//	eCT_MAX,
//};
//
////pos,view rect,type(2d 3d),angle
////view port,scissor,view ration,
//class	cCameraBehavior
//{
//	float				m_fCurrentTime;
//	eCameraType			m_eCameraType;
//	cOrthogonalCamera*	m_pOrthogonalCamera;
//	cFrameCamera*		m_pFrameCamera;
//	UT::sTimeCounter	m_Time;
//
//	sTwoPointTime<Vector3>	*m_pPos;
//	sTwoPointTime<Vector3>	*m_pAngle;
//	sTwoPointTime<Vector4>	*m_pViewRect;
//	cCameraBehavior(cOrthogonalCamera*e_pOrthogonalCamera,cFrameCamera*e_pFrameCamera,eCameraType e_eCameraType)
//	{
//		m_eCameraType = e_eCameraType;
//		m_pOrthogonalCamera = e_pOrthogonalCamera;
//		m_pFrameCamera = e_pFrameCamera;
//		m_pPos = 0;
//		m_pAngle = 0;
//		m_pViewRect = 0;
//		m_fCurrentTime = 0.f;
//	}
//	~cCameraBehavior()
//	{
//		SAFE_DELETE(m_pPos);
//		SAFE_DELETE(m_pAngle);
//		SAFE_DELETE(m_pViewRect);
//	}
//	bool	IsDone()
//	{
//		bool	l_bDone = true;
//		if(m_pPos)
//		{
//			if(this->m_fCurrentTime<m_pPos->vPointTime.y)
//				l_bDone = false;
//		}
//		if(m_pAngle)
//		{
//			if(this->m_fCurrentTime<m_pAngle->vPointTime.y)
//				l_bDone = false;
//		}
//		if(m_pViewRect)
//		{
//			if(this->m_fCurrentTime<m_pViewRect->vPointTime.y)
//				l_bDone = false;
//		}
//		return l_bDone;
//	}
//	void	SetPosData(Vector3 e_vPos1,Vector3 e_vPos2,float e_fDuration)
//	{
//		if( !m_pPos )
//			m_pPos = new sTwoPointTime<Vector3>;
//		m_pPos->Value1 = e_vPos1;
//		m_pPos->Value2 = e_vPos2;
//		m_pPos->vPointTime.x = 0.f;
//		m_pPos->vPointTime.y = e_fDuration;
//	}
//	void	SetAngleData(Vector3 e_vAngle1,Vector3 e_vAngle2,float e_fDuration)
//	{
//		if( !m_pAngle )
//			m_pAngle = new sTwoPointTime<Vector3>;
//		m_pAngle->Value1 = e_vAngle1;
//		m_pAngle->Value2 = e_vAngle2;
//		m_pAngle->vPointTime.x = 0.f;
//		m_pAngle->vPointTime.y = e_fDuration;
//	}
//	void	SetViewRectData(Vector4 e_vViewRect1,Vector4 e_vViewRect2,float e_fDuration)
//	{
//		if( !m_pViewRect )
//			m_pViewRect = new sTwoPointTime<Vector4>;
//		m_pViewRect->Value1 = e_vViewRect1;
//		m_pViewRect->Value2 = e_vViewRect2;
//		m_pViewRect->vPointTime.x = 0.f;
//		m_pViewRect->vPointTime.y = e_fDuration;
//	}
//
//	void	Update(float e_fElpaseTime)
//	{
//		m_fCurrentTime+=e_fElpaseTime;
//		if(m_pPos)
//			m_pPos->GetLertValue(m_fCurrentTime);
//		if(m_pAngle)
//			m_pAngle->GetLertValue(m_fCurrentTime);
//		if(m_pViewRect)
//			m_pViewRect->GetLertValue(m_fCurrentTime);
//	}
//	void	Render()
//	{
//		if( m_eCameraType == eCT_PESPECTIVE )
//		{
//		
//		}
//		else
//		if( m_eCameraType == eCT_ORTHGNAL )
//		{
//		
//		}
//	}
//};

//#endif