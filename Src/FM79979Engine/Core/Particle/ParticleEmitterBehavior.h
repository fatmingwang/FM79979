#ifndef PARCILE_EMITTER_BEHAVIOR
#define PARCILE_EMITTER_BEHAVIOR

#include "ParticleEmitter.h"

namespace FATMING_CORE
{
	//only actived once,while time is reached it will call
	//cParticleBase::DataChenged(void*e_pData,eDataChangeType e_eDataChangeType){return false;}
	struct sPolicyDataChanged
	{
		UT::sTimeCounter	ActiveTimeCounter;
		cParticleBase*pPolicy;//get from out side
		char*pData;
		cParticleBase::eDataChangeType DataChangeType;
		sPolicyDataChanged()
		{
			pPolicy = 0;
			pData = 0;
		}
		~sPolicyDataChanged()
		{
			SAFE_DELETE(pData);
		}
		void	SetData(cParticleBase*e_pPolicy,void*e_pData,cParticleBase::eDataChangeType e_DataChangeType,float e_fStartTime)
		{
			pPolicy = e_pPolicy;
			ActiveTimeCounter.SetTargetTime(e_fStartTime);
			SAFE_DELETE(pData);
			pData = (char*)e_pData;
		}
		void	Update(float e_fElpaseTime)
		{
			if(!ActiveTimeCounter.bTragetTimrReached)
			{
				ActiveTimeCounter.Update(e_fElpaseTime);
				if(ActiveTimeCounter.bTragetTimrReached)
				{
#ifdef DEBUG
					assert(pPolicy->DataChenged(pData,DataChangeType));
#else
					pPolicy->DataChenged(pData,DataChangeType);
#endif
				}
			}
		}
		void	Start(float e_fElpaseTime){ ActiveTimeCounter.Start(); }
	};

	class	cParticleEmitterGroup;
	//===============
	//a advance particle emitter,it will copy data from cPrtEmitter(not resource from soruce,it's a clone)
	//and it will shot at the time we arranged.
	//===============
	//fuck for editor reason so I do not hierachiert from cPrtEmitter,because cPrtEmitter will change a lot time.
	class	cParticleEmiterWithShowPosition:public cFMTimeLineAnimationRule//PRT::cPrtEmitter
	{
		friend class cParticleEmitterGroup;
		//add dynamic particle data change let particle more cool,but I am lazy to do this now.
		//std::vector<sPolicyDataChanged*>m_pPolicyDataChangedVector;
		//void	DoPolicyDataChangedUpdate(float e_fElpaseTime);
		//void	AddPolicyDataChanged(sPolicyDataChanged*e_pData){m_pPolicyDataChangedVector.push_back(e_pData);}
		//
		inline	void	InternalInit();
		//virtual	void	InternalStart(float e_fStartTime);
		virtual	void	InternalUpdate(float e_fElpaseTime);
		inline	void	InternalRender(){if(this->m_fPastTime<this->m_fStartTime)return;m_pPrtEmitter->Render();}
		void	PathUpdate(float e_fElpaseTime);
	public:
		//how particled will emit at the specific position
		enum	ePathType
		{
			ePT_NO_PATH = 0,//just at the position as we expected
			ePT_FOLLOW_PATH ,//shot position by time
			ePT_ALL_PATH_POINT,//while time is reached shot position will at the path point
		};
	protected:
		//make sure particle emitter has a instance not just a reference pointer!because clone object would need to emit it again
		GET_SET_DEC(cPrtEmitter*,m_pPrtEmitter,GetPrtEmitter,SetPrtEmitter);
		//
		ePathType	m_ePathType;
		//the path we will going,it effect by endtim counter,the last time should be same as end time counter's target time
		cCurveWithTime*m_pCurveWithTime;
		//what tim is it start
		//GETP_SET_DEC(UT::sTimeCounter,m_StartTimeCounter,GetStartTimeCounter,SetStartTimeCounter);
		//what time is it died
		UT::sTimeCounter	m_EndTimeCounter;
		//if true the current time over then
		GETP_SET_DEC(bool,m_bImediatelyKilled,IsImediatelyKilled,SetImediatelyKilled);
		//if particle is loop and m_bImediatelyKilled is true
		//we do not expect particle keep emit,so we have to set false,
		//but we want to set it to its original loop state
		//!!but for now we always set particle for loop now!!
		//bool	m_bParticleLoop;
		//each cParticleEmiterWithShowPosition might has different start position or angle if curve is exists
		//keep data for clone.
		Vector3	m_vEmitterPos;
		//for emitter's emit direction,it's work for editor save the angle we have changed the emitter's direction
		//each emitter we hope it might has different emit direction.
		Vector3	m_vEmitDirection;
		//for curve if curve is exist
		Vector3	m_vCurveRotationAngle;
		eParticleRotateMode	m_eParticleRotateMode;
	public:
		DEFINE_TYPE_INFO();
		cParticleEmiterWithShowPosition(wchar_t*e_strName);
		cParticleEmiterWithShowPosition(cParticleEmiterWithShowPosition*e_pParticleEmiterWithShowPosition);
		CLONE_MYSELF(cParticleEmiterWithShowPosition)
		virtual ~cParticleEmiterWithShowPosition();
		virtual	void		InvertOrder(){}
		virtual	void		RearrangeTime(float e_fNewTime);
		virtual	void		RearrangeTimeByPercent(float e_fPercenttage);
		virtual	void		UpdateByGlobalTime(float e_fGlobalTime);
		virtual	void		RenderByGlobalTime(float e_fTime);
		//start with endtime counter time
		virtual	float		GetEndTime();
		virtual	void		SetAnimationLoop(bool e_bLoop);
		virtual	void		SetAnimationDone(bool e_bAnimationDone);
		ePathType			GetPathType(){ return m_ePathType; }
		void				SetPathType(ePathType e_ePathType);
		cCurveWithTime*		GetCurveWithTime(){ return m_pCurveWithTime; }
		void				SetCurveWithTime(cCurveWithTime* e_pCurveWithTime);
		Vector3*			GetPosPointer(){ return this->GetLocalPositionPointer(); }
		float*				GetStartTimePointer(){ return &this->m_fStartTime; }
		void				TransformEmitDirection(cMatrix44 e_mat);
		//also setup curve time!
		void				SetEndTimeCounter(float e_fTime);
		UT::sTimeCounter*	GetEndTimeCounter(){ return &m_EndTimeCounter; }

		void				RefreshAngleAndPosData();
		//only for itself the m_vPos is only for parent(cParticleEmitterGroup) call!
		void				SetEmitterPos(Vector3 e_vPos);
		void				SetCurveAngle(Vector3 e_vAngle);
		void				SetParticleRotateMode(eParticleRotateMode e_eParticleRotateMode);
		void				SetEmitDirection(Vector3 e_vDirection);
		Vector3*			GetEmitterPos(){return &m_vEmitterPos;}
		Vector3*			GetCurveRotationAngle(){return &m_vCurveRotationAngle;}
		Vector3*			GetEmitDirection(){return &m_vEmitDirection;}

		eParticleRotateMode	GetParticleRotateMode(){return m_eParticleRotateMode;}
	};

//end namespace PRT
}

#endif