#include "../stdafx.h"
#include "ParticleBase.h"
#include "ParticleEmitterBehavior.h"
namespace FATMING_CORE
{	
	TYPDE_DEFINE_MARCO(cParticleEmiterWithShowPosition);
	cParticleEmiterWithShowPosition::cParticleEmiterWithShowPosition(wchar_t*e_strName)
	{
		SetName(e_strName);
		m_vEmitDirection = m_vCurveRotationAngle = m_vEmitterPos = Vector3::Zero;
		m_pPrtEmitter = 0;
		m_EndTimeCounter.SetTargetTime(0.f);
		m_bImediatelyKilled = false;
		m_pCurveWithTime = 0;
		//m_fAngle = 0.f;
		m_ePathType = ePT_NO_PATH;
		m_eParticleRotateMode = ePRM_NONE;
	}

	cParticleEmiterWithShowPosition::cParticleEmiterWithShowPosition(cParticleEmiterWithShowPosition*e_pParticleEmiterWithShowPosition)
	:cFMTimeLineAnimationRule(e_pParticleEmiterWithShowPosition)
	{
		m_pPrtEmitter = 0;
		m_pCurveWithTime = 0;
		m_vEmitDirection = e_pParticleEmiterWithShowPosition->m_vEmitDirection;
		m_eParticleRotateMode = e_pParticleEmiterWithShowPosition->m_eParticleRotateMode;
		m_vCurveRotationAngle = e_pParticleEmiterWithShowPosition->m_vCurveRotationAngle;
		m_vEmitterPos = e_pParticleEmiterWithShowPosition->m_vEmitterPos;
		//m_fAngle = e_pParticleEmiterWithShowPosition->m_fAngle;
		m_pPrtEmitter = new cPrtEmitter(e_pParticleEmiterWithShowPosition->m_pPrtEmitter);
		m_EndTimeCounter = e_pParticleEmiterWithShowPosition->m_EndTimeCounter;
		m_bImediatelyKilled = e_pParticleEmiterWithShowPosition->m_bImediatelyKilled;
		if( e_pParticleEmiterWithShowPosition->m_pCurveWithTime )
		{
			m_pCurveWithTime = new cCurveWithTime(e_pParticleEmiterWithShowPosition->m_pCurveWithTime);
			//m_pCurveWithTime = new cCurveWithTime();
			//*m_pCurveWithTime = *e_pParticleEmiterWithShowPosition->m_pCurveWithTime;
		}
		m_ePathType = e_pParticleEmiterWithShowPosition->m_ePathType;
		SetName(e_pParticleEmiterWithShowPosition->GetName());
	}

	cParticleEmiterWithShowPosition::~cParticleEmiterWithShowPosition()
	{
		SAFE_DELETE(m_pPrtEmitter);
		SAFE_DELETE(m_pCurveWithTime);
	}

	//void	cParticleEmiterWithShowPosition::InternalStart(float e_fStartTime)
	//{
	//	m_EndTimeCounter.Start();
	//	if( m_pCurveWithTime )
	//	{
	//		m_pCurveWithTime->Start(e_fStartTime);
	//		//curve has been convert position by m_vEmitterPos so ignore m_vPos
	//		m_pPrtEmitter->Start(this->m_pCurveWithTime->GetCurrentPosition()+m_vPos,e_fStartTime,false);
	//	}
	//	else
	//	{
	//		m_pPrtEmitter->Start(m_vEmitterPos+m_vPos,e_fStartTime,false);
	//	}
	//}


