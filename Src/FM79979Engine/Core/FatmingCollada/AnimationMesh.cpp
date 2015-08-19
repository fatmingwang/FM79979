#include "StdAfx.h"
#include "AnimationMesh.h"
#include "FatmingController.h"

#ifndef MIN_VALID_WEIGHT
#define MIN_VALID_WEIGHT 0.001f
#endif
extern void	DumpBoneInfo(cBone*e_pBone);

const wchar_t*     cAnimationMesh::TypeID( L"cAnimationMesh" );
//========================================================
//
//========================================================
cBone::cBone(const WCHAR*e_strName)
{
	//m_mtFinalMatrix = cMatrix44::Identity;
	//swprintf(m_strSidName,MAX_PATH,L"%ls\0",e_strSidName);
	SetName(e_strName);
	m_fMinKeyTime = 0;
	m_fMaxKeyTime = 0;
	m_bAnimation = false;
	m_bDestroyConnectionWhileDestroy = false;
}
//========================================================
//
//========================================================
cBone::cBone(cBone*e_pBone)
{
	//swprintf(m_strSidName,MAX_PATH,L"%ls\0",e_pBone->m_strSidName);
	SetName(e_pBone->GetName());
	m_bAnimation = e_pBone->m_bAnimation;
	m_fMinKeyTime = e_pBone->m_fMinKeyTime;
	m_fMaxKeyTime = e_pBone->m_fMaxKeyTime;
	m_FormKeyFrames = e_pBone->m_FormKeyFrames;
	m_matInvBindPose = e_pBone->m_matInvBindPose;
	m_bDestroyConnectionWhileDestroy = e_pBone->m_bDestroyConnectionWhileDestroy;
	this->SetLocalTransform(this->GetLocalTransform());
}
//========================================================
//
//========================================================
//int	l_iii = 0;
cBone::~cBone()
{
//	DestoryAllChild(this);
}
//========================================================
//
//========================================================
cBone*	cBone::FinChildByName(WCHAR*e_strBoneName)
{
	if( !wcscmp(e_strBoneName,GetName()) )
		return this;
	if( this->GetNextSibling() != nullptr)
	{		
		return ((cBone*)(GetNextSibling()))->FinChildByName(e_strBoneName);
	}

	if (GetFirstChild() != nullptr)
	{
		return ((cBone*)(GetFirstChild()))->FinChildByName(e_strBoneName);
	}
	return nullptr;
}
//========================================================
//
//========================================================
void	cBone::SetFormKeyFrames(FloatTocMatrix44Map	e_FormKeyFrames)
{
	m_FormKeyFrames = e_FormKeyFrames;
	UINT	l_uiSize = m_FormKeyFrames.size();
	float l_fMinKeyTime(FLT_MAX), l_fMaxKeyTime(FLT_MIN),l_fKeyTime;
	for( FloatTocMatrix44Map::iterator l_Iterator = m_FormKeyFrames.begin();l_Iterator!=m_FormKeyFrames.end();++l_Iterator )
	{
		l_fKeyTime = l_Iterator->first;
        l_fMinKeyTime = min(l_fMinKeyTime, l_fKeyTime);
        l_fMaxKeyTime = max(l_fMaxKeyTime, l_fKeyTime);
	}
	m_fMaxKeyTime = l_fMaxKeyTime;
	m_fMinKeyTime = l_fMinKeyTime;
	if( e_FormKeyFrames.size() )
	{
		m_bAnimation = true;
	}
	else
	{
		cGameApp::OutputDebugInfoString(this->GetName());
		cGameApp::OutputDebugInfoString(L"animation frame is zero");
	}
}
//========================================================
//
//========================================================
//void	cBone::DestoryAllChild(cBone*e_pNextBone)
//{
//	cBone*	l_pNextBone = 0;
//	if( e_pNextBone->GetNextSibling() != nullptr)
//	{
//		l_pNextBone = (cBone*)e_pNextBone->GetNextSibling();
//		l_pNextBone->DestoryAllChild(l_pNextBone);
//	}
//
//	if (e_pNextBone->GetFirstChild() != nullptr)
//	{
//		l_pNextBone =(cBone*)e_pNextBone->GetFirstChild();
//		l_pNextBone->DestoryAllChild(l_pNextBone);
//	}
//	//l_iii++;
//	//WCHAR	l_str[MAX_PATH];
//	//swprintf_s(l_str,MAX_PATH,L"%s-%d\n",l_pNextBone?l_pNextBone->GetName():L"..",l_iii);
//	////_itow(l_iii,l_str,10);
//	//if( l_iii == 185)
//	//{
//	//	int a=0;
//	//	int b=a;
//	//}
//	//cGameApp::OutputDebugInfoString(l_str);
//	SAFE_DELETE(l_pNextBone);
//}
//========================================================
//
//========================================================
void	cBone::EvaluateLocalXForm(float timeValue,bool e_bSetChildBonesDirty)
{
	if(m_FormKeyFrames.size() == 0)
		return;

	//find keyframe before and after and do linear interpolation between them
	FloatTocMatrix44Map::iterator prevKey(m_FormKeyFrames.lower_bound(timeValue));
	FloatTocMatrix44Map::iterator nextKey(prevKey);
   
	if( (prevKey == m_FormKeyFrames.end()) ||
		((prevKey != m_FormKeyFrames.begin()) && (prevKey->first > timeValue)) )
		--prevKey;

	// because m_FormKeyFrames.size() != 0 prevKey should be a valid iterator here
	assert(prevKey != m_FormKeyFrames.end());

	if( (prevKey == nextKey) || (nextKey == m_FormKeyFrames.end()))
	{
		this->SetLocalTransform(prevKey->second.Transposed());
		return;
	}


	float time0 = prevKey->first;
	float time1 = nextKey->first;
	float l_fTimeDis = (timeValue - time0)/(time1-time0);

	const cMatrix44& m0 = prevKey->second;
	const cMatrix44& m1 = nextKey->second;


	cMatrix44 mNew = m0;

	// Get axis vectors
	Vector3 m0x( m0[0] ), m0y( m0[1] ), m0z( m0[2] );
	Vector3 m1x( m1[0] ), m1y( m1[1] ), m1z( m1[2] );
	//Vector3 m0x( m0._11,m0._21,m0._31 );
	//Vector3 m0y( m0._12,m0._22,m0._32 );
	//Vector3 m0z( m0._31,m0._32,m0._33 );
	//Vector3 m1x( m1._11,m1._21,m1._31 );
	//Vector3 m1y( m1._12,m1._22,m1._32 );
	//Vector3 m1z( m1._31,m1._32,m1._33 );
    #define SCALE_EPSILON 0.02f
    if( ((m0x.LengthSquared() - 1.0f) > SCALE_EPSILON) || ((m0y.LengthSquared() - 1.0f) > SCALE_EPSILON) ||
        ((m0z.LengthSquared() - 1.0f) > SCALE_EPSILON) || ((m1x.LengthSquared() - 1.0f) > SCALE_EPSILON) ||
        ((m1y.LengthSquared() - 1.0f) > SCALE_EPSILON) || ((m1z.LengthSquared() - 1.0f) > SCALE_EPSILON) )
    {
		//assert(0&&"sorry I am lazy to do scale about skinning");
        // TODO: need to interpolate scaling too
    }
	else
	{
		// Get translation components
		Vector3 t0 = m0.GetTranslation();
		Vector3 t1 = m1.GetTranslation();

		// Get axis of rotation from m0 to m1
		Vector3 dX(m1x - m0x), dY(m1y - m0y), dZ(m1z - m0z);
		Vector3 axis = (dX ^ dY) + (dY ^ dZ) + (dZ ^ dX);

		#define VLENGTH_EPSILON 0.00000001f
		if( axis.Length() >= VLENGTH_EPSILON )
		{
			Vector3 dU = dX;
			Vector3 U = m0x;
			if( dU.Length() < VLENGTH_EPSILON )
			{
				dU = dY;
				U = m0y;
			}
            double angle;
            axis.NormalizeIt();


            Vector3 axisCrossU = (axis ^ U);

            // Flip axis if it's not aligned with dU
            if( (axisCrossU * dU ) < 0.0 )
                axis = -axis;

            // Get angle of rotation around axis that gets m0 to m1
            if( axisCrossU.Length() < VLENGTH_EPSILON )
                angle = 0;
            else
            {
                double a = dU.Length() / (2.0 * axisCrossU.Length());
                // clamp a before passing to asin to avoid incorrect results
                a = (a < -1.0) ? -1.0 : ((a > 1.0) ? 1.0 : a);
                angle = 2.0 * asin(a);
            }

            // Interpolate angle
            double aNew = l_fTimeDis * angle;

            // Computer rotation matrix
            cMatrix44 mRot( cMatrix44::AxisRotationMatrix(axis, (float)aNew) );

            // Rotate starting keyframe matrix by this rotation
            mNew = mRot*m0;
		}

		// Interpolate translation
		Vector3 tNew = t0 + l_fTimeDis * (t1 - t0);
		mNew.SetTranslation(tNew);
	}
	//directX to openGL
	mNew = mNew.Transposed();
	this->SetLocalTransform(mNew,e_bSetChildBonesDirty);
}

