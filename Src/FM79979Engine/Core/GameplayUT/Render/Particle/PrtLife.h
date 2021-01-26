#pragma once
namespace FATMING_CORE
{
	//===========================================
	//Life Init
	//it should have a infinity life Sphere
	//===========================================	
	class cPrtLifeInitrSetLife:public cParticleBase
	{
	private:
		GETP_SET_DEC(float,m_fLifeMin,GetLifeMin,SetLifeMin);
		GETP_SET_DEC(float,m_fLifeRange,GetLifeRange,SetLifeRange);
		GETP_SET_DEC(bool,m_bRandom,IsRandom,SetRandom);
	public:
		DEFINE_TYPE_INFO()
		explicit cPrtLifeInitrSetLife() throw():m_fLifeMin(0.5f), m_fLifeRange(5.f),m_bRandom(true) {}
		virtual ~cPrtLifeInitrSetLife(){}
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)override
		{
			if( m_bRandom )
			{
				e_pParticleData->fLifespan = m_fLifeMin + (frand(0.f,m_fLifeRange));
				e_pParticleData->fOriginaLifeSpan = e_pParticleData->fLifespan;
			}
			else
			{
				e_pParticleData->fLifespan = m_fLifeMin;
				e_pParticleData->fOriginaLifeSpan = m_fLifeMin;
			}
		}
		inline void SetLifeRange(const float& e_fLifeMin, const float& e_fLifeMax) throw() 
		{
			assert(e_fLifeMax>e_fLifeMin?true:false&&"Max smaller than Min");
			m_fLifeMin = e_fLifeMin;
			m_fLifeRange = e_fLifeMax + 1 - e_fLifeMin;
		}
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*  Clone()override;
		virtual char*	GetDataInfo()override;
		//this one for output data value by "," to separate value
		virtual char*	GetOutputDataString()override;
		//input the output data string,and analyze it
		virtual bool	SetDataByDataString(char*e_pString)override;
	};	
	//===========================================
	//Life Action
	//it should intergrated into emiter
	//===========================================	
	class cPrtLifeActDyingByGameTime:public cParticleBase
	{
	public:
		DEFINE_TYPE_INFO()
		explicit cPrtLifeActDyingByGameTime(){}
		virtual ~cPrtLifeActDyingByGameTime(){}
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)override
		{
			e_pParticleData->fLifespan -= e_fElpaseTime;
			e_pParticleData->fPastTime = e_pParticleData->fOriginaLifeSpan-e_pParticleData->fLifespan;
		}
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*  Clone()override
		{
			cPrtLifeActDyingByGameTime* l_p = new cPrtLifeActDyingByGameTime; 
			l_p->SetName(GetName());
			return l_p; 
		};
	};

	//class cPrtLifeActRebirth:public cParticleBase
	//{
	//	//we need proior position to trace,the rebirth where it should be
	//	Vector3	m_vPriorPosition;
	//	//to test dis time arrive or not
	//	float	m_fElpaseTimeCompare;
	//	int m_iCount;
	//	int		m_iCountCompaer;
	//	//indicate which one particle name will be used
	//	wchar_t	m_sParticleName[MAX_PATH];
	//	//this one usually reference Life Time MinTime,because sometimes the min time is too small,so it won't happen
	//	GET_SET_DEC(float,m_fDeadlineTime,GetDeadlineTime,SetDeadlineTime);
	//	GET_SET_DEC(Vector3,m_vRelatedPos,GetRelatedPos,SetRelatedPos);
	//	GET_SET_DEC(bool,m_bImmediateDied,GetImmediateDied,SetImmediateDied);
	//	GET_SET_DEC(float,m_fElpaseTimeToShot,GetElpaseTimeToShot,SetElpaseTimeToShot);
	//	//if this set to true than we will get prior position and fille the tracked particle through
	//	GET_SET_DEC(bool,m_bFillAllTrace,IsFillAllTrace,SetFillTrace);
	//	//when the FillAllTrace is true we will use m_iFillCount to set how many particle emiter we should called
	//	GET_SET_DEC(int,m_iFillCount,GetFillCount,SetFillCount);
	//public:
	//	cPrtLifeActRebirth(){ memset(m_sParticleName,0,sizeof(m_sParticleName)); m_fDeadlineTime = 0.1f; m_iCount = 1; m_bImmediateDied = true; m_fElpaseTimeToShot = 0.0f; m_fElpaseTimeCompare = 0.f; m_iCountCompaer; m_iFillCount = 1; m_bFillAllTrace = false; }
	//	virtual ~cPrtLifeActRebirth(){}
	//	virtual inline void PrepareAction(sParticleData*e_pParticleData) throw(){ m_iCountCompaer = m_iCount; m_vPriorPosition = e_pParticleData->vPos; }
	//	int	GetCount(){ return m_iCount; }
	//	void	SetCount(int e_iCount){ m_iCount = m_iCountCompaer = e_iCount; }
	//	wchar_t*	GetParticleName(){ return m_sParticleName; }
	//	void	SetParticleName(wchar_t*e_pName){ swprintf_s(m_sParticleName,MAX_PATH,L"%s",e_pName); }
	//	DEFINE_TYPE_INFO()
	//	virtual inline void	operator()(sParticleData*e_pParticleData,float e_fElpaseTime)
	//	{//if need to shot still by count value bigger 0,and deadlinetime
	//		if( e_pParticleData->fLifespan<=m_fDeadlineTime&&m_iCountCompaer )
	//		{
	//			//if distance time bigger 0 than we need to have a elpase time to shot
	//			m_fElpaseTimeCompare+=e_fElpaseTime;
	//			if( m_fElpaseTimeToShot<=m_fElpaseTimeCompare )
	//			{
	//				m_fElpaseTimeCompare = 0.f;
	//				m_iCountCompaer--;
	//				if( this->m_bFillAllTrace )
	//				{
	//					Vector3	l_vDistance = e_pParticleData->vVelocity - m_vPriorPosition;
	//					Vector3	l_vDir = l_vDistance/(float)m_iFillCount;
	//					for( int i=0;i<m_iFillCount;++i )
	//					{
	//						g_pPrt->GetEmiterManager()->GetWaitForEmitParticle()->push_back(this->m_sParticleName);
	//						g_pPrt->GetEmiterManager()->GetWaitForEmitParticlePos()->push_back(m_vRelatedPos+e_pParticleData->vPos-((float)i*l_vDir));
	//					}
	//					m_vPriorPosition = e_pParticleData->vPos;
	//				}
	//				else
	//				{
	//					g_pPrt->GetEmiterManager()->GetWaitForEmitParticle()->push_back(this->m_sParticleName);
	//					g_pPrt->GetEmiterManager()->GetWaitForEmitParticlePos()->push_back(m_vRelatedPos+e_pParticleData->vPos);
	//				}
	//				if( m_bImmediateDied )
	//					e_pParticleData->fLifespan = 0;
	//			} 
	//		}
	//	}

	//	virtual NamedTypedObject*  Clone()
	//	{
	//		cPrtLifeActRebirth* l_p = new cPrtLifeActRebirth;
	//		l_p->SetCount(this->m_iCount);
	//		l_p->SetDeadlineTime(this->m_fDeadlineTime);
	//		l_p->SetParticleName(this->m_sParticleName);
	//		l_p->SetRelatedPos(m_vRelatedPos);
	//		l_p->SetName(GetName());
	//		l_p->SetImmediateDied(m_bImmediateDied);
	//		l_p->SetElpaseTimeToShot(m_fElpaseTimeToShot);
	//		return l_p; 
	//	};
	//	//wchar_t	m_sParticleName[MAX_PATH];
	//	////this one usually reference Life Time MinTime,because sometimes the min time is too small,so it won't happen
	//	//GET_SET_DEC(float,m_fDeadlineTime,GetDeadlineTime,SetDeadlineTime);
	//	//GET_SET_DEC(Vector3,m_vRelatedPos,GetRelatedPos,SetRelatedPos);
	//	//GET_SET_DEC(bool,m_bImmediateDied,GetImmediateDied,SetImmediateDied);
	//	//GET_SET_DEC(float,m_fDistanceTimeToShot,GetDistanceTimeToShot,SetDistanceTimeToShot);
	//	////to test dis time arrive or not
	//	//float	m_fElpaseTimeCompare; 
	//	//int m_iCount;
	//	//int		m_iCountCompaer;
	//	virtual char*	GetDataInfo()
	//	{
	//		char	l_temp2[MAX_PATH];
	//		WCHAR_TO_CHAR(m_sParticleName,l_temp2);
	//		sprintf_s(m_sTemp,MAX_PATH,"DeadLineTime:=%.2f\nCount:%d\nRelatedPos:x=%.2f,y=%.2f,z=%.2f\nParticleName:%s\nImmediateDied:%s\nElpaseTimeToShot:%.2f\0",
	//			m_fDeadlineTime,m_iCount,m_vRelatedPos.x,m_vRelatedPos.y,m_vRelatedPos.z,l_temp2,this->m_bImmediateDied?"true":"false",this->m_fElpaseTimeToShot);
	//		return m_sTemp;
	//	}
	//	//this one for output data value by "," to separate value
	//	virtual char*	GetOutputDataString()
	//	{
	//		char	l_temp2[MAX_PATH];
	//		WCHAR_TO_CHAR(m_sParticleName,l_temp2);
	//		sprintf_s(m_sTemp,MAX_PATH,"%.2f,%d,%.2f,%.2f,%.2f,%s,%d,%.2f\0",
	//			m_fDeadlineTime,m_iCount,m_vRelatedPos.x,m_vRelatedPos.y,m_vRelatedPos.z,l_temp2,this->m_bImmediateDied?1:0,this->m_fElpaseTimeToShot);
	//		return m_sTemp;
	//	}
	//	//input the output data string,and analyze it
	//	virtual bool	SetDataByDataString(char*e_pString)
	//	{
	//		char*Nexttoken = 0;
	//		char* l_pString = strtok(e_pString,",");
	//		this->m_fDeadlineTime = (float)atof(l_pString);
	//		l_pString = strtok(0,",");
	//		this->m_iCount = atoi(l_pString);
	//		l_pString = strtok(0,",");
	//		this->m_vRelatedPos.x = (float)atof(l_pString);
	//		l_pString = strtok(0,",");
	//		this->m_vRelatedPos.y = (float)atof(l_pString);
	//		l_pString = strtok(0,",");
	//		this->m_vRelatedPos.z = (float)atof(l_pString);
	//		CHAR_TO_WCHAR(strtok(0,","),m_sParticleName);
	//		assert(wcslen(m_sParticleName));
	//		l_pString = strtok(0,",");
	//		m_bImmediateDied = atoi(l_pString)?true:false;
	//		l_pString = strtok(0,",");
	//		m_fElpaseTimeToShot = (float)atof(l_pString);
	//		if( strtok(0,",") )
	//			return false;
	//		return true;
	//	}
	//};
//end namespace FATMING_CORE
}