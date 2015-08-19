#ifndef PRT_ROTATE_H
#define PRT_ROTATE_H

#include "ParticleType.h"
namespace FATMING_CORE
{
	class cPrtRotateInitRotate:public cParticleBase
	{	
		GETP_SET_DEC(Vector3,m_vAngle,GetAngle,SetAngle);
	public :
		DEFINE_TYPE_INFO();
		cPrtRotateInitRotate(){ memset(m_vAngle,0,sizeof(Vector3)); }
		virtual ~cPrtRotateInitRotate(){}
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)
		{
			e_pParticleData->vAngle += m_vAngle;
		    e_pParticleData->vOriginaAngle = e_pParticleData->vAngle;
			e_pParticleData->ePRM = ePRM_SELF;
		}
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*  Clone();
		virtual char*	GetDataInfo();
		virtual char*	GetOutputDataString();
		//input the output data string,and analyze it
		virtual bool	SetDataByDataString(char*e_pString);
	};

	class cPrtRotateActRotate:public cParticleBase
	{	
		GETP_SET_DEC(eParticleRotateMode,m_ePRM,GetPRM,SetPRM);
		GETP_SET_DEC(Vector3,m_vAngle,GetAngle,SetAngle);
		//if random on,we will reference random offset value to gain the angle
		GETP_SET_DEC(bool,m_bRandom,IsRandom,SetRandom);
		GETP_SET_DEC(float,m_fRandomOffset,GetRandomOffset,SetRandomOffset);
		//if true random value could be negatic
		GETP_SET_DEC(bool,m_bNegative,IsNegative,SetNegative);
	public :
		DEFINE_TYPE_INFO();
		cPrtRotateActRotate(){ memset(m_vAngle,0,sizeof(Vector3)); m_bRandom = false; m_fRandomOffset = 0.f; m_ePRM = ePRM_SELF; m_bNegative = false; }
		virtual ~cPrtRotateActRotate(){}
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)
		{
			e_pParticleData->ePRM = m_ePRM;

			if( this->m_bRandom )
			{
				Vector3	l_v = m_vAngle*frand(m_bNegative?-m_fRandomOffset:0,m_fRandomOffset);
				l_v*=e_fElpaseTime;
				e_pParticleData->vAngle+=l_v;
			}
			else
			{
				e_pParticleData->vAngle+=m_vAngle*e_fElpaseTime;
			}
		}
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*  Clone();
		virtual char*	GetDataInfo();
		virtual char*	GetOutputDataString();
		//input the output data string,and analyze it
		virtual bool	SetDataByDataString(char*e_pString);
	};
}

#endif