//========================================================
//
//========================================================
cAnimationMesh::cAnimationMesh(cMesh*e_pMesh,cMatrix44 e_matMeshBindShapePose):cMesh(e_pMesh)
{
	m_bOnlyInflunceJoint = false;
	m_iOnlyInflunceJoint = 0;
	m_iStartJoint = 0;
	//here......sux XD.
	//m_matMeshBindShapePose = cMatrix44::ZupToYUp*e_matMeshBindShapePose;
	m_matMeshBindShapePose = e_matMeshBindShapePose;
	m_fMinKeyTime = FLT_MIN;
	m_fMaxKeyTime = FLT_MIN;
	m_fCurrentTime = 0;
	m_fStartTime = 0;
	m_fEndTime = 0;
	if(!this->m_pVBOBuffer&&m_uiVertexBufferCount>0)
	{
		m_pvVertexBuferForSkinned = new float[m_uiVertexBufferCount*3];//xyz stride for 3
	}
	else
	{
		m_pvVertexBuferForSkinned = 0;
	}
	m_pAllBonesMatrixForSkinned = 0;
	m_pMainRootBone = 0;
	m_fLastNodesUpdateTime = 0.f;
}

cAnimationMesh::cAnimationMesh(cAnimationMesh*e_pAnimationMesh):cMesh(e_pAnimationMesh)
{
	m_bOnlyInflunceJoint = false;
	m_iOnlyInflunceJoint = 0;
	m_iStartJoint = 0;
	m_matMeshBindShapePose = e_pAnimationMesh->m_matMeshBindShapePose;
	m_pMainRootBone = e_pAnimationMesh->m_pMainRootBone;
	m_fMinKeyTime = e_pAnimationMesh->m_fMinKeyTime;
	m_fMaxKeyTime = e_pAnimationMesh->m_fMaxKeyTime;
	m_fCurrentTime = e_pAnimationMesh->m_fCurrentTime;
	m_fStartTime = e_pAnimationMesh->m_fStartTime;
	m_fEndTime = e_pAnimationMesh->m_fEndTime;
	m_fLastNodesUpdateTime = e_pAnimationMesh->m_fLastNodesUpdateTime;
	//
	this->m_SkinningBoneVector.CopyListPointer(&e_pAnimationMesh->m_SkinningBoneVector);
	m_AllBoneVector.CopyListPointer(&e_pAnimationMesh->m_AllBoneVector);
	this->CopyListPointer(e_pAnimationMesh);
	//
	m_pAllBonesMatrixForSkinned = new cMatrix44[this->m_SkinningBoneVector.Count()];
	m_pvVertexBuferForSkinned = e_pAnimationMesh->m_pvVertexBuferForSkinned;
}
//========================================================
//
//========================================================
cAnimationMesh::~cAnimationMesh()
{
	m_SkinningBoneVector.SetFromResource(true);
	m_AllBoneVector.Destroy();
	SAFE_DELETE(m_pAllBonesMatrixForSkinned);
	if( !m_AllBoneVector.IsFromResource() )
	{
		SAFE_DELETE(m_pvVertexBuferForSkinned);
	}
}
//========================================================
//
//========================================================
void	cAnimationMesh::SetBindShapePose(cMatrix44 e_mat)
{
//I am lazy to judge is it Y up or Zup in Collada parse,so I force to make bind shape to Yup
	this->m_matMeshBindShapePose = cMatrix44::ZupToYUp*e_mat;
}
//========================================================
//
//========================================================
void	cAnimationMesh::RefreshAnimationData()
{
	cBone**l_ppBone = &(*m_SkinningBoneVector.GetList())[0];
    m_fMinKeyTime = FLT_MAX;
    m_fMaxKeyTime = FLT_MIN;
	int	l_iNum = this->m_SkinningBoneVector.Count();
	for( int i=0;i<l_iNum;++i )
	{
		cBone*l_pBone = l_ppBone[i];
		m_fMinKeyTime = min(m_fMinKeyTime,l_pBone->GetMinKeyTime());
		m_fMaxKeyTime = max(l_pBone->GetMaxKeyTime(),m_fMaxKeyTime);
	}
    if(m_fMinKeyTime != FLT_MAX)
    {
        m_fStartTime = m_fMinKeyTime;
        m_fEndTime = m_fMaxKeyTime;
    }
	else 
	{
        m_fMinKeyTime = 0;
        m_fMaxKeyTime = 0;
    }
    if(m_fStartTime < 0) 
        m_fStartTime = 0;
	SAFE_DELETE(m_pAllBonesMatrixForSkinned);
	m_pAllBonesMatrixForSkinned = new cMatrix44[l_iNum];
	bool	l_bShowDebugInfo = true;
	if( l_bShowDebugInfo )
	{
		DumpBoneInfo(this->m_SkinningBoneVector[0]);
		cGameApp::OutputDebugInfoString(L"\n");
		for( int i=0;i<m_SkinningBoneVector.Count();++i )
		{
			//cGameApp::OutputDebugInfoString(m_SkinningBoneVector[i]->GetName());
			cGameApp::OutputDebugInfoString(m_SkinningBoneVector[i]->m_strSID.c_str());
			cGameApp::OutputDebugInfoString(L",");
			cGameApp::OutputDebugInfoString(m_SkinningBoneVector[i]->GetName());
			cGameApp::OutputDebugInfoString(L"\n");
		}
	}
}

