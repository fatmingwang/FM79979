#pragma once

#if defined(IOS) || defined(ANDROID) || defined(WASM) || defined(LINUX) || defined(UWP)
#define GL_QUADS 0x0007
#endif
#include "ParticleBase.h"
namespace FATMING_CORE
{
	class cPrtEmitter;
	#define	MAX_PARTICLE_BASE_COUNT	30
	//a interface to update policy data
	class cQuickUpdateParticleObjectListByName:public cNamedTypedObjectVector<cParticleBase>
	{
		//for more effectively to update data without pointer deliver
		cParticleBase*m_ppParticleBase[MAX_PARTICLE_BASE_COUNT];
		int	m_iSize;
	public:
		cQuickUpdateParticleObjectListByName(cQuickUpdateParticleObjectListByName*e_pQuickUpdateParticleObjectListByName,cPrtEmitter*e_pCloneTargetPrtEmitter);
		cQuickUpdateParticleObjectListByName() { m_iSize = 0; }
		virtual ~cQuickUpdateParticleObjectListByName(){}
		//en sure u have call this after remover object or add object
		void	GenerateListBuffer()
		{
			m_iSize = Count();
			assert(m_iSize<=MAX_PARTICLE_BASE_COUNT);
			for(int j=0;j<m_iSize;++j)
			{
				m_ppParticleBase[j] = this->m_ObjectList[j];
			}
		}

