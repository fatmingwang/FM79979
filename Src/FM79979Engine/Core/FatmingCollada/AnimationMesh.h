#pragma once

#include "Mesh.h"

typedef std::map<float, cMatrix44> FloatTocMatrix44Map;

class cBone:public Frame
{
	//std::string m_strSubId;
	GET_SET_DEC(float,m_fMinKeyTime,GetMinKeyTime,SetMinKeyTime);
	GET_SET_DEC(float,m_fMaxKeyTime,GetMaxKeyTime,SetMaxKeyTime);
	FloatTocMatrix44Map	m_FormKeyFrames;
	//for quickly to get world transofmr
	//jointTransformation = jointNode->CalculateWorldTransform() * jointBindPoseInverse * skinBindShapeMatrix;
	//jointTransformation = final transform
	GET_SET_DEC(cMatrix44,m_matInvBindPose,GetInvBindPose,SetInvBindPose);//for original bone pos in the skeleton
	//WCHAR	m_strSidName[MAX_PATH];
public:
	//cBone(const WCHAR*e_strName,const WCHAR*e_strSidName);
	cBone(const WCHAR*e_strName);
	cBone(cBone*e_pBone);
	~cBone();
	cBone*	FinChildByName(WCHAR*e_strBoneName);
	void	SetFormKeyFrames(FloatTocMatrix44Map	e_FormKeyFrames);
	//WCHAR*	GetSid(){ return m_strSidName; }
	void	EvaluateLocalXForm(float e_fTime,bool e_bSetChildBonesDirty = true);
	bool	m_bAnimation;
//#ifdef DEBUG
	std::wstring	m_strSID;
//#endif
};
//=================
//ensure the Bones have optmize order so we could quickly to do animation while rendering
//ProcessController() has done the bone order,so bind pose matriex has set
//=================
class	cAnimationMesh:public cMesh
{
	Vector3	GetSkinWorldPosBySkinningData(Vector3 e_vPos,float*e_pfWeihts,char*e_pcInflunceBoneIndex);
	GET_SET_DEC(cBone*,m_pMainRootBone,GetMainRootBone,SetMainRootBone);//the toppest root bone
	//from collada scene node,store bones here to get correct world transform.
	GETP_SET_DEC(cNamedTypedObjectVector<cBone>,m_AllBoneVector,GetAllBoneVector,SetAllBoneVector);
	//the bone for skinning,do not delete its vector data because its come from m_AllBoneVector
	GETP_SET_DEC(cNamedTypedObjectVector<cBone>,m_SkinningBoneVector,GetSkinningBoneVector,SetSkinningBoneVector);
//animation time
	GET_SET_DEC(float,m_fMinKeyTime,GetMinKeyTime,SetMinKeyTime);
	GET_SET_DEC(float,m_fMaxKeyTime,GetMaxKeyTime,SetMaxKeyTime);
	float	m_fCurrentTime;
	GET_SET_DEC(float,m_fStartTime,GetStartTime,SetStartTime);
	GET_SET_DEC(float,m_fEndTime,GetEndTime,SetEndTime);
	// the last time value at which all nodes where updated
    GET_SET_DEC(float,m_fLastNodesUpdateTime,GetLastNodesUpdateTime,SetLastNodesUpdateTime);
	void	UpdateNodes(float e_fTimeValue);
	void	UpdateNode(cBone*e_pBone,float e_fTime);
	cMatrix44*	m_pAllBonesMatrixForSkinned;//while the bones are skinned,store the matrix in here
	float*	m_pvVertexBuferForSkinned;//while the bones are skinned,store the vertex data in here
	//shader
	//cGLSLProgram*m_pGLSLProgram;
	cMatrix44 m_matMeshBindShapePose;//start controller pose
public:
	DEFINE_TYPE_INFO()
	cAnimationMesh(cMesh*e_pMesh,cMatrix44 e_matMeshBindShapePose);
	cAnimationMesh(cAnimationMesh*e_pAnimationMesh);
	CLONE_MYSELF(cAnimationMesh);
	~cAnimationMesh();
	cMatrix44	GetBindShapePose(){ return m_matMeshBindShapePose; }
	void	SetBindShapePose(cMatrix44 e_mat);

	float	GetCurrentTime(){ return m_fCurrentTime; }
	//while bone and animation has been assigned,refresh time for skinning
	void	RefreshAnimationData();
	//for current time to add destination time
	virtual void	Update(float elapsedTime)override;
	void	JointUpdate(float elapsedTime);
	//to specification time
	void	SetCurrentAnimationTime(float e_fCurrentTime);
	virtual	void	Render(WCHAR*e_strShaderName = SKINNING_MESH_SHADER)override;
	virtual	void	RenderSkeleton();
	void			RenderSkeletonName(float*e_pfProjection,float*e_pfMatMV,int*e_piViewport);
	//for debug
	void	DebugRender();
	bool	m_bOnlyInflunceJoint;
	int		m_iOnlyInflunceJoint;
	int		m_iStartJoint;
};


//uniform mat4 bonesmat[256];
//attribute vec4 bonesidx;
//attribute vec4 bonesweights;
//
//varying vec3 normal;
//varying vec3 viewvec;
//
//void main(void)
//{
//    gl_Position = vec4 (0, 0, 0, 0);
//    ivec4 ibonesidx = ivec4(bonesidx);
//    for (int i = 0; i < 4; i++)
//    {
//        gl_Position += bonesmat[ibonesidx[i]] * gl_Vertex * bonesweights[i];
//    }
//
//    gl_Position = gl_ModelViewProjectionMatrix * gl_Position;
//
//    normal = gl_NormalMatrix * gl_Normal;
//    viewvec = gl_NormalMatrix * vec3 (gl_Vertex);
//}