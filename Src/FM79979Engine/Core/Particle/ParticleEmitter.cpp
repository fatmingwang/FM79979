#include "../stdafx.h"
#include "ParticleBase.h"
#include "ParticleEmitter.h"
#include "PrtVelocity.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cPrtEmitter);
	char	g_strTempStringForOutput[MAX_PATH];

	cQuickUpdateParticleObjectListByName::cQuickUpdateParticleObjectListByName
		(cQuickUpdateParticleObjectListByName*e_pQuickUpdateParticleObjectListByName,cPrtEmitter*e_pCloneTargetPrtEmitter)
	{
		for(int i=0;i<e_pQuickUpdateParticleObjectListByName->Count();++i)
		{
			cParticleBase*l_pParticleBase = dynamic_cast<cParticleBase*>((*e_pQuickUpdateParticleObjectListByName)[i]->Clone());
			l_pParticleBase->SetEmitterOwner(e_pCloneTargetPrtEmitter);
			this->m_ObjectList.push_back(l_pParticleBase);
		}
		GenerateListBuffer();
	}
	//cPrtTextureActDynamicTexture
	bool	cQuickUpdateParticleObjectListByName::IsOwenrOfPolicyIsImportant()
	{
		for(int i=0;i<this->Count();++i)
		{
			cParticleBase*l_pParticleBase = (*this)[i];
			if( l_pParticleBase->IsOwnerIsImportant() )
				return true;
		}
		return false;
	}

	cPrtEmitter::cPrtEmitter(const wchar_t*e_pName)
	{
		m_iPlayCount = -1;
		m_fCurrentTime = 0.f;
		m_pTexture = 0;
		m_iPrimitiveType = GL_QUADS;
		this->m_pVelocityInit = new cPrtVelocityInitSetVelocity();
		m_fGapTimeToShot = 0.5f;
		m_iCurrentEmitCount = 0;
		m_iParticleEmitCount = 1;
		m_iCurrentWorkingParticles = 0;
		m_iEmitParticleAmount = 5;
		m_iMaxParticleCount = -1;
		m_pvAllColorPointer = 0;
		m_pvAllPosPointer = 0;
		m_pvAllTexCoordinatePointer = 0;
		m_pParticleData = 0;
		this->SetMaxParticleCount(100);
		m_pInitPolicyParticleList = new cQuickUpdateParticleObjectListByName();
		m_pActPolicyParticleList = new cQuickUpdateParticleObjectListByName;
		m_bPolicyFromClone = false;
		m_bActived = false;
		SetName(e_pName);
		m_SrcBlendingMode = GL_SRC_ALPHA;
		m_DestBlendingMode = GL_ONE_MINUS_SRC_ALPHA;
	}

	cPrtEmitter::cPrtEmitter(cPrtEmitter*e_pPrtEmitter,bool e_bPolicyFromClone)
		:cFMTimeLineAnimationRule(e_pPrtEmitter)
	{
		m_iPlayCount = -1;
	    m_bActived = false;
		m_pVelocityInit = 0;
		m_pParticleData = 0;
		m_pvAllColorPointer = 0;
		m_pvAllPosPointer = 0;
		m_pvAllTexCoordinatePointer = 0;
		m_pInitPolicyParticleList = 0;
		m_pActPolicyParticleList = 0;
		m_iPrimitiveType = e_pPrtEmitter->GetPrimitiveType();
		m_SrcBlendingMode = e_pPrtEmitter->GetSrcBlendingMode();
		m_DestBlendingMode = e_pPrtEmitter->GetDestBlendingMode();
		m_pVelocityInit = dynamic_cast<cParticleBase*>(e_pPrtEmitter->GetVelocity()->Clone());
		m_fCurrentTime = 0.f;
		m_fGapTimeToShot = e_pPrtEmitter->GetGapTimeToShot();
		m_iCurrentEmitCount = 0;
		m_iParticleEmitCount = e_pPrtEmitter->GetParticleEmitCount();
		m_iCurrentWorkingParticles = 0;
		m_iEmitParticleAmount = e_pPrtEmitter->GetEmitParticleAmount();
		m_iMaxParticleCount = -1;
		m_pvAllPosPointer = 0;
		m_pvAllTexCoordinatePointer = 0;
		m_pParticleData = 0;
		m_pTexture = e_pPrtEmitter->m_pTexture;
		this->SetMaxParticleCount(e_pPrtEmitter->GetMaxParticleCount());
		//cPrtTextureActDynamicTexture
		if( e_pPrtEmitter->GetInitPolicyParticleList()->IsOwenrOfPolicyIsImportant() || e_pPrtEmitter->GetActPolicyParticleList()->IsOwenrOfPolicyIsImportant() )
			e_bPolicyFromClone = false;
		if( e_bPolicyFromClone )
		{
			m_pInitPolicyParticleList = e_pPrtEmitter->GetInitPolicyParticleList();
			m_pActPolicyParticleList = e_pPrtEmitter->GetActPolicyParticleList();
		}
		else
		{
			m_pInitPolicyParticleList = new cQuickUpdateParticleObjectListByName(e_pPrtEmitter->GetInitPolicyParticleList(),this);
			m_pActPolicyParticleList = new cQuickUpdateParticleObjectListByName(e_pPrtEmitter->GetActPolicyParticleList(),this);
		}
		m_bPolicyFromClone = e_bPolicyFromClone;
		SetName(e_pPrtEmitter->GetName());
	}

	cPrtEmitter::~cPrtEmitter()
	{
		SAFE_DELETE(m_pVelocityInit);
		SAFE_DELETE(m_pParticleData);
		SAFE_DELETE(m_pvAllColorPointer);
		SAFE_DELETE(m_pvAllPosPointer);
		SAFE_DELETE(m_pvAllTexCoordinatePointer);

		if( !m_bPolicyFromClone )
		{
			SAFE_DELETE(m_pInitPolicyParticleList);
			SAFE_DELETE(m_pActPolicyParticleList);
		}
	}

	void	cPrtEmitter::SetTexture(cBaseImage*e_pTexture)
	{
		this->m_pTexture = e_pTexture;
		if(this->m_pTexture)
		{
			memcpy(m_pvAllTexCoordinatePointer,UVToTwoTriangle(m_pTexture->GetUV()),sizeof(Vector2)*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT);
		}
		else
		{
			m_pvAllTexCoordinatePointer[0].x = 0.f;			m_pvAllTexCoordinatePointer[0].y = 1.f;
			m_pvAllTexCoordinatePointer[1].x = 1.f;			m_pvAllTexCoordinatePointer[1].y = 1.f;
			m_pvAllTexCoordinatePointer[2].x = 0.f;			m_pvAllTexCoordinatePointer[2].y = 0.f;

			m_pvAllTexCoordinatePointer[3].x = 0.f;			m_pvAllTexCoordinatePointer[3].y = 0.f;
			m_pvAllTexCoordinatePointer[4].x = 1.f;			m_pvAllTexCoordinatePointer[4].y = 1.f;
			m_pvAllTexCoordinatePointer[5].x = 1.f;			m_pvAllTexCoordinatePointer[5].y = 0.f;
		}
		if( m_pTexture && m_pTexture->Type() != cTexture::TypeID )
		{
			for( int i=1;i<m_iMaxParticleCount;++i )
			{
				memcpy(m_pvAllTexCoordinatePointer[i*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT],m_pvAllTexCoordinatePointer[0],sizeof(Vector2)*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT);
			}
		}
	}

	void	cPrtEmitter::AddInitPolicy(cParticleBase*e_pParticleBase)
	{
		m_pInitPolicyParticleList->AddObjectNeglectExist(e_pParticleBase); 
		e_pParticleBase->m_pEmitterOwner = this;
	}
	void	cPrtEmitter::AddActPolicy(cParticleBase*e_pParticleBase)
	{
		m_pActPolicyParticleList->AddObjectNeglectExist(e_pParticleBase); 
		e_pParticleBase->m_pEmitterOwner = this;
	}
	void	cPrtEmitter::SetupPolicy()
	{
		assert(m_pInitPolicyParticleList->Count()<MAX_PARTICLE_BASE_COUNT);
		assert(m_pActPolicyParticleList->Count()<MAX_PARTICLE_BASE_COUNT);
		m_pInitPolicyParticleList->GenerateListBuffer();
		m_pActPolicyParticleList->GenerateListBuffer();
	}
	//
	char*	cPrtEmitter::GetDataInfo()
	{
		sprintf(g_strTempStringForOutput,"%.3f,%d,%d,%d,%d,%d,%d,%s\0",
			m_fGapTimeToShot,
			m_iParticleEmitCount,
			m_iEmitParticleAmount,
			m_iMaxParticleCount,
			this->m_SrcBlendingMode,
			this->m_DestBlendingMode,
			m_iPrimitiveType,
			m_pVelocityInit->GetOutputDataString());
		return g_strTempStringForOutput;
	}
	//input the output data string,and analyze it
	bool	cPrtEmitter::SetDataByDataString(char*e_pString)
	{
		char* l_pString = strtok(e_pString,",");
		m_fGapTimeToShot = (float)atof(l_pString);
		l_pString = strtok(0,",");
		m_iParticleEmitCount = atoi(l_pString);
		l_pString = strtok(0,",");
		m_iEmitParticleAmount = atoi(l_pString);
		l_pString = strtok(0,",");
		int	l_iMaxParticleCount = atoi(l_pString);
		this->SetMaxParticleCount(l_iMaxParticleCount);
		l_pString = strtok(0,",");
		this->m_SrcBlendingMode = (GLenum)atoi(l_pString);
		l_pString = strtok(0,",");
		this->m_DestBlendingMode = (GLenum)atoi(l_pString);
		l_pString = strtok(0,",");
		this->m_iPrimitiveType = (GLenum)atoi(l_pString);
		l_pString = strtok(0,"\0");
		return m_pVelocityInit->SetDataByDataString(l_pString);
	}

	Vector3 cPrtEmitter::GetEmitDirection()
	{
		Vector3	l_Direction;
		if(m_pVelocityInit->Type() == cPrtVelocityInitSetVelocity::TypeID)
		{
			cPrtVelocityInitSetVelocity*l_pPrtVelocityInitSetVelocity = (cPrtVelocityInitSetVelocity*)m_pVelocityInit;
			l_Direction =  *l_pPrtVelocityInitSetVelocity->GetVeolcity();
		}
		return l_Direction;
	}

	Vector3* cPrtEmitter::GetEmitDirectionPointer()
	{
		Vector3	*l_pDirection = 0;
		if(m_pVelocityInit->Type() == cPrtVelocityInitSetVelocity::TypeID)
		{
			cPrtVelocityInitSetVelocity*l_pPrtVelocityInitSetVelocity = (cPrtVelocityInitSetVelocity*)m_pVelocityInit;
			return l_pPrtVelocityInitSetVelocity->GetVeolcity();
		}
		return l_pDirection;
	}

	void	cPrtEmitter::SetEmitDirection(Vector3 e_vDirection)
	{
		Vector3	l_vDir = e_vDirection.Normalize();
		cMatrix44	l_mat = cMatrix44::Identity;
		//l_mat = l_mat.XAxisRotationMatrix(l_vDir.x)*l_mat.YAxisRotationMatrix(l_vDir.x)*l_mat.ZAxisRotationMatrix(l_vDir.x);
		if(m_pVelocityInit->Type() == cPrtVelocityInitSetVelocity::TypeID)
		{
			cPrtVelocityInitSetVelocity*l_pPrtVelocityInitSetVelocity = (cPrtVelocityInitSetVelocity*)m_pVelocityInit;
			Vector3 l_vVelocity =  *l_pPrtVelocityInitSetVelocity->GetVeolcity();
			//l_vVelocity = l_mat.TransformVector(l_vVelocity);
			//((cPrtVelocityInitSetVelocity*)(m_pVelocityInit))->SetVelocity(l_vVelocity);
			((cPrtVelocityInitSetVelocity*)(m_pVelocityInit))->SetVelocity(l_vVelocity.Length()*l_vDir);
		}
		else
			assert(0);
	}

	void	cPrtEmitter::SetMaxParticleCount(int	 e_iMaxParticleCount)
	{
		assert(e_iMaxParticleCount);
		if( e_iMaxParticleCount != m_iMaxParticleCount )
		{
			int	l_iOriginalSize = m_iMaxParticleCount;
			m_iMaxParticleCount = e_iMaxParticleCount;
			sParticleData*l_pParticleData = new sParticleData[m_iMaxParticleCount];
			Vector2	*l_pvAllTexCoordinatePointer = new Vector2[m_iMaxParticleCount*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
			Vector3	*l_pvAllPosPointer = new Vector3[m_iMaxParticleCount*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
			Vector4	*l_pvAllColorPointer = new Vector4[m_iMaxParticleCount*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
			memset(l_pParticleData,0,sizeof(sParticleData)*m_iMaxParticleCount);
			if( l_iOriginalSize>m_iMaxParticleCount )
				l_iOriginalSize = m_iMaxParticleCount;
			if(l_iOriginalSize>0)
			{
				memcpy(l_pParticleData,m_pParticleData,sizeof(sParticleData)*l_iOriginalSize);
				memcpy(l_pvAllPosPointer,m_pvAllPosPointer,sizeof(Vector3)*l_iOriginalSize*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT);
				memcpy(l_pvAllColorPointer,m_pvAllColorPointer,sizeof(Vector4)*l_iOriginalSize*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT);
				memcpy(l_pvAllTexCoordinatePointer,m_pvAllTexCoordinatePointer,sizeof(Vector2)*l_iOriginalSize*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT);
			}
			SAFE_DELETE(m_pParticleData);
			SAFE_DELETE(m_pvAllColorPointer);
			SAFE_DELETE(m_pvAllPosPointer);
			SAFE_DELETE(m_pvAllTexCoordinatePointer);
			m_pParticleData = l_pParticleData;
			m_pvAllColorPointer = l_pvAllColorPointer;
			m_pvAllPosPointer = l_pvAllPosPointer;
			m_pvAllTexCoordinatePointer = l_pvAllTexCoordinatePointer;
			if(this->m_pTexture)
			{
				memcpy(m_pvAllTexCoordinatePointer,UVToTwoTriangle(m_pTexture->GetUV()),sizeof(Vector2)*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT);
			}
			else
			{
				m_pvAllTexCoordinatePointer[0].x = 0.f;			m_pvAllTexCoordinatePointer[0].y = 1.f;
				m_pvAllTexCoordinatePointer[1].x = 1.f;			m_pvAllTexCoordinatePointer[1].y = 1.f;
				m_pvAllTexCoordinatePointer[2].x = 0.f;			m_pvAllTexCoordinatePointer[2].y = 0.f;

				m_pvAllTexCoordinatePointer[3].x = 0.f;			m_pvAllTexCoordinatePointer[3].y = 0.f;
				m_pvAllTexCoordinatePointer[4].x = 1.f;			m_pvAllTexCoordinatePointer[4].y = 1.f;
				m_pvAllTexCoordinatePointer[5].x = 1.f;			m_pvAllTexCoordinatePointer[5].y = 0.f;
			}
			for( int i=1;i<m_iMaxParticleCount;++i )
			{
				memcpy(m_pvAllTexCoordinatePointer[i*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT],m_pvAllTexCoordinatePointer[0],sizeof(Vector2)*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT);
			}
		}
	}

	void	cPrtEmitter::Shot(int e_iNumParticle,float e_fElpaseTime)
	{
		int	l_iCurrentWorkingParticleLastIndex = m_iCurrentWorkingParticles;
		int	l_iGoShotCount = e_iNumParticle;
		if( l_iGoShotCount+m_iCurrentWorkingParticles>m_iMaxParticleCount  )
			l_iGoShotCount = m_iMaxParticleCount-m_iCurrentWorkingParticles;
		m_iCurrentWorkingParticles += l_iGoShotCount;
		assert(m_iCurrentWorkingParticles<=m_iMaxParticleCount);
		for( int i=l_iCurrentWorkingParticleLastIndex;i<m_iCurrentWorkingParticles;++i )
		{
			m_pParticleData[i].vPos = m_vPos;
			m_pParticleData[i].vOriginalPos = m_vPos;
			m_pParticleData[i].ePRM = ePRM_NONE;
			m_pParticleData[i].vAngle = Vector3(0,0,0);
			m_pVelocityInit->Update(0.001f,i,&m_pParticleData[i]);//setup velocity
			m_pInitPolicyParticleList->Update(e_fElpaseTime,i,&m_pParticleData[i]);
		}	
	}

	void	cPrtEmitter::Shot(float e_fElpaseTime)
	{
		this->SetAnimationDone(false);
		m_bActived = true;
		int	l_iCurrentWorkingParticleLastIndex = m_iCurrentWorkingParticles;
		int	l_iGoShotCount = m_iEmitParticleAmount;
		if( l_iGoShotCount+m_iCurrentWorkingParticles>m_iMaxParticleCount  )
			l_iGoShotCount = m_iMaxParticleCount-m_iCurrentWorkingParticles;
		m_iCurrentWorkingParticles += l_iGoShotCount;
		assert(m_iCurrentWorkingParticles<=m_iMaxParticleCount);
		for( int i=l_iCurrentWorkingParticleLastIndex;i<m_iCurrentWorkingParticles;++i )
		{
			m_pParticleData[i].vPos = m_vPos;
			m_pParticleData[i].vOriginalPos = m_vPos;
			m_pParticleData[i].ePRM = ePRM_NONE;
			m_pParticleData[i].vAngle = Vector3(0,0,0);
			m_pVelocityInit->Update(e_fElpaseTime,i,&m_pParticleData[i]);//setup velocity
			//INIT UPDATE DO NOT CARE ELPASE TIME.
			m_pInitPolicyParticleList->Update(1.f,i,&m_pParticleData[i]);
		}
	}

	void	cPrtEmitter::InternalInit()
	{
		m_bStart  =true;
		Emit(m_vPos,EPSIONAL,false);
	}

	void	cPrtEmitter::Emit(bool e_bKillOldParticles)
	{
		m_iCurrentEmitCount = 1;
		if( e_bKillOldParticles )
			m_iCurrentWorkingParticles = 0;
		Emit(m_vPos,0.f,e_bKillOldParticles);	
	}
	void	cPrtEmitter::Update(float e_fElpaseTime)
	{
		InternalUpdate(e_fElpaseTime);
	}
	//if u call it twice in a short time,u could do the thing u may not expect,
	//here suggest u clone and shot,if u wanna to shot 2 objects at same time
	void	cPrtEmitter::Emit(Vector3 e_vPos,float e_fStartTime,bool e_bKillOldParticles)
	{
		this->m_vPos = e_vPos;	
		m_iCurrentEmitCount = 1;
		if( e_bKillOldParticles )
			m_iCurrentWorkingParticles = 0;
		m_fCurrentTime = 0.f;
		Shot(e_fStartTime);
	}

	void	cPrtEmitter::InternalUpdate(float e_fElpaseTime)
	{
		if(this->m_bActived)
		{
			ShotUpdate(e_fElpaseTime);
			ParticleUpdate(e_fElpaseTime);
		}
	}
	cMatrix44	cPrtEmitter::GetParticleDataMatrix(sParticleData*e_pParticleData)
	{
		cMatrix44	l_mat = cMatrix44::Identity;
		if( e_pParticleData->ePRM == ePRM_NONE )			
			l_mat = cMatrix44::TranslationMatrix(e_pParticleData->vPos);
		else
		{
			if( e_pParticleData->ePRM == ePRM_SELF )
			{
				l_mat = cMatrix44::TranslationMatrix(e_pParticleData->vPos);
				if( e_pParticleData->vAngle.x!=0.f )
					l_mat *= cMatrix44::XAxisRotationMatrix(e_pParticleData->vAngle.x);
				if( e_pParticleData->vAngle.y!=0.f )
					l_mat *= cMatrix44::YAxisRotationMatrix(e_pParticleData->vAngle.y);
				if( e_pParticleData->vAngle.z!=0.f )
					l_mat *= cMatrix44::ZAxisRotationMatrix(e_pParticleData->vAngle.z);
				//U COULD INSTEAD JUST ONE LINE HERE
				//l_mat = cMatrix44::TranslationMatrix(e_pParticleData->vPos)*Quaternion::EulerRotationQuaternion(e_pParticleData->vAngle.x,e_pParticleData->vAngle.y,e_pParticleData->vAngle.z).ToMatrix();
			}
			else
			if( e_pParticleData->ePRM == ePRM_WORLD )
			{
				if( e_pParticleData->vAngle.x!=0.f )
					l_mat = cMatrix44::XAxisRotationMatrix(e_pParticleData->vAngle.x);
				if( e_pParticleData->vAngle.y!=0.f )
					l_mat *= cMatrix44::YAxisRotationMatrix(e_pParticleData->vAngle.y);
				if( e_pParticleData->vAngle.z!=0.f )
					l_mat *= cMatrix44::ZAxisRotationMatrix(e_pParticleData->vAngle.z);
				l_mat *= cMatrix44::TranslationMatrix(e_pParticleData->vPos);
				//U COULD INSTEAD JUST ONE LINE HERE
				//l_mat = Quaternion::EulerRotationQuaternion(e_pParticleData->vAngle.x,e_pParticleData->vAngle.y,e_pParticleData->vAngle.z).ToMatrix()*l_mat;
			}
		}
		return l_mat;
	}

	void	cPrtEmitter::InternalRender()
	{
		if(this->m_bActived&&m_iCurrentWorkingParticles>0)
		{
			UseShaderProgram(DEFAULT_SHADER);
			//this one should be called by UseParticleShaderProgram,but u might want to setup it's new position if u need
			//SetupParticleShaderWorldMatrix(cMatrix44::Identity);
			GLint	l_Src,l_Dest;
			glGetIntegerv(GL_BLEND_SRC,&l_Src);
			glGetIntegerv(GL_BLEND_DST,&l_Dest);
			bool	l_bSameBleinding = false;
			if( l_Src == m_SrcBlendingMode&&m_DestBlendingMode == l_Dest  )
				l_bSameBleinding = true;
			else
				glBlendFunc(m_SrcBlendingMode,m_DestBlendingMode);
			if(m_pTexture )
				m_pTexture->ApplyImage();
			cMatrix44	l_mat;
			cMatrix44	l_matWorldTransform = this->GetWorldTransform();
			if( m_iPrimitiveType == GL_QUADS )
			{
				for(int i=0;i<this->m_iCurrentWorkingParticles;++i)
				{
					int	l_iIndex = i*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT;
					sParticleData*l_pParticleData = &m_pParticleData[i];
					l_mat = GetParticleDataMatrix(l_pParticleData);
					l_mat = l_matWorldTransform*l_mat;
					for( int j=0;j<TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT;++j )
					{
						m_pvAllColorPointer[l_iIndex+j] = l_pParticleData->vColor;
					}
					Vector3	l_vPos = l_mat.GetTranslation();
					Vector3	l_vRight = l_mat.GetAxis(MyMath::X);
					Vector3	l_vUp = l_mat.GetAxis(MyMath::Y);
					Vector2	l_vHalfSize(l_pParticleData->vSize.x/2,l_pParticleData->vSize.y/2);
					//first triangle	second triangle
					//2,3				5
					//0					1,4
					//set l_vPos as center
					m_pvAllPosPointer[l_iIndex] = l_vPos-(l_vHalfSize.x*l_vRight)-(l_vHalfSize.y*l_vUp);
					l_vPos = m_pvAllPosPointer[l_iIndex];
					l_vPos += (l_pParticleData->vSize.y*l_vUp);
					m_pvAllPosPointer[l_iIndex+2] = l_vPos;
					m_pvAllPosPointer[l_iIndex+3] = l_vPos;
					l_vPos += (l_pParticleData->vSize.x*l_vRight);
					m_pvAllPosPointer[l_iIndex+5] = l_vPos;
					l_vPos -= (l_pParticleData->vSize.y*l_vUp);
					m_pvAllPosPointer[l_iIndex+1] = l_vPos;
					m_pvAllPosPointer[l_iIndex+4] = l_vPos;
				}
				SetupShaderWorldMatrix(cMatrix44::Identity);
				myGlVertexPointer(  3, m_pvAllPosPointer );
				myGlUVPointer( 2, m_pvAllTexCoordinatePointer );
				myGlColorPointer( 4,  m_pvAllColorPointer );
				//GL_TRIANGLE_STRIP will conbine together so do not work for particle
				MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0, m_iCurrentWorkingParticles*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT);
			}
			else
			if( m_iPrimitiveType == GL_POINTS )
			{
				//assert(0&&"soory.....I am lazy to do this with opengl es");
				//return;
				//make sure the shader has no texture attribute.
				for(int i=0;i<this->m_iCurrentWorkingParticles;++i)
				{
					sParticleData*l_pParticleData = &m_pParticleData[i];
					m_pvAllColorPointer[i] = l_pParticleData->vColor;
					m_pvAllPosPointer[i] = l_pParticleData->vPos;
				}
				//here could speed up by shader and change size.
#ifndef OPENGLES_2_X
				glPointSize(15);
#endif//set gl_PointSize value at shader code.
				UseShaderProgram(NO_TEXTURE_SHADER);
				//myGlColorPointer(4, m_pvAllColorPointer);
				myGlVertexPointer(3, m_pvAllPosPointer);
				MY_GLDRAW_ARRAYS(GL_POINTS, 0, m_iCurrentWorkingParticles);
			}
#ifdef DEBUG
			else
				assert(0&&"do not support such privmative");
#endif
			if(!l_bSameBleinding)
				glBlendFunc(l_Src,l_Dest);
		}
	}

	void	cPrtEmitter::KillParticleByOutRange(RECT e_rc)
	{
		for(int i=0;i<this->m_iCurrentWorkingParticles;)
		{
			sParticleData*l_pParticleData = &this->m_pParticleData[i];
			if( e_rc.left  < l_pParticleData->vPos.x&&
				e_rc.right > l_pParticleData->vPos.x&&
				e_rc.top   < l_pParticleData->vPos.y&&
				e_rc.bottom> l_pParticleData->vPos.y)
			{
				i++;
			}
			else
			{
				//set current particle data to last working data,and minus the working count
				m_pParticleData[i] = m_pParticleData[m_iCurrentWorkingParticles - 1];
				m_iCurrentWorkingParticles--;
				if( m_iCurrentWorkingParticles == 0 )
				{
					if( m_iParticleEmitCount != 0 )
					{
						this->m_bActived = false;
						this->SetAnimationDone(true);
					}
				}				
			}
		}	
	}

	int		cPrtEmitter::GetCurrentWorkingParticles(){ return m_iCurrentWorkingParticles; }
	void	cPrtEmitter::SetLoop(bool e_bLoop)
	{
		this->SetAnimationLoop(e_bLoop);
		if( e_bLoop )
		{
			m_iParticleEmitCount = 0;
		}
		else 
			m_iParticleEmitCount = 1;
	}
	bool	cPrtEmitter::IsLoop(){ return m_iParticleEmitCount==0?true:false; }
//end namespace FATMING_CORE
}