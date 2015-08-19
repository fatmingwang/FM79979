#ifndef PARTICLE_BASE_H
#define PARTICLE_BASE_H
#include "ParticleType.h"
namespace FATMING_CORE
{
	class	cPrtEmitter;
	class cParticleBase :virtual    public NamedTypedObject
	{
		friend class cPrtEmitter;
	protected:
		//it will add while emitter is calling AddInitPolicy or AddActPolicy
		//most time we do not emitter owner,but for some specil policy,
		//we have to access vertices pointer or texture coordinate pointer
		//so we have to reset the correct one!
		GET_SET_DEC(cPrtEmitter*,m_pEmitterOwner,GetEmitterOwner,SetEmitterOwner);
		//for GetDataInfo,just a temp
		//static char	m_sTemp[MAX_PATH];
		char	m_sTemp[MAX_PATH];
	public:
		DEFINE_TYPE_INFO();
		inline	cParticleBase(){ m_pEmitterOwner = 0; }
		inline	virtual ~cParticleBase(){}
		//index for the working particle index
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData){ assert(0); };
		//UsageExplanation
		static char*	UsageExplanation;
		//for data info
		virtual char*	GetDataInfo(){ return 0; }
		//this one for output data value by "," to separate value
		//exprte to xml
		virtual char*	GetOutputDataString(){ return 0; }
		//input the output data string,and analyze it
		//from xml
		virtual bool	SetDataByDataString(char*e_pString){ return false; }
		//fuck I am lazy to do this now...because it's quite hard to decied to use the features like this or insert a time span to determinate the policy running.
		//too hard to edit for now
		enum eDataChangeType
		{
			eDCT_SPEED = 0,
			eDCT_COLOR,
			eDCT_SIZE,
			eDCT_SIZE_BIGGER_SMALLER,
		};
		//dynamicly to change the policy data,so it will more cool.
		virtual	bool	DataChenged(void*e_pData,eDataChangeType e_eDataChangeType){return false;}
		//see m_pEmitterOwner's comment,if it's true the policy can't be cloned it must be new a instance for owner!.
		//above condition was consided while we are cloning a prt emitter.
		//cPrtTextureActDynamicTexture
		virtual	bool	IsOwnerIsImportant(){ return false; }
	};
//end PRT namespace
};
#endif