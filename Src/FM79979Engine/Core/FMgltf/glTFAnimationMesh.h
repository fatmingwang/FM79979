#pragma once

#include "gltfMesh.h"
#include "gltfBone.h"
#include <map>
#include <string>

class cAnimationMesh : public cMesh
{
    cBone* m_pMainRootBone;
    cNamedTypedObjectVector<cBone> m_SkinningBoneVector;
    cMatrix44 m_matMeshBindShapePose;
    cMatrix44* m_pAllBonesMatrixForSkinned;
    struct sAnimationData
    {
        std::map<int, FloatTocMatrix44Map> m_BoneIDAndAnimationData;
        float m_fMinKeyTime;
        float m_fMaxKeyTime;
        float m_fCurrentTime;
        float m_fStartTime;
        float m_fEndTime;
    };
    std::map<std::string, sAnimationData> m_NameAndAnimationMap; // Animation name -> Node index -> Keyframes
    sAnimationData* m_pCurrentAnimationData;
    std::string m_CurrentAnimation;

    void UpdateNodes(float e_fTimeValue);
    void UpdateNode(cBone* e_pBone, float e_fTime);

    public:
    cAnimationMesh();
    virtual ~cAnimationMesh();

    void LoadAnimations(const tinygltf::Model& model);
    void SetCurrentAnimation(const std::string& animationName);
    void UpdateAnimation(float deltaTime);
    void RefreshAnimationData();
    virtual void Update(float elapsedTime) override;
    virtual void Draw() override;
    void JointUpdate(float elapsedTime);
    void SetCurrentAnimationTime(float e_fCurrentTime);
    //virtual void RenderSkeleton();
};
