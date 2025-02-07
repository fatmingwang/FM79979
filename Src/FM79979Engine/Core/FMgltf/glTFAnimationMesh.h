#pragma once

#include "gltfMesh.h"
#include "gltfBone.h"
#include <map>
#include <string>

class cAnimationMesh : public cMesh
{
    cBone* FindBoneByIndex(int e_iIndex);
    cBone* m_pMainRootBone;
    cNamedTypedObjectVector<cBone> m_SkinningBoneVector;
    std::vector<int>                m_JointOrderVector;
    cMatrix44 m_matMeshBindShapePose;
    cMatrix44* m_pAllBonesMatrixForSkinned;
    struct sAnimationData
    {
        std::map<cBone*, FloatToSRTMap> m_BoneIDAndAnimationData;
        float m_fMinKeyTime;
        float m_fMaxKeyTime;
        float m_fCurrentTime;
        float m_fStartTime;
        float m_fEndTime;
    };
    std::map<std::string, sAnimationData*> m_NameAndAnimationMap;
    sAnimationData* m_pCurrentAnimationData;
    std::string m_CurrentAnimation;

    void UpdateNodes(float e_fTimeValue);
    void UpdateNode(cBone* e_pBone, float e_fTime);

    public:
    cAnimationMesh();
    virtual ~cAnimationMesh();
    void    loadSkins(const tinygltf::Model& model, std::map<int, cBone*>& nodeToBoneMap);
    void    loadAnimations(const tinygltf::Model& model, std::map<int, cBone*>& nodeToBoneMap);
    void    loadNode(const tinygltf::Node& node, const tinygltf::Model& model, cBone* parentBone, std::map<const tinygltf::Node*, cBone*> e_tinyglTFNodeAndJointIndexMap);

    void    LoadAnimations(const tinygltf::Model& model);
    void    SetCurrentAnimation(const std::string& animationName);
    void    SetCurrentAnimationTime(float e_fCurrentTime);
    void    UpdateAnimation(float deltaTime);
    void    RefreshAnimationData();
    void    Update(float elapsedTime) override;
    void    Draw() override;
    void    JointUpdate(float elapsedTime);
    void    RenderSkeleton();
};
