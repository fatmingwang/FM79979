#pragma once

namespace FATMING_CORE
{
	class cPrtVelocityInitSetVelocity:public cParticleBase
	{
	  GETP_SET_DEC(Vector3,m_vVeolcity,GetVeolcity,SetVelocity);
	  GETP_SET_DEC(bool,m_bRandom,IsRandom,SetRandom);
	  //more cloase 0 more effective
	  GETP_SET_DEC(float,m_fRandomOffSet,GetRandomOffSet,SetRandomOffSet);
	public :
		DEFINE_TYPE_INFO()
		cPrtVelocityInitSetVelocity()
		{
			m_vVeolcity = Vector3(1.0f, 1.f, 0.f);
			m_bRandom = true;
			m_fRandomOffSet = 0.1f;
		}
		virtual ~cPrtVelocityInitSetVelocity(){}
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)override
		{
			if( m_bRandom )
			{
				Vector3	l_vD1(frand(-m_fRandomOffSet,m_fRandomOffSet),frand(-m_fRandomOffSet,m_fRandomOffSet),frand(-m_fRandomOffSet,m_fRandomOffSet));
				Vector3	l_vD2(m_vVeolcity.Normalize());
				Vector3	l_vVeolcity = l_vD1 + l_vD2;


				float	l_fLength =  m_vVeolcity.Length();
				l_vVeolcity = l_vVeolcity*l_fLength;
				//float	l_fOffsetX = UT::frand(0,l_fLength)*(l_fLength/m_fRandomOffSet)*((rand()%500)>250?1.f:-1.f);
				//float	l_fOffsetY = UT::frand(0,l_fLength)*(l_fLength/m_fRandomOffSet)*((rand()%500)>250?1.f:-1.f);
				//float	l_fOffsetZ = UT::frand(0,l_fLength)*(l_fLength/m_fRandomOffSet)*((rand()%500)>250?1.f:-1.f);
				//Vector3	l_vVeolcity = m_vVeolcity;
				//l_vVeolcity.x *= frand(0,l_fLength);
				//l_vVeolcity.y *= frand(0,l_fLength);
				//l_vVeolcity.z *= frand(0,l_fLength);
				//l_vVeolcity.x += l_fOffsetX;
				//l_vVeolcity.y += l_fOffsetY;
				//l_vVeolcity.z += l_fOffsetZ;

				e_pParticleData->vVelocity = l_vVeolcity;
				e_pParticleData->vOrigonalVelocity = l_vVeolcity;
			}
			else
			{
				e_pParticleData->vVelocity = m_vVeolcity;	
				e_pParticleData->vOrigonalVelocity = m_vVeolcity;
			}
		}
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*  Clone()override;

		virtual char*	GetOutputDataString()override;

		virtual char*	GetDataInfo()override;