	void	cParticleEmiterWithShowPosition::PathUpdate(float e_fElpaseTime)
	{
		if( ePT_NO_PATH == this->m_ePathType )
		{
			m_pPrtEmitter->SetPos(m_vEmitterPos+this->GetPos());
			m_pPrtEmitter->Update(e_fElpaseTime);
		}
		else
		if( ePT_FOLLOW_PATH == this->m_ePathType )
		{
			m_pCurveWithTime->Update(e_fElpaseTime);
			m_pPrtEmitter->SetPos(this->m_pCurveWithTime->GetCurrentPosition()+this->GetPos());
			m_pPrtEmitter->Update(e_fElpaseTime);
		}
		else
		if( ePT_ALL_PATH_POINT == this->m_ePathType )
		{
			if(m_pPrtEmitter->ShotUpdate(e_fElpaseTime))
			{
				int	l_iNum = (int)this->m_pCurveWithTime->GetOriginalPointList().size();
				//skip start position because it has shot.
				for( int i=1;i<l_iNum;++i )
				{
					m_pPrtEmitter->SetPos(m_pCurveWithTime->GetOriginalPointList()[i]+this->GetPos());
					m_pPrtEmitter->Shot(m_pPrtEmitter->m_iEmitParticleAmount,0.001f);
				}//set to statr position so if particle is time to shot it will shot at start position.
				m_pPrtEmitter->SetPos(m_pCurveWithTime->GetOriginalPointList()[0]+this->GetPos());
			}
			m_pPrtEmitter->ParticleUpdate(e_fElpaseTime);
		}
	}

	void	cParticleEmiterWithShowPosition::InternalUpdate(float e_fElpaseTime)
	{
	    if(this->m_fPastTime<this->m_fStartTime)
	        return;
		if( this->IsAnimationLoop() )
		{
			PathUpdate(e_fElpaseTime);
		}
		else
		{
			if( m_EndTimeCounter.bTragetTimrReached )
			{
				if( m_bImediatelyKilled )
				{
					m_pPrtEmitter->Stop();
					m_pPrtEmitter->m_bActived = false;
					this->SetAnimationDone(true);
				}
				else
				{//still update rest particles.
					m_pPrtEmitter->ParticleUpdate(e_fElpaseTime);
					if( m_pPrtEmitter->GetCurrentWorkingParticles() == 0 )
					{
						this->SetAnimationDone(true);
				    }
				}
			}
			else
			{
				m_EndTimeCounter.Update(e_fElpaseTime);
				if( !m_EndTimeCounter.bTragetTimrReached )
					PathUpdate(e_fElpaseTime);
			}
		}
	}

	void	cParticleEmiterWithShowPosition::RearrangeTime(float e_fNewTime)
	{
		float	l_fPercent = e_fNewTime/GetEndTime();
		this->m_fStartTime *= l_fPercent;
		this->m_EndTimeCounter.SetTargetTime(l_fPercent*m_EndTimeCounter.fTargetTime);
		if( m_ePathType != ePT_NO_PATH )
		{
			this->m_pCurveWithTime->RearrangeTime(l_fPercent*m_EndTimeCounter.fTargetTime);
		}
	}

	void	cParticleEmiterWithShowPosition::RearrangeTimeByPercent(float e_fPercenttage)
	{
		this->m_fStartTime *= e_fPercenttage;
		this->m_EndTimeCounter.SetTargetTime(e_fPercenttage*m_EndTimeCounter.fTargetTime);
		if( m_ePathType != ePT_NO_PATH )
		{
			this->m_pCurveWithTime->RearrangeTime(e_fPercenttage*m_EndTimeCounter.fTargetTime);
		}	
	}

	void	cParticleEmiterWithShowPosition::RenderByGlobalTime(float e_fTime)
	{
		UpdateByGlobalTime(e_fTime);
		this->Render();
	}

	void	cParticleEmiterWithShowPosition::UpdateByGlobalTime(float e_fGlobalTime)
	{
		this->Init();
		this->Update(e_fGlobalTime);
	}

	void	cParticleEmiterWithShowPosition::SetAnimationDone(bool e_bAnimationDone)
	{
		cFMTimeLineAnimationRule::SetAnimationDone(e_bAnimationDone);
		if(this->m_bImediatelyKilled)
		{
			if( m_pPrtEmitter )
				this->m_pPrtEmitter->Stop();
		}
	}

