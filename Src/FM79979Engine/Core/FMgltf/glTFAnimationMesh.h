#pragma once

#include "gltfMesh.h"
#include "gltfNode.h"
#include <map>
#include <string>

class cSkinningMesh : public cMesh
{
    friend class cAnimationClip;
    cglTFNodeData*                          m_pMainRootBone;
    std::vector<cglTFNodeData*>             m_SkinningBoneVector;
    std::vector<cMatrix44>                  m_NodeInversePoseMatrixVector;
    std::vector<int>                        m_JointOrderVector;
    cMatrix44                               m_matMeshBindShapePose;
    cMatrix44*                              m_pAllBonesMatrixForSkinned;
    void    UpdateJointsMatrix();
public:
    cSkinningMesh();
    virtual ~cSkinningMesh();
    void    LoadAnimations(const tinygltf::Skin&e_Skin,cglTFModel*e_pModel, tinygltf::Model&e_Model);
    void    RefreshAnimationData();
    void    Update(float elapsedTime) override;
    void    Render() override;
    void    JointUpdate(float elapsedTime);
    void    RenderSkeleton();
};