		//input the output data string,and analyze it
		virtual bool	SetDataByDataString(char*e_pString)override;
	}; 
	//===================
	//we should only have one initial velocity policy
	//so this one is ignore,but  it might useful in the furture
	//random fly to a Direction
	//===================

	//class cPrtVelocityInitRandomFly:public cParticleBase
	//{
	//	GETP_SET_DEC(float,m_fDistance,GetDistance,SetDistance);	
	//	GETP_SET_DEC(bool,m_bRandom,IsRandom,SetRandom);
	//	GETP_SET_DEC(Vector3,m_vDir,GetDir,SetDir);
	//public :
	//	DEFINE_TYPE_INFO()
	//	explicit cPrtVelocityInitRandomFly()
	//	{
	//		m_fDistance = 1.f;
	//		m_bRandom = false;
	//		m_vDir = Vector3(1.f,0.f,0.f);
	//	}
	//	virtual ~cPrtVelocityInitRandomFly(){}
	//	virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)
	//	{
	//		if( m_bRandom )
	//		{
	//			float	l_fX = frand(0,m_fDistance);
	//			float	l_fY = frand(0,m_fDistance);
	//			float	l_fZ = frand(0,m_fDistance);
	//			l_fX*=(rand()%500>250?1.f:-1.f);
	//			l_fY*=(rand()%500>250?1.f:-1.f);
	//			l_fZ*=(rand()%500>250?1.f:-1.f);
	//			e_pParticleData->vVelocity.x*=l_fX;
	//			e_pParticleData->vVelocity.y*=l_fY;
	//			e_pParticleData->vVelocity.z*=l_fZ;
	//			e_pParticleData->vOrigonalVelocity = e_pParticleData->vVelocity;
	//		 }
	//		 else
	//		 {
	//			 Vector3	l_vDir = m_vDir.Normalize();
	//			  l_vDir.x = ( l_vDir.x + rand()%500>250?0.01f:-0.01f )*frand(0,m_fDistance);
	//			  l_vDir.y = ( l_vDir.y + rand()%500>250?0.01f:-0.01f )*frand(0,m_fDistance);
	//			  l_vDir.z = ( l_vDir.z + rand()%500>250?0.01f:-0.01f )*frand(0,m_fDistance);
	//			  l_vDir = l_vDir.Normalize()*frand(0,m_fDistance);
	//			  e_pParticleData->vVelocity = l_vDir;
	//			  e_pParticleData->vOrigonalVelocity = l_vDir;
	//		 }	
	//	}
	//	//usage explanation
	//	static char*	UsageExplanation;
	//	//
	//	virtual cParticleBase*  Clone();
	//	virtual char*	GetDataInfo();
	//	virtual char*	GetOutputDataString();
	//	//input the output data string,and analyze it
	//	virtual bool	SetDataByDataString(char*e_pString);
	//};
	//cPrtAcceleration
	//it could be grative too,if acceleration is negative
	//yout have make sure update piority,or it could be wrong
	//because here we will minus the distance particle has been moved
	class cPrtVelocityActAcceleration:public cParticleBase
	{
		GETP_SET_DEC(float,m_fAccelerationSpeed,GetAccelerationSpeed,SetAccelerationSpeed);
	public:
		DEFINE_TYPE_INFO()
		cPrtVelocityActAcceleration(){ m_fAccelerationSpeed = 0.f; }
		virtual ~cPrtVelocityActAcceleration(){}
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)override
		{
			e_pParticleData->vVelocity += m_fAccelerationSpeed*e_fElpaseTime*e_pParticleData->vOrigonalVelocity.Normalize();
		}
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*  Clone()override;
		//
		virtual char*	GetOutputDataString()override;
		//
		virtual char*	GetDataInfo()override;
		//input the output data string,and analyze it
		virtual bool	SetDataByDataString(char*e_pString)override;
	};
	//==================================
	//
	//==================================
	class cPrtVelocityActDircctionChange:public cParticleBase
	{
		//how long particle original velocity will be zreo for xyz.
		GETP_SET_DEC(Vector3,m_vStopTime,GetStopTime,SetStopTime);
		//after direction speed time is up,new velocity .
		GETP_SET_DEC(Vector3,m_vNewSpeed,GetNewSpeed,SetNewSpeed);
		//after direction speed time is up,new move velocity is oging for accesleration or normal speed
		GETP_SET_DEC(bool,m_bAccesleration,IsAccesleration,SetAccesleration)
	public:
		DEFINE_TYPE_INFO()
		cPrtVelocityActDircctionChange(){ m_vNewSpeed = m_vStopTime = Vector3::One; m_bAccesleration = true; }
		virtual ~cPrtVelocityActDircctionChange(){}
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)override
		{
			float	l_fProgress = 0.f;
			float	l_fPastTime = e_pParticleData->fOriginaLifeSpan - e_pParticleData->fLifespan;
			//still not reach stop time
			if( m_vStopTime.x != 0.f )
			{
				if( l_fPastTime < m_vStopTime.x )
				{
					l_fProgress = 1.f-(e_pParticleData->fPastTime/m_vStopTime.x);
					e_pParticleData->vVelocity.x = e_pParticleData->vOrigonalVelocity.x*l_fProgress;
				}
				else
				{
					if( m_bAccesleration )
						e_pParticleData->vVelocity.x += (m_vNewSpeed.x*2*e_fElpaseTime);
					else
						e_pParticleData->vVelocity.x = m_vNewSpeed.x*e_fElpaseTime;
				}
			}


			if( m_vStopTime.y != 0.f )
			{
				if( l_fPastTime < m_vStopTime.y )
				{
					l_fProgress = 1.f-(e_pParticleData->fPastTime/m_vStopTime.y);
					e_pParticleData->vVelocity.y = e_pParticleData->vOrigonalVelocity.y*l_fProgress;
				}
				else
				{
					if( m_bAccesleration )
						e_pParticleData->vVelocity.y += (m_vNewSpeed.y*2*e_fElpaseTime);
					else
						e_pParticleData->vVelocity.y = m_vNewSpeed.y*e_fElpaseTime;
				}
			}
			if( m_vStopTime.z != 0.f )
			{
				if( l_fPastTime < m_vStopTime.z )
				{
					l_fProgress = 1.f-(e_pParticleData->fPastTime/m_vStopTime.z);
					e_pParticleData->vVelocity.z = e_pParticleData->vOrigonalVelocity.z*l_fProgress;
				}
				else
				{
					if( m_bAccesleration )
						e_pParticleData->vVelocity.z += (m_vNewSpeed.z*2*e_fElpaseTime);
					else
						e_pParticleData->vVelocity.z = m_vNewSpeed.z*e_fElpaseTime;
				}
			}
		}
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*  Clone()override;
		//
		virtual char*	GetOutputDataString()override;
		//
		virtual char*	GetDataInfo()override;
		//input the output data string,and analyze it
		virtual bool	SetDataByDataString(char*e_pString)override;
	};

	class cPrtVelocityActBySatelliteAction:public cParticleBase
	{
		//radius for current radius can be change make initial start different
		GET_SET_DEC(float,m_fRadius,GetRadius,SetRadius);
		//rotate speed,each time radius will add speed
		GET_SET_DEC(float,m_fSpeed,GetSpeed,SetSpeed);
		//the track length
		GET_SET_DEC(float,m_fTrackLength,GetTrackLength,SetTrackLength);
		//rotate by specific pivot
		GET_SET_DEC(Vector3,m_vPivot,GetPivot,SetPivot);
	public :
		DEFINE_TYPE_INFO()
		cPrtVelocityActBySatelliteAction(){  m_vPivot = Vector3(0.f,1.f,0.f); m_fRadius = 0.f;  }
		virtual ~cPrtVelocityActBySatelliteAction(){}
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)override
		{
		  //set pivot
		  cMatrix44	l_mat = cMatrix44::Identity;
		  l_mat.r[0].x = m_vPivot.x;
		  l_mat.r[1].y = m_vPivot.y;
		  l_mat.r[2].z = m_vPivot.z;
		  //set sphere position
		  Vector3	l_vPos(e_pParticleData->vPos.x+sin(m_fRadius)*m_fTrackLength,e_pParticleData->vPos.y,e_pParticleData->vPos.z+cos(m_fRadius)*m_fTrackLength);
		  //position transform for pivot
		  l_vPos = l_mat.TransformVector(l_vPos);
		  //apply position to particle position
		  e_pParticleData->vPos+=l_vPos;
		}
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*  Clone()override;
		virtual char*	GetDataInfo()override;
		virtual char*	GetOutputDataString()override;
		//input the output data string,and analyze it
		virtual bool	SetDataByDataString(char*e_pString)override;
	};  //  end of class StartPositionByVelocityAction

//end namespace FATMING_CORE
}