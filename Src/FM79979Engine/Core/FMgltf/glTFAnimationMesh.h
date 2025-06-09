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
    std::shared_ptr<std::vector<cMatrix44>> m_pNodeInversePoseMatrixVector;
    std::shared_ptr<std::vector<int>>       m_JointOrderVector;
    //cMatrix44                               m_matMeshBindShapePose;
    std::vector<cMatrix44>                  m_AllBonesMatrixForSkinnedVector;
    void                                    UpdateJointsMatrix();
    virtual void					        SetSubMeshCommonUniformData(sSubMesh* e_pMesh, cMatrix44& e_mat)override;
public:
    DEFINE_TYPE_INFO();
    cSkinningMesh();
    cSkinningMesh(cSkinningMesh*e_pSkinningMesh);
    virtual ~cSkinningMesh();
    LAZY_CLONE_FUNCTION(cMesh);
    void    LoadJointsData(const tinygltf::Skin&e_Skin,cglTFModel*e_pModel,const  tinygltf::Model&e_Model);
    void    RefreshAnimationData();
    void    Update(float elapsedTime) override;
    void    Render() override;
    void    JointUpdate(float elapsedTime);
    void    RenderSkeleton();
};
