#pragma once
#include "gltfMesh.h"
#include <map>
#include <string>

struct sSRT
{
    Vector3             vScale = Vector3::One;
    Quaternion          qRotation = Quaternion::Identity;
    Vector3             vTranslation = Vector3::Zero;
    int                 iSRTFlag = 0;//S 1<<1 R 1<< 2 T 1<<3
    cMatrix44           GetMatrix();
    static sSRT         Blend(sSRT& e_Source1, sSRT& e_Source2,float e_fFactor);
};
#define SRT_SCALE_FLAG          (1<<1)
#define SRT_ROTATION_FLAG       (1<<2)
#define SRT_TRANSLATION_FLAG    (1<<3)

using FloatToSRTMap = std::map<float, sSRT>;
typedef std::map<float, cMatrix44> FloatTocMatrix44Map;
void    ConvertSRTMapToMatrixMap(const FloatToSRTMap& srtMap, FloatTocMatrix44Map& matrixMap);

struct cBone : public Frame
{
    int         m_iJointIndex = -1;
    int         m_iNodeIndex;
    cMatrix44   m_matInvBindPose;
    //for non skinning animation
    std::string m_strTargetMeshName;

    cBone(const wchar_t* e_strName,int e_iJointIndex);
    ~cBone();
    cBone* FinChildByName(const wchar_t* e_strBoneName);
    void ApplySRT(const sSRT& srt, bool e_bSetChildBonesDirty);
    cMatrix44   m_StartNodeWorldTransform;
    //rest pose
    cMatrix44   m_StartNodeTransform;
    sSRT        m_StartSRT;
};

struct sAnimationData
{
    std::map<cBone*, FloatToSRTMap> m_BoneIDAndAnimationData;
    //it should ony taget to bone where has mesh name
    cBone*  m_pNotSkinningMeshBone = nullptr;
    bool    m_bLoop;
    float   m_fMinKeyTime;
    float   m_fMaxKeyTime;
    float   m_fCurrentTime;
    float   m_fStartTime;
    float   m_fEndTime;
    void    Update(float e_fElpaseTime);
};

class cAnimationClip:public NamedTypedObject
{
    friend class cAnimationMesh;
    class cAnimationMesh*                   m_pAnimationMesh;
    //
    std::vector<sSRT>                       m_SRTVector;
    std::string                             m_strAnimationName;
    sAnimationData*                         m_pCurrentAnimationData = nullptr;
	bool                                    SampleToTime(float e_fTime, bool e_bAssignToBone,std::vector<sSRT>*e_pSRTVector = nullptr);
    void	                                UpdateNode(cBone* e_pBone, float e_fTime,sSRT&e_SRT, bool e_bAssignToBone);
    float                                   m_fBlendingTime = 0;
public:
    void	SetBoneAndAnimationData(class cAnimationMesh* e_pAnimationMesh);
    bool    SetAnimation(const char* e_strAnimationName,bool e_bLoop,float e_fTargetTime = 0.f);
	void    UpdateToTargetTime(float e_fTime,bool e_bAssignToBone);
    void    BlendClips(float e_fTime,const char*e_strAnimationName1, const char* e_strAnimationName2, bool e_bAssignToBone, bool e_bLoop = true, float e_fTargetFactor = 0.5);
    void    Update(float e_fElpaseTime);
};