	void	cParticleEmiterWithShowPosition::SetCurveWithTime(cCurveWithTime* e_pCurveWithTime)
	{
		SAFE_DELETE(m_pCurveWithTime);
		if( e_pCurveWithTime )
			m_pCurveWithTime = new cCurveWithTime(e_pCurveWithTime);
	}

	float	cParticleEmiterWithShowPosition::GetEndTime()
	{
		return this->m_fStartTime+this->m_EndTimeCounter.fTargetTime;
		//if( l_fTime == 0.f )//
		//{
		//	if( this->m_pCurveWithTime )
		//		l_fTime = this->m_fStartTime+m_pCurveWithTime->GetLastTime();
		//}
		//return l_fTime;
	}

	void	cParticleEmiterWithShowPosition::SetAnimationLoop(bool e_bLoop)
	{
		cFMTimeLineAnimationRule::SetAnimationLoop(e_bLoop);
		if(m_pCurveWithTime)
			m_pCurveWithTime->SetCurveLoop(e_bLoop);
	}

	void	cParticleEmiterWithShowPosition::SetPathType(ePathType e_ePathType)
	{
		m_ePathType = e_ePathType;
		if( m_ePathType == ePT_NO_PATH )
		{
			SAFE_DELETE(m_pCurveWithTime);
		}
	}

	void	cParticleEmiterWithShowPosition::TransformEmitDirection(cMatrix44 e_mat)
	{
		this->m_pPrtEmitter->SetEmitDirection(e_mat.TransformCoordinate(this->m_pPrtEmitter->GetEmitDirection()));
			
	}

	void	cParticleEmiterWithShowPosition::SetEndTimeCounter(float e_fTime)
	{
		this->m_EndTimeCounter.SetTargetTime(e_fTime);
		if(this->m_pCurveWithTime)
		{
			m_pCurveWithTime->RearrangeTime(e_fTime);
		}
	}


	void	cParticleEmiterWithShowPosition::RefreshAngleAndPosData()
	{
		if(this->m_pCurveWithTime)
		{
			cMatrix44	l_mat = Quaternion::EulerRotationQuaternion(m_vCurveRotationAngle).ToMatrix();
			switch(this->m_eParticleRotateMode)
			{
				case ePRM_NONE:
					break;
				break;
				case ePRM_SELF:
					m_pCurveWithTime->RotateCurveWithCurveCenter(l_mat);
					break;
				case ePRM_WORLD:
					m_pCurveWithTime->TransformCurve(l_mat);
					break;
			}
			m_pCurveWithTime->AddOffsetToAllPoint(m_vEmitterPos+this->GetPos());
		}
		if( m_pPrtEmitter )
			this->m_pPrtEmitter->SetEmitDirection(this->m_vEmitDirection);
	}

	void	cParticleEmiterWithShowPosition::SetEmitterPos(Vector3 e_vPos)
	{
		m_vEmitterPos = e_vPos;
		RefreshAngleAndPosData();
	}

	void	cParticleEmiterWithShowPosition::SetCurveAngle(Vector3 e_vAngle)
	{
		m_vCurveRotationAngle = e_vAngle;
		RefreshAngleAndPosData();
	}

	void	cParticleEmiterWithShowPosition::InternalInit()
	{
		if(m_pPrtEmitter)
		{
		    if(this->m_bImediatelyKilled)
			    m_pPrtEmitter->Stop();
	    }
	}

	void	cParticleEmiterWithShowPosition::SetParticleRotateMode(eParticleRotateMode e_eParticleRotateMode)
	{
		m_eParticleRotateMode = e_eParticleRotateMode;
		RefreshAngleAndPosData();	
	}

	void	cParticleEmiterWithShowPosition::SetEmitDirection(Vector3 e_vDirection)
	{
		this->m_vEmitDirection = e_vDirection;
		this->m_pPrtEmitter->SetEmitDirection(this->m_vEmitDirection);
	}
//end namespace FATMING_CORE
}