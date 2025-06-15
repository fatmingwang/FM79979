#pragma once

#include "glTFNode.h"
#include "Common.h"
class cglTFNodeData;
struct sAnimationData:public NamedTypedObject
{
    sAnimationData()
    {
		m_BoneIDAndAnimationData = std::make_shared<std::map<int, FloatToSRTMap>>();
    }
    std::shared_ptr<std::map<int, FloatToSRTMap>>                m_BoneIDAndAnimationData;
    cNamedTypedObjectVector<cglTFNodeData>*     m_pBoneVector;
    //morphing
    //here only contain weights because position and others are store in SubMesh
    //here only apply weight and sub mesh do it's own morpthing.
    std::map<float, std::vector<float>>         m_TimaAndMorphWeightMap;
    class cMesh*                                m_pTargetMesh = nullptr;
    int                                         m_iTargetNodeIndex = -1;
    bool    m_bLoop;
    float   m_fMinKeyTime;
    float   m_fMaxKeyTime;
    float   m_fCurrentTime;
    float   m_fStartTime;
    float   m_fEndTime;
    void    Update(float e_fElpaseTime);
    std::vector<float> GetInterpolatedWeights(float e_fTime);
    static sAnimationData* CloneFromModel(class cglTFModelRenderNode* e_pglTFModelRenderNode, sAnimationData*e_pSource);
};


class cAnimationClip :public NamedTypedObject
{
    friend class cglTFModel;
    friend class cglTFModelRenderNode;
    //from model
    std::map<int, cglTFNodeData*>*          m_pNodeIndexAndBoneMap;
    std::map<std::string, sAnimationData*>* m_pNameAndAnimationMap;
    cNamedTypedObjectVector<cglTFNodeData>* m_pNodesVector;
    std::vector<sSRT>                       m_SRTVector;
    std::string                             m_strAnimationName;
    sAnimationData*                         m_pCurrentAnimationData = nullptr;
    float                                   m_fBlendingTime = 0;
    //class cglTFModel*                       m_pglTFModel = nullptr;
    bool                                    SampleToTime(float e_fTime, bool e_bAssignToBone, std::vector<sSRT>* e_pSRTVector = nullptr);
    void	                                UpdateNode(cglTFNodeData* e_pBone, float e_fTime, sSRT& e_SRT, bool e_bAssignToBone);
    friend class cAnimTexture;
public:
    void	SetBoneAndAnimationData(class cglTFModel* e_pglTFModel);
    void	SetBoneAndAnimationData(class cglTFModelRenderNode* e_pglTFModel);
    bool    SetAnimation(const char* e_strAnimationName, bool e_bLoop, float e_fTargetTime = 0.f);
    void    UpdateToTargetTime(float e_fTime, bool e_bAssignToBone);
    void    BlendClips(float e_fTime, const char* e_strAnimationName1, const char* e_strAnimationName2, bool e_bAssignToBone, bool e_bLoop = true, float e_fTargetFactor = 0.5);
    void    Update(float e_fElpaseTime);
    sAnimationData* GetCurrentAnimationData()
    {
        return m_pCurrentAnimationData;
    }
};

class cAnimTexture :public NamedTypedObject
{
protected:
    float* mData;
    unsigned int mSize;
    unsigned int mHandle;
    public:
    cAnimTexture(cAnimationClip&e_AnimationClip, const char* e_strAnimationName);
    cAnimTexture(const cAnimTexture&);
    cAnimTexture& operator=(const cAnimTexture&);
    cAnimTexture(const char* path);
    cAnimTexture(unsigned int size);
    ~cAnimTexture();

    bool Load(const char* path);
    bool Save(const char* path);
    void UploadTextureDataToGPU();

    unsigned int Size();
    //make it square
    void Resize(unsigned int e_uiWidthAndHeight);
    float* GetData();

    void Set(unsigned int uniformIndex, unsigned int textureIndex);
    void UnSet(unsigned int textureIndex);
    unsigned int GetHandle();
    void        SetTexel(unsigned int x, unsigned int y, const Vector4& e_Value);
    void        SetTexel(unsigned int x, unsigned int y, const Vector3& e_Value);
};