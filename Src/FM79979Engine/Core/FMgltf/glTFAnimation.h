#pragma once

#include "glTFNode.h"
#include "Common.h"
class cglTFNodeData;
struct sAnimationData:public NamedTypedObject
{
    std::map<int, FloatToSRTMap>                m_BoneIDAndAnimationData;
    cNamedTypedObjectVector<cglTFNodeData>*     m_pBoneVector;
    //std::vector<cglTFNodeData>              m_NodeVectorFromAnimationData;
    //morphing
    //here only contain weights because position and others are store in SubMesh
    //here only apply weight and sub mesh do it's own morpthing.
    std::map<float, std::vector<float>>         m_TimaAndMorphWeightMap;
    class cMesh*                                m_pTargetMesh;
    int                                         m_iTargetNodeIndex = -1;
    bool    m_bLoop;
    float   m_fMinKeyTime;
    float   m_fMaxKeyTime;
    float   m_fCurrentTime;
    float   m_fStartTime;
    float   m_fEndTime;
    void    Update(float e_fElpaseTime);
    std::vector<float> GetInterpolatedWeights(float e_fTime);
};


class cAnimationClip :public NamedTypedObject
{
    friend class cglTFModel;
    //
    std::vector<sSRT>                       m_SRTVector;
    std::string                             m_strAnimationName;
    sAnimationData*                         m_pCurrentAnimationData = nullptr;
    float                                   m_fBlendingTime = 0;
    class cglTFModel*                       m_pglTFModel = nullptr;
    bool                                    SampleToTime(float e_fTime, bool e_bAssignToBone, std::vector<sSRT>* e_pSRTVector = nullptr);
    void	                                UpdateNode(cglTFNodeData* e_pBone, float e_fTime, sSRT& e_SRT, bool e_bAssignToBone);
public:
    void	SetBoneAndAnimationData(class cglTFModel* e_pglTFModel);
    bool    SetAnimation(const char* e_strAnimationName, bool e_bLoop, float e_fTargetTime = 0.f);
    void    UpdateToTargetTime(float e_fTime, bool e_bAssignToBone);
    void    BlendClips(float e_fTime, const char* e_strAnimationName1, const char* e_strAnimationName2, bool e_bAssignToBone, bool e_bLoop = true, float e_fTargetFactor = 0.5);
    void    Update(float e_fElpaseTime);
};