void cAnimationMesh::UpdateNodes(float e_fTimeValue)
{
    assert((e_fTimeValue>=m_fMinKeyTime) && (e_fTimeValue <=m_fMaxKeyTime));
	//
    if(m_fLastNodesUpdateTime == e_fTimeValue)
        return;
	int	l_iNum = this->m_SkinningBoneVector.Count();
    for(int i=0;i<l_iNum;++i)
    {
		m_SkinningBoneVector[i]->EvaluateLocalXForm(e_fTimeValue,i==0?true:false);
		//UpdateNode(m_SkinningBoneVector[i],e_fTimeValue);
		//if(!m_SkinningBoneVector[i]->GetParent())
            //UpdateNode(m_SkinningBoneVector[i],e_fTimeValue);
    }
    m_fLastNodesUpdateTime = e_fTimeValue;
}

//========================================================
//
//========================================================
void	cAnimationMesh::UpdateNode(cBone*e_pBone,float e_fTime)
{
	e_pBone->EvaluateLocalXForm(e_fTime);
	cBone*l_pBone = (cBone*)e_pBone->GetFirstChild();
	if (l_pBone != nullptr)
	{
		UpdateNode( l_pBone,e_fTime);
	}
	l_pBone = (cBone*)e_pBone->GetNextSibling();
	if( l_pBone != nullptr)
	{
		UpdateNode( l_pBone,e_fTime);
	}
}
//========================================================
//
//========================================================
void	cAnimationMesh::JointUpdate(float elapsedTime)
{
    float newTime(m_fCurrentTime);
    // Handle time advance based on animation playback options
    if((m_fCurrentTime+elapsedTime) > m_fEndTime)
    {
        float endMinusStart = m_fEndTime-m_fStartTime;
        if( endMinusStart > 0.001)
        {
            //switch(m_animEndHandling)
            //{
            //case LOOP:
            //    {
                float overTime = (m_fCurrentTime + elapsedTime - m_fEndTime);
                newTime = m_fStartTime + fmod(overTime, endMinusStart);
            //    }
            //    break;
            //case BLOCK:
            //default:
            //    newTime = m_endTime;
            //    break;
            //}
        }
        else
        {
            newTime = m_fCurrentTime;
        }
    }
    else
    {
        newTime = m_fCurrentTime + elapsedTime;
    };    
    m_fCurrentTime = newTime;
	assert(this->m_fMinKeyTime <= this->m_fMaxKeyTime);
	float	l_fUpdateTime = 0;
	cBone*l_pBone = m_SkinningBoneVector[m_iStartJoint];
#ifdef DEBUG
	l_pBone = m_SkinningBoneVector[m_iStartJoint];
#endif
	//cBone*l_pBone = m_pMainRootBone;
    if(m_fCurrentTime > m_fMaxKeyTime)
		UpdateNode(l_pBone,m_fMaxKeyTime);
    else if(m_fCurrentTime< m_fMinKeyTime)
		UpdateNode(l_pBone,m_fMinKeyTime);
    else
		UpdateNode(l_pBone,m_fCurrentTime);
}
//========================================================
//
//========================================================
void	cAnimationMesh::Update(float elapsedTime)
{
    float newTime(m_fCurrentTime);
    // Handle time advance based on animation playback options
    if((m_fCurrentTime+elapsedTime) > m_fEndTime)
    {
        float endMinusStart = m_fEndTime-m_fStartTime;
        if( endMinusStart > 0.001)
        {
            //switch(m_animEndHandling)
            //{
            //case LOOP:
            //    {
                float overTime = (m_fCurrentTime + elapsedTime - m_fEndTime);
                newTime = m_fStartTime + fmod(overTime, endMinusStart);
            //    }
            //    break;
            //case BLOCK:
            //default:
            //    newTime = m_endTime;
            //    break;
            //}
        }
        else
        {
            newTime = m_fCurrentTime;
        }
    }
    else
    {
        newTime = m_fCurrentTime + elapsedTime;
    };    
    m_fCurrentTime = newTime;
	//if(m_fCurrentTime>m_fEndTime)
	//	m_fCurrentTime -= m_fEndTime;
	SetCurrentAnimationTime(m_fCurrentTime);
	int	l_iBoneSize = this->m_SkinningBoneVector.Count();
	cBone**l_ppBone = &(*m_SkinningBoneVector.GetList())[0];
	for(int i=0;i<l_iBoneSize;++i)
	{
		cMatrix44  l_WorldTransform =  l_ppBone[i]->GetWorldTransform()*l_ppBone[i]->GetInvBindPose()*m_matMeshBindShapePose;
		m_pAllBonesMatrixForSkinned[i] = l_WorldTransform;
	}
}
//========================================================
//
//========================================================
void	cAnimationMesh::SetCurrentAnimationTime(float e_fCurrentTime)
{
	this->m_fCurrentTime = e_fCurrentTime;
    if(m_fCurrentTime > m_fEndTime)
        UpdateNodes(m_fEndTime);
    else 
	if(m_fCurrentTime < this->m_fStartTime)
        UpdateNodes(m_fStartTime);
    else
        UpdateNodes(m_fCurrentTime);
	//UpdateNodes(m_fCurrentTime);
}

