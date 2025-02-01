#pragma once

#include "gltfMesh.h"

typedef std::map<float, cMatrix44> FloatTocMatrix44Map;
struct cBone :public Frame
{
	//std::string m_strSubId;
	float m_fMinKeyTime;
	float m_fMaxKeyTime;
	FloatTocMatrix44Map	m_FormKeyFrames;
	//for quickly to get world transofmr
	//jointTransformation = jointNode->CalculateWorldTransform() * jointBindPoseInverse * skinBindShapeMatrix;
	//jointTransformation = final transform
	cMatrix44 m_matInvBindPose;
	//WCHAR	m_strSidName[MAX_PATH];
	public:
	//cBone(const WCHAR*e_strName,const WCHAR*e_strSidName);
	cBone(const WCHAR* e_strName);
	cBone(cBone* e_pBone);
	~cBone();
	cBone* FinChildByName(const wchar_t* e_strBoneName);
	void	SetFormKeyFrames(FloatTocMatrix44Map	e_FormKeyFrames);
	//WCHAR*	GetSid(){ return m_strSidName; }
	void	EvaluateLocalXForm(float e_fTime, bool e_bSetChildBonesDirty = true);
	bool	m_bAnimation;
	std::wstring	m_strSID;
};

class cAnimationMeshFromglTF : public cMesh
{
	cBone* m_pMainRootBone;
	//from collada scene node,store bones here to get correct world transform.
	cNamedTypedObjectVector<cBone> m_AllBoneVector;
	//the bone for skinning,do not delete its vector data because its come from m_AllBoneVector
	cNamedTypedObjectVector<cBone> m_SkinningBoneVector;
	//animation time
	float	m_fMinKeyTime;
	float	m_fMaxKeyTime;
	float	m_fCurrentTime;
	float	m_fStartTime;
	float	m_fEndTime;

	cMatrix44 m_matMeshBindShapePose;//start controller pose
	cMatrix44* m_pAllBonesMatrixForSkinned;//while the bones are skinned,store the matrix in here
	void	UpdateNodes(float e_fTimeValue);
	void	UpdateNode(cBone* e_pBone, float e_fTime);
public:
	cAnimationMeshFromglTF();
    ~cAnimationMeshFromglTF();

    void LoadAnimation(const tinygltf::Model& model, const tinygltf::Animation& animation);
    void UpdateAnimation(float deltaTime);
	//while bone and animation has been assigned,refresh time for skinning
	void	RefreshAnimationData();
	//for current time to add destination time
	virtual void	Update(float elapsedTime)override;
	void	JointUpdate(float elapsedTime);
	//to specification time
	void	SetCurrentAnimationTime(float e_fCurrentTime);
	virtual	void	RenderSkeleton();
	void			RenderSkeletonName(float* e_pfProjection, float* e_pfMatMV, int* e_piViewport);
	//for debug
	void	DebugRender();

private:
};