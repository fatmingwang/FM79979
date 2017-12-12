#ifndef _PARTICLE_EMITTER_GROUP_H_
#define _PARTICLE_EMITTER_GROUP_H_

#include "ParticleEmitterBehavior.h"
namespace FATMING_CORE
{
	class	cParticleEmitterGroup:public cFatmingGroupBehaviorList<cParticleEmiterWithShowPosition>
	{
	    GET_SET_DEC(bool,m_bStart,IsStart,SetStart);
	    //how many working particles is alive?
		int	CalculateWorkingParticles();
		int	m_iCurrentWorkingParticles;
		virtual bool	WaitUntilAllAnimationIsDone();
		//the matrix to effect child emit direction
		cMatrix44	m_matOrientationAndPositionTransform;
		cMatrix44	m_matParticleDirectionTransform;
	public:
		DEFINE_TYPE_INFO()
		cParticleEmitterGroup();
		cParticleEmitterGroup(cParticleEmitterGroup*e_pParticleEmitterGroup);
		virtual~cParticleEmitterGroup();
		int	GetCurrentWorkingParticles(){ return m_iCurrentWorkingParticles; }
		//set transform will effect children's emit direcion
		void		SetParticleDirectionTransform(cMatrix44	e_mat);
		cMatrix44	GetParticleDirectionTransform(){ return m_matParticleDirectionTransform; }
		void		SetOrientationAndPositionTransform(cMatrix44	e_mat);
		cMatrix44	GetOrientationAndPositionTransform(){ return m_matOrientationAndPositionTransform; }
		void        SetPos(Vector3 e_vPos);
		Vector3     GetPos();
		//
		virtual	void	Init();
        virtual	void	Update(float e_fElpaseTime);
        virtual	void	Stop();
	};
	typedef cParticleEmitterGroup cPRTG;
//end name space
}
#endif