Vector3	cAnimationMesh::GetSkinWorldPosBySkinningData(Vector3 e_vPos,float*e_pfWeihts,char*e_pcInflunceBoneIndex)
{
	Vector3	l_vFinalPos = Vector3::Zero;
#ifdef DEBUG
	float	l_f[4] = {e_pfWeihts[0],e_pfWeihts[1],e_pfWeihts[2],e_pfWeihts[3]};
	float	l_fTotalWeight = l_f[0]+l_f[1]+l_f[2]+l_f[3];
	assert(l_f[0]>=0.f&&l_f[1]>=0.f&&l_f[2]>=0.f&&l_f[3]>=0.f);
	if( l_fTotalWeight<=0.9f || l_fTotalWeight>=1.1f )
	{
		assert(0);
	}
	char	l_InflunceJointIndex[4] = {e_pcInflunceBoneIndex[0],e_pcInflunceBoneIndex[1],e_pcInflunceBoneIndex[2],e_pcInflunceBoneIndex[3]};
#endif
#ifdef DEBUG
	if( m_bOnlyInflunceJoint )
	{
		bool	l_bMatchOneOfThem = false;
		if( (l_InflunceJointIndex[0] == m_iOnlyInflunceJoint&&l_f[0]>0.f)||
			(l_InflunceJointIndex[1] == m_iOnlyInflunceJoint&&l_f[1]>0.f)||
			(l_InflunceJointIndex[2] == m_iOnlyInflunceJoint&&l_f[2]>0.f)||
			(l_InflunceJointIndex[3] == m_iOnlyInflunceJoint&&l_f[3]>0.f))
			l_bMatchOneOfThem = true;
		if( l_bMatchOneOfThem )
		{
			if(e_pfWeihts[0]>MIN_VALID_WEIGHT)
			{
				l_vFinalPos += (m_pAllBonesMatrixForSkinned[e_pcInflunceBoneIndex[0]].TransformCoordinate(e_vPos)*e_pfWeihts[0]);
			}
			if(e_pfWeihts[1]>MIN_VALID_WEIGHT)
			{
				l_vFinalPos += (m_pAllBonesMatrixForSkinned[e_pcInflunceBoneIndex[1]].TransformCoordinate(e_vPos)*e_pfWeihts[1]);
			}
			if(e_pfWeihts[2]>MIN_VALID_WEIGHT)
			{
				l_vFinalPos += (m_pAllBonesMatrixForSkinned[e_pcInflunceBoneIndex[2]].TransformCoordinate(e_vPos)*e_pfWeihts[2]);
			}
			if(e_pfWeihts[3]>MIN_VALID_WEIGHT)
			{
				l_vFinalPos += (m_pAllBonesMatrixForSkinned[e_pcInflunceBoneIndex[3]].TransformCoordinate(e_vPos)*e_pfWeihts[3]);
			}
		}
		else
		{
			cMatrix44	l_ToYUp = cMatrix44::ZupToYUp;
			l_vFinalPos = l_ToYUp.TransformVector(e_vPos);
		}
	}
	else
#endif
	{
		float	l_fTotalWeight = 0.f;
		if(e_pfWeihts[0]>MIN_VALID_WEIGHT)
		{
			l_vFinalPos += ((m_pAllBonesMatrixForSkinned[e_pcInflunceBoneIndex[0]].TransformCoordinate(e_vPos))*e_pfWeihts[0]);
			l_fTotalWeight+=e_pfWeihts[0];
		}
		if(e_pfWeihts[1]>MIN_VALID_WEIGHT)
		{
			l_vFinalPos += ((m_pAllBonesMatrixForSkinned[e_pcInflunceBoneIndex[1]].TransformCoordinate(e_vPos))*e_pfWeihts[1]);
			l_fTotalWeight+=e_pfWeihts[1];
		}
		if(e_pfWeihts[2]>MIN_VALID_WEIGHT)
		{
			l_vFinalPos += ((m_pAllBonesMatrixForSkinned[e_pcInflunceBoneIndex[2]].TransformCoordinate(e_vPos))*e_pfWeihts[2]);
			l_fTotalWeight+=e_pfWeihts[2];
		}
		if(e_pfWeihts[3]>MIN_VALID_WEIGHT)
		{
			l_vFinalPos += ((m_pAllBonesMatrixForSkinned[e_pcInflunceBoneIndex[3]].TransformCoordinate(e_vPos))*e_pfWeihts[3]);
			l_fTotalWeight+=e_pfWeihts[3];
		}
	}
	return l_vFinalPos;
}
//========================================================
//
//========================================================
void	cAnimationMesh::Render(WCHAR*e_strShaderName)
{
	cBaseShader*l_p2DShader = GetCurrentShader();
	UseShaderProgram(e_strShaderName);
#ifdef DEBUG
	bool	l_bTextureExist = false;
	for(UINT i=0;i<this->m_ObjectList.size();++i)
	{
		if( (*this)[i]->GetTechniqueType() == eTL_DIFFUSE )
			l_bTextureExist = true;
	}
	if( !l_bTextureExist )
	{
		if(cGameApp::m_spImageParser)
		{
			cTexture*l_pSimpleGLTexture = dynamic_cast<cTexture*>(cGameApp::m_spImageParser->GetObject(0));
			if( l_pSimpleGLTexture )
				l_pSimpleGLTexture->ApplyImage();
		}
		else
		{
		    glBindTexture( GL_TEXTURE_2D, 0);
		    //m_suiLastUsingImageIndex = 0;
		}
	}
#endif
	for(UINT i=0;i<this->m_ObjectList.size();++i)
	{
		if( (*this)[i]->GetTechniqueType() == eTL_DIFFUSE )
			(*this)[i]->ApplyImage();
	}
	SetupShaderWorldMatrix(this->GetWorldTransform()*m_smatAxisTransform);
	int	l_iBoneSize = this->m_SkinningBoneVector.Count();
	SetupShaderBonesData(m_pAllBonesMatrixForSkinned,l_iBoneSize);
	if( !m_pVBOBuffer )
	{
#ifndef OPENGLES_2_X
		Vector3	*l_pvPos = 0;
		l_pvPos = (Vector3*)this->m_ppfVerticesBuffer[FVF_POS];
		float*	l_fWeight = this->m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT];
		char*	l_pcInflunceJoints = (char*)m_ppfVerticesBuffer[FVF_SKINNING_BONE_INDEX];//endusr the size as expected
		cMatrix44	l_mat;
		for( UINT i=0;i<m_uiVertexBufferCount;++i )
		{
			Vector3	l_vPos = GetSkinWorldPosBySkinningData(
										l_pvPos[i],
										&l_fWeight[i*MAX_RELEATED_BONE_COUNT],
										&l_pcInflunceJoints[i*MAX_RELEATED_BONE_COUNT]);
			m_pvVertexBuferForSkinned[i*3] = l_vPos.x;
			m_pvVertexBuferForSkinned[i*3+1] = l_vPos.y;
			m_pvVertexBuferForSkinned[i*3+2] = l_vPos.z;
		}
		myGlVertexPointer(3,m_pvVertexBuferForSkinned);
		if( m_ppfVerticesBuffer[FVF_TEX0] )
			myGlUVPointer(2,m_ppfVerticesBuffer[FVF_TEX0]);
		if( m_ppfVerticesBuffer[FVF_NORMAL] )
			myGlNormalPointer(3,m_ppfVerticesBuffer[FVF_NORMAL]);
#else
		for( int i=0;i<TOTAL_FVF;++i )
		{
			if( this->m_bVerticesBuffer[i] && g_uiAttribArray[i] != -1 )
			{
				glVertexAttribPointer(g_uiAttribArray[i],g_iFVF_DataStride[i], g_iFVF_DataType[i],0, 0, m_ppfVerticesBuffer[i]);
			}
		}
#endif
		MY_GLDRAW_ELEMENTS(GL_TRIANGLES,m_uiIndexBufferCount, g_iDrawindiceType,m_puiIndexBuffer );
	}
	else//for shader
	{
		this->m_pVBOBuffer->Render(false);
	}
	UseShaderProgram(l_p2DShader);
}

