#include "../stdafx.h"
#include "ParticleBase.h"
#include "ParticleEmitterGroup.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cParticleEmitterGroup);
	cParticleEmitterGroup::cParticleEmitterGroup()
	{	
		m_matParticleDirectionTransform = m_matOrientationAndPositionTransform = cMatrix44::Identity;
		m_iCurrentWorkingParticles = 0;
		m_bStart = false;
	}

	cParticleEmitterGroup::cParticleEmitterGroup(cParticleEmitterGroup*e_pParticleEmitterGroup):cFatmingGroupBehaviorList<cParticleEmiterWithShowPosition>(e_pParticleEmitterGroup)
	{
	    m_bStart = false;
		m_iCurrentWorkingParticles = e_pParticleEmitterGroup->m_iCurrentWorkingParticles;
		m_matOrientationAndPositionTransform = e_pParticleEmitterGroup->m_matOrientationAndPositionTransform;
		m_matParticleDirectionTransform = e_pParticleEmitterGroup->m_matParticleDirectionTransform;
		for( int i=0;i<e_pParticleEmitterGroup->Count();++i )
		{
			cParticleEmiterWithShowPosition*l_pParticleEmiterWithShowPosition  = new cParticleEmiterWithShowPosition((*e_pParticleEmitterGroup)[i]);
			this->AddObject(l_pParticleEmiterWithShowPosition);
		}
	}

	cParticleEmitterGroup::~cParticleEmitterGroup()
	{
	
	}

	void	cParticleEmitterGroup::Init()
	{
	    //below has been call at cParticleEmiterWithShowPosition::Start
		//int	l_iCount = Count();
		//for( int i=0;i<l_iCount;++i )
		//{
		//	cParticleEmiterWithShowPosition*l_pParticleEmiterWithShowPosition  = this->m_ObjectList[i];
		//	if( l_pParticleEmiterWithShowPosition->IsImediatelyKilled() )
		//	    l_pParticleEmiterWithShowPosition->GetPrtEmitter()->Stop();
		//}
		m_bStart = true;
		cFatmingGroupBehaviorList<cParticleEmiterWithShowPosition>::Init();
	}

    void	cParticleEmitterGroup::Update(float e_fElpaseTime)
    {
        if( m_bStart )
        {
            cFatmingGroupBehaviorList<cParticleEmiterWithShowPosition>::Update(e_fElpaseTime);
            CalculateWorkingParticles();
        }
    }
    
    void	cParticleEmitterGroup::Stop()
    {
        m_bStart = false;
        cFatmingGroupBehaviorList<cParticleEmiterWithShowPosition>::Stop();
        int l_iCount = this->Count();
        for( int i=0;i<l_iCount;++i )
        {
            if((*this)[i]->GetPrtEmitter())
            {
                (*this)[i]->GetPrtEmitter()->Stop();
            }
        }
    }
    
	int		cParticleEmitterGroup::CalculateWorkingParticles()
	{
		m_iCurrentWorkingParticles = 0;
		int	l_iCount = Count();
		for( int i=0;i<l_iCount;++i )
		{
			cParticleEmiterWithShowPosition*l_pParticleEmiterWithShowPosition  = this->m_ObjectList[i];
			m_iCurrentWorkingParticles += l_pParticleEmiterWithShowPosition->GetPrtEmitter()->GetCurrentWorkingParticles();
		}
		if( m_iCurrentWorkingParticles == 0 &&this->m_iPlayCount != -1 )
		    m_bStart = false;
		return m_iCurrentWorkingParticles;
	}

	bool	cParticleEmitterGroup::WaitUntilAllAnimationIsDone()
	{
		return m_iCurrentWorkingParticles==0?false:true;
	}

	void		cParticleEmitterGroup::SetOrientationAndPositionTransform(cMatrix44	e_mat)
	{
		m_matOrientationAndPositionTransform = e_mat;
		int	l_iNum = this->Count();
		for( int i=0;i<l_iNum;++i )
		{
			cParticleEmiterWithShowPosition*l_pParticleEmiterWithShowPosition = (*this)[i];
			l_pParticleEmiterWithShowPosition->TransformEmitDirection(m_matOrientationAndPositionTransform);
			l_pParticleEmiterWithShowPosition->SetPos(Vector3(m_matOrientationAndPositionTransform.m[3]));
		}
	}

	void		cParticleEmitterGroup::SetParticleDirectionTransform(cMatrix44	e_mat)
	{
		m_matParticleDirectionTransform = e_mat;
		int	l_iNum = this->Count();
		for( int i=0;i<l_iNum;++i )
		{
			cParticleEmiterWithShowPosition*l_pParticleEmiterWithShowPosition = (*this)[i];
			l_pParticleEmiterWithShowPosition->TransformEmitDirection(m_matParticleDirectionTransform);
		}	
	}

	void        cParticleEmitterGroup::SetPos(Vector3 e_vPos)
	{
		int	l_iNum = this->Count();
		for( int i=0;i<l_iNum;++i )
		{
			cParticleEmiterWithShowPosition*l_pParticleEmiterWithShowPosition = (*this)[i];
			l_pParticleEmiterWithShowPosition->SetPos(e_vPos);
		}	
	}
	
	Vector3     cParticleEmitterGroup::GetPos()
	{
        return Vector3(m_matOrientationAndPositionTransform.m[3]);	
	}
//end namespace namespace FATMING_CORE
}