		inline	void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)
		{
			for( int i=0;i<m_iSize;++i )
			//for( UINT i=0;i<this->m_ObjectList.size();++i )
			{
				this->m_ppParticleBase[i]->Update(e_fElpaseTime,e_iIndex,e_pParticleData);
				//(*this)[i]->Update(e_fElpaseTime,e_pParticleData);
			}
		}
		//check if the policy is not allow to be cloned!cPrtTextureActDynamicTexture
		bool	IsOwenrOfPolicyIsImportant();
	};
	//initial velocity and current position are setup up by inner Shot and Update function.
	//simple init and act velocity are not a particlebase inhiret class.
	//single one emiter.
	// 0 ___ 1
	//  |   |
	//  |   |
	// 2 --- 3
	//012,first triangle
	//213,second triangle
	//here using CW,it's because the glEnable2D I have use as left hand coordinate
	//so assign texture coordinate as above mention
	class	cPrtTextureActDynamicTexture;
	class	cParticleEmiterWithShowPosition;
	//#include "../AllCoreInclude.h"
	class cPrtEmitter:public cFMTimeLineAnimationRule
	{
		class cBatchRender* m_pBatchRender;
		friend class	cPrtTextureActDynamicTexture;
		friend class	cParticleEmiterWithShowPosition;
		//while primitive is points ActSizeBlending and initSize just fetch x data
		//current only support quads and point,but quad is made by 2 triangle
		GET_SET_DEC(GLenum,m_iPrimitiveType,GetPrimitiveType,SetPrimitiveType);
		//
		GET_SET_DEC(GLenum,m_SrcBlendingMode,GetSrcBlendingMode,SetSrcBlendingMode);
		GET_SET_DEC(GLenum,m_DestBlendingMode,GetDestBlendingMode,SetDestBlendingMode);
		//texture,it always come from resource,or u have to delete it by urself
		class cBaseImage*	m_pBaseImage;
		//Actived,Start
		bool	m_bActived;
		//comr from particle manager
		bool	m_bPolicyFromClone;
		GET_SET_DEC(cQuickUpdateParticleObjectListByName*,m_pInitPolicyParticleList,GetInitPolicyParticleList,SetInitPolicyParticleList);
		GET_SET_DEC(cQuickUpdateParticleObjectListByName*,m_pActPolicyParticleList,GetActPolicyParticleList,SetActPolicyParticleList);
		//velocity,cPrtVelocityInitrSetVelocity,so we could setup velocity,each emiter need different direction so we clone it as need
		GET_SET_DEC(cParticleBase*,m_pVelocityInit,GetVelocity,SetVelocity);
		GETP_SET_DEC(Vector3,m_vPos,GetPos,SetPos);
		//compare to m_fGapTimeToShot if m_fGapTimeToShot is smaller than current,it's mean time to emit
		float	m_fCurrentTime;
		//how long should we emit,if it's 0 it's inlegal,so but we just make it fire and skip to next frame
		GET_SET_DEC(float,m_fGapTimeToShot,GetGapTimeToShot,SetGapTimeToShot);
		int	m_iCurrentEmitCount;
		//how many times we should to emit,0 for infinite
		GET_SET_DEC(int,m_iParticleEmitCount,GetParticleEmitCount,SetParticleEmitCount);
		//how many particle is working
		int	m_iCurrentWorkingParticles;
		//how many particles will be emit at once
		GET_SET_DEC(int,m_iEmitParticleAmount,GetEmitParticleAmount,SetEmitParticleAmount);
		//the max count particles could be alive,exceed will be ignore
		int	m_iMaxParticleCount;
		//whole particle data
		sParticleData*	m_pParticleData;
		//here we alloceate size equal m_iMaxParticleCount multiply 6.
		//all paticle do not use teiangle strip,instead triangle,because strip will conect together.
		Vector4*		m_pvAllColorPointer;
		Vector3*		m_pvAllPosPointer;
		Vector2*		m_pvAllTexCoordinatePointer;
		cMatrix44		GetParticleDataMatrix(sParticleData*e_pParticleData);
		void			Shot(float e_fElpaseTime = 0.016f);
		//time eto shot a new one?if true shot or still wait for shot
		bool			ShotUpdate(float e_fElpaseTime);
		//particle behavior update
		void			ParticleUpdate(float e_fElpaseTime);
		virtual	void	InternalInit()override;
		virtual	void	InternalUpdate(float e_fElpaseTime)override;
		virtual	void	InternalRender()override;
		bool			BatchRender();
		//shot particles
		void	Shot(int e_iNumParticle,float e_fElpaseTime = 0.016f);
	public:
		DEFINE_TYPE_INFO()
		cPrtEmitter(const wchar_t*e_pName);
		//basicly u just input cPrtEmitter,
		//but if u wanna clone policy list u have set e_bPolicyFromClone as true
		//ensure u have control memory problem
		cPrtEmitter(cPrtEmitter*e_pPrtEmitter,bool e_bPolicyFromClone = true);
		CLONE_MYSELF(cPrtEmitter);
		virtual ~cPrtEmitter();
		cBaseImage*			GetBaseImage(){return m_pBaseImage;}
		void				SetBaseImage(cBaseImage*e_pBaseImage);

		//add into policy list,after add into list u should call SetupPolicy
		void				AddInitPolicy(cParticleBase*e_pParticleBase);
		void				AddActPolicy(cParticleBase*e_pParticleBase);
		void				SetupPolicy();
		//
		virtual std::string	GetDataInfo();
		//input the output data string,and analyze it
		virtual bool		SetDataByDataString(const char*e_pString);
		bool				SetDataByDataString(std::string e_String) { return SetDataByDataString(e_String.c_str()); }

		void				SetEmitDirection(Vector3 e_vDirection);
		Vector3				GetEmitDirection();
		Vector3*			GetEmitDirectionPointer();

		int					GetMaxParticleCount(){return m_iMaxParticleCount;}
		void				SetMaxParticleCount(int	 e_iMaxParticleCount);
		bool				IsActived(){ return m_bActived; }
		//
		virtual void		Update(float e_fElpaseTime)override;
		//all alive particle to be killed and call shot
		void				Emit(Vector3 e_vPos,float e_fStartTime = 0.0001f,bool e_bKillOldParticles = true);
		void				Emit(bool e_bKillOldParticles = true);
		void				Stop()override { m_iCurrentWorkingParticles = 0; this->m_iCurrentEmitCount = this->m_iParticleEmitCount;  }
		//for 2D using,if particle has been rotated,it does't work,because it have to transform or fetch matrix to get last position.
		//it cose a lot work,so I rather forget this.
		virtual	void		KillParticleByOutRange(RECT e_rc);
		int					GetCurrentWorkingParticles();
		//ensure if u do not expect loop, the emit count is what u want
		void				SetLoop(bool e_bLoop);
		bool				IsLoop();
		virtual	void		RearrangeTime(float e_fNewTime)override { assert(0&&"not support"); }
		virtual	void		RearrangeTimeByPercent(float e_fPercenttage)override { assert(0&&"not support"); }
		virtual	void		UpdateByGlobalTime(float e_fGlobalTime)override { assert(0&&"not support"); }
		virtual	void		RenderByGlobalTime(float e_fTime)override {assert(0&&"not support");}
		virtual	void		InvertOrder()override {}
		virtual	float		GetEndTime()override { return -1.f; }
	};
//end namespace FATMING_CORE
}