void	cAnimationMesh::DebugRender()
{
	//cAnimationMesh::Render();
	RenderSkeleton();
}

void	cAnimationMesh::RenderSkeleton()
{
	int	l_iNumBone = 0;
	int	l_iBoneSize = this->m_SkinningBoneVector.Count();
	cBone**l_ppBone = &(*m_SkinningBoneVector.GetList())[0];
	Vector3	l_vAllVertices[512];
	cMatrix44	l_mat = cMatrix44::Identity;
	for(int i=1;i<l_iBoneSize;++i)
	{
		cBone*l_pMe = l_ppBone[i];
		cBone*l_pParent = (cBone*)l_pMe->GetParent();
		cMatrix44	l_mat = cMatrix44::Identity;
		if( l_pParent )
		{
			//l_mat = (this->m_matBindPose*l_pMe->GetWorldTransform());
			l_mat = (l_pParent->GetWorldTransform())*this->m_matMeshBindShapePose;
			//parent
			l_vAllVertices[l_iNumBone*2+1] = l_mat.GetTranslation();
			//me
			l_mat = (l_pMe->GetWorldTransform())*this->m_matMeshBindShapePose;
			l_vAllVertices[l_iNumBone*2] = l_mat.GetTranslation();
			++l_iNumBone;
		}
	}
	l_mat = this->GetWorldTransform()*cMatrix44::ZupToYUp;
	GLRender::RenderLine((float*)&l_vAllVertices[0],l_iBoneSize*2,Vector4(0.f,1.f,0.5f,1.f),3,l_mat);
	GLRender::RenderPoints(l_vAllVertices,l_iBoneSize*2,15,Vector4(0.f,1.f,1.f,1.f),l_mat);
}

void cAnimationMesh::RenderSkeletonName(float*e_pfProjection,float*e_pfMatMV,int*e_piViewport)
{
	int l_iBoneSize = this->m_SkinningBoneVector.Count();
	cBone**l_ppBone = &(*m_SkinningBoneVector.GetList())[0];
	//cGameApp::m_spGlyphFontRender->SetScale(0.01f);
	cMatrix44	l_Root = this->GetWorldTransform()*cMatrix44::ZupToYUp;
	for(int i=1;i<l_iBoneSize;++i)
	{
		cBone*l_pMe = l_ppBone[i];
		const WCHAR*l_strName = l_pMe->GetName();
		Vector3 l_vPos = (l_Root*l_pMe->GetWorldTransform()*this->m_matMeshBindShapePose).GetTranslation();
		Vector3 l_vPos2 = UT::WorldToScreen(l_vPos.x,l_vPos.y,l_vPos.z,e_pfProjection,e_pfMatMV,e_piViewport);
		if( cGameApp::m_spGlyphFontRender )
		{
			cGameApp::m_spGlyphFontRender->RenderFont(l_vPos2.x,l_vPos2.y,l_strName);
		}
	}
	//cGameApp::m_spGlyphFontRender->SetScale(1.f);
}