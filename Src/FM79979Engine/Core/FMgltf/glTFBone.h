#pragma once
#include "gltfMesh.h"
#include <map>
#include <string>

struct sSRT
{
    Vector3             vScale = Vector3::One;
    Quaternion          qRotation = Quaternion::Zero;
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

struct cBone : public Frame
{
    void ConvertSRTMapToMatrixMap(const FloatToSRTMap& srtMap, FloatTocMatrix44Map& matrixMap);
    //std::pair <float,SRT>     m_PreviousSRT;
    //for debug
    int     m_iJointIndex = -1;
    int     m_iNodeIndex;
    float m_fMinKeyTime;
    float m_fMaxKeyTime;
    FloatToSRTMap               m_FormKeyFrames;
    FloatTocMatrix44Map         m_MatrixKeyFrames;
    cMatrix44 m_matInvBindPose;
    std::wstring m_strSID;

    cBone(const WCHAR* e_strName,int e_iJointIndex);
    ~cBone();
    cBone* FinChildByName(const wchar_t* e_strBoneName);
    void SetFormKeyFrames(FloatToSRTMap e_FormKeyFrames);
    void SetFormKeyFrames(FloatTocMatrix44Map e_FormKeyFrames);
    void EvaluateLocalXForm(float e_fTime, bool e_bSetChildBonesDirty = true);
    void EvaluateLocalXForm2(float e_fTime, bool e_bSetChildBonesDirty = true);
    void ApplySRT(const sSRT& srt, bool e_bSetChildBonesDirty);
    cMatrix44   m_StartNodeWorldTransform;
    //rest pose
    cMatrix44   m_StartNodeTransform;
    sSRT         m_StartSRT;
};

struct sAnimationData
{
    std::map<cBone*, FloatToSRTMap> m_BoneIDAndAnimationData;
    float m_fMinKeyTime;
    float m_fMaxKeyTime;
    float m_fCurrentTime;
    float m_fStartTime;
    float m_fEndTime;
};

class cAnimationClip:public NamedTypedObject
{
    //must same as joints influnces order,same as std::vector<cBone*>   cAnimationMesh::m_SkinningBoneVector;
	std::vector<cBone*>*        m_pOrderedBonesVector;
    std::vector<FloatToSRTMap*> m_OrderedAnimationVector;
    std::vector<sSRT>            m_SRTVector;
	void    SampleToTime(float e_fTime, bool e_bAssignToBone);
public:
	float   m_fCurrentTime;
	float   m_fEndTime;
	bool    m_bLoop;
    void	SetBoneAndAnimationData(std::vector<cBone*>* e_pBoneVector, sAnimationData*e_pAnimationData);
	void    UpdateToTargetTime(float e_fTime,bool e_bAssignToBone);
    void    BlendClips(float e_fTime,cAnimationClip*e_pTarget, bool e_bAssignToBone, float e_fTargetFactor = 0.5);
};