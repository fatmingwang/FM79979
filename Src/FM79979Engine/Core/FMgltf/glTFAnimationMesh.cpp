#include "glTFModel.h"
#include "glTFAnimationMesh.h"
#include <set>
#include <unordered_set>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <cfloat>
#include <iostream>
#include <sstream>
#include "glTFLight.h"
#include "glTFCamera.h"
bool g_bApplyInverseBindPose = true;
TYPDE_DEFINE_MARCO(cSkinningMesh);
cSkinningMesh::cSkinningMesh()
    : m_pMainRootBone(nullptr)
{
	m_pNodeInversePoseMatrixVector = std::make_shared<std::vector<cMatrix44>>();
    m_JointOrderVector = std::shared_ptr<std::vector<int>>();
}

cSkinningMesh::cSkinningMesh(cSkinningMesh* e_pSkinningMesh) :cMesh(e_pSkinningMesh)
{
    m_pNodeInversePoseMatrixVector = e_pSkinningMesh->m_pNodeInversePoseMatrixVector;
    m_JointOrderVector = e_pSkinningMesh->m_JointOrderVector;

    m_SkinningBoneVector = e_pSkinningMesh->m_SkinningBoneVector;
    m_pMainRootBone = e_pSkinningMesh->m_pMainRootBone;
    //m_matMeshBindShapePose = e_pSkinningMesh->m_matMeshBindShapePose;
    m_AllBonesMatrixForSkinnedVector = e_pSkinningMesh->m_AllBonesMatrixForSkinnedVector;
}

cSkinningMesh::~cSkinningMesh()
{
}


void	DumpBoneIndexDebugInfo(cglTFNodeData* e_pBone, bool e_bDoNextSibling, bool e_bRoot)
{
    Frame* l_pParentNode = e_pBone->GetParent();
    static int	l_siCount = 0;
    ++l_siCount;
    std::string l_strDebugInfo;
    while (l_pParentNode)
    {
        l_strDebugInfo += "-----";
        //FMLog::LogWithFlag(L"-----",false,false);
        l_pParentNode = l_pParentNode->GetParent();
    }
    l_strDebugInfo += "Joint:";
    l_strDebugInfo += ValueToString(e_pBone->m_iJointIndex);
    l_strDebugInfo += ",";
    l_strDebugInfo += "Node:";
    l_strDebugInfo += ValueToString(e_pBone->m_iNodeIndex);
    l_strDebugInfo += ",Name:";
    l_strDebugInfo += e_pBone->GetCharName();
    //FMLog::LogWithFlag(l_str, CORE_LOG_FLAG);
    FMLog::Log(l_strDebugInfo.c_str(), false);
    if (e_pBone->GetFirstChild())
    {
        DumpBoneIndexDebugInfo(dynamic_cast<cglTFNodeData*>(e_pBone->GetFirstChild()), true, false);
    }

    if (e_bDoNextSibling && e_pBone->GetNextSibling())
    {
        DumpBoneIndexDebugInfo(dynamic_cast<cglTFNodeData*>(e_pBone->GetNextSibling()), e_bDoNextSibling, false);
    }
    if (e_bRoot)
    {
        FMLOG("Total count:%d", l_siCount);
        l_siCount = 0;
    }
}

void cSkinningMesh::LoadJointsData(const tinygltf::Skin& e_Skin, cglTFModel* e_pModel, const tinygltf::Model& e_Model)
{
    if (e_Skin.skeleton != -1)
    {
        m_pMainRootBone = e_pModel->m_NodesVector[e_Skin.skeleton];
    }
    else
    {
        m_pMainRootBone = e_pModel->m_NodesVector[0];
    }
    m_JointOrderVector = std::make_shared<std::vector<int>>(e_Skin.joints);
    auto l_uiJoinsSize = e_Skin.joints.size();
    if (e_Skin.inverseBindMatrices > -1)
    {
        const auto& accessor = e_Model.accessors[e_Skin.inverseBindMatrices];
        const auto& bufferView = e_Model.bufferViews[accessor.bufferView];
        const auto& buffer = e_Model.buffers[bufferView.buffer];
        const float* data = reinterpret_cast<const float*>(buffer.data.data() + (bufferView.byteOffset + accessor.byteOffset));
        //const float* matrixData = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
        assert(accessor.count == l_uiJoinsSize && "inverse matrix not match");
        m_pNodeInversePoseMatrixVector->resize(l_uiJoinsSize);
        for (size_t i = 0; i < l_uiJoinsSize; ++i)
        {
            int l_NodeIndex = e_Skin.joints[i];
            auto l_pNode = e_pModel->m_NodesVector[l_NodeIndex];
            m_SkinningBoneVector.push_back(l_pNode);
            assert(l_pNode->m_iNodeIndex == l_NodeIndex);
            if (l_pNode)
            {
                l_pNode->m_iJointIndex = (int)i;
                cMatrix44 l_InversePoseMatrix(data + i * 16);
				(*m_pNodeInversePoseMatrixVector)[i] = l_InversePoseMatrix;
                FMLOG("NodeIndex:%d\tJoint index:%d", l_NodeIndex,i);
            }
            else
            {
                FMLOG("Error no such joint:%d", i);
            }
        }
    }
    if (m_AllBonesMatrixForSkinnedVector.size() == 0)
    {
		m_AllBonesMatrixForSkinnedVector.resize(l_uiJoinsSize);
    }
    RefreshAnimationData();
    if (this->m_pMainRootBone)
    {
        DumpBoneIndexDebugInfo(this->m_pMainRootBone, false, true);
    }
}



void cSkinningMesh::UpdateJointsMatrix()
{
    int boneCount = (int)this->m_JointOrderVector->size();
    for (int i = 0; i < boneCount; ++i)
    {
        //int l_iBoneIndex = (*m_JointOrderVector)[i];
        cglTFNodeData* bone = m_SkinningBoneVector[i];
        int l_iBoneIndex = bone->m_iJointIndex;
        if (bone)
        {
            assert(i == bone->m_iJointIndex);
            assert((*m_JointOrderVector)[i] == bone->m_iNodeIndex);
            if (bone->m_iJointIndex == -1)
            {
                continue;
            }
            auto l_matWorldTransform = bone->GetWorldTransform();
            auto l_mat = l_matWorldTransform;
            if (g_bApplyInverseBindPose)
            {
                l_mat = l_matWorldTransform * (*m_pNodeInversePoseMatrixVector)[i];
            }
            m_AllBonesMatrixForSkinnedVector[i] = l_mat;
        }
        else
        {
            int a = 0;
        }
    }
}

void	cSkinningMesh::SetSubMeshCommonUniformData(sSubMesh* e_pSubMesh, cMatrix44& e_mat)
{
	cMesh::SetSubMeshCommonUniformData(e_pSubMesh, e_mat);
    // Pass the bone matrices to the shader
    GLuint boneMatricesLocation = glGetUniformLocation(e_pSubMesh->m_iShaderProgramID, "uBoneTransforms");
    glUniformMatrix4fv(boneMatricesLocation, (GLsizei)m_AllBonesMatrixForSkinnedVector.size(), GL_FALSE, (float*)&m_AllBonesMatrixForSkinnedVector[0]);
}

void cSkinningMesh::RefreshAnimationData()
{
    int boneCount = (int)this->m_JointOrderVector->size();
    for (int i = 0; i < boneCount; ++i)
    {
        auto l_pBone = m_SkinningBoneVector[i];
        m_AllBonesMatrixForSkinnedVector[i] = l_pBone->m_StartNodeWorldTransform;
    }
}

void cSkinningMesh::Update(float e_fElpaseTime)
{
    UpdateJointsMatrix();
}

void cSkinningMesh::Render()
{
    if (m_SkinningBoneVector.size() == 0)
    {
        cMesh::Render();
        return;
    }
    auto l_matWorldTransoform = this->GetWorldTransform();
    for (auto& l_pSubMesh : this->m_SubMeshesVector)
    {
        SetSubMeshCommonUniformData(l_pSubMesh.get(), l_matWorldTransoform);
		cMesh::CallOpenGLDraw(l_pSubMesh.get());
    }
}

void  cSkinningMesh::Render(std::shared_ptr<cAnimationInstanceManager>e_spAnimationInstanceManager, std::shared_ptr<sAniamationInstanceData>e_AniamationInstanceData)
{
    auto l_pMeshInstance = e_spAnimationInstanceManager->GetMeshInstance();
    if (l_pMeshInstance)
    {
        auto l_matTransform = this->GetWorldTransform();
        size_t instanceCount = l_pMeshInstance->GetCount();
        assert(instanceCount > 0);
        if (l_pMeshInstance->GetInstanceVBO() == 0)
        {
            l_pMeshInstance->InitBuffer(m_SubMeshesVector);
        }

        if (l_pMeshInstance->IsBufferDirty())
        {
            l_pMeshInstance->UpdateBuffer();
        }
        for (auto& l_SubMesh : m_SubMeshesVector)
        {
            //SetSubMeshCommonUniformData(l_SubMesh.get(), l_matTransform);            
            cMesh::SetSubMeshCommonUniformData(l_SubMesh.get(), l_matTransform);
            e_spAnimationInstanceManager->Render(l_SubMesh->m_iShaderProgramID, e_AniamationInstanceData);
            glBindVertexArray(l_SubMesh->m_uiVAO);
            glDrawElementsInstanced(GL_TRIANGLES,
                                    static_cast<GLsizei>(l_SubMesh->m_IndexBuffer.size()),
                                    GL_UNSIGNED_INT, 0,
                                    static_cast<GLsizei>(instanceCount));
        }
    }
}


void cSkinningMesh::RenderSkeleton()
{
    std::vector<cglTFNodeData*>* l_pVector = nullptr;
    //l_pVector = m_AllNodeConvertToBoneBoneVector.GetList();
    l_pVector = &m_SkinningBoneVector;
    int	l_iNumBone = 0;
    int	l_iBoneSize = (int)l_pVector->size();
    std::vector<Vector3>l_vAllVertices;
    std::vector<Vector3>l_vPoints;
    cMatrix44	l_mat = cMatrix44::Identity;
    for (int i = 0; i < l_iBoneSize; ++i)
    {
        cglTFNodeData* l_pMe = (*l_pVector)[i];
        cglTFNodeData* l_pParent = dynamic_cast<cglTFNodeData*>(l_pMe->GetParent());
        if (l_pParent)
        {
            //parent
            l_mat = l_pParent->GetWorldTransform();
            if (g_bApplyInverseBindPose)
            {
                if (l_pParent->m_iJointIndex != -1)
                {
                    l_mat = l_pParent->GetWorldTransform() * (*m_pNodeInversePoseMatrixVector)[l_pParent->m_iJointIndex];
                }
            }
            l_vAllVertices.push_back(l_mat.GetTranslation());
            //me
            l_mat = (l_pMe->GetWorldTransform());
            if (g_bApplyInverseBindPose)
            {
                l_mat = (l_pMe->GetWorldTransform() * (*m_pNodeInversePoseMatrixVector)[l_pMe->m_iJointIndex]);
            }
            l_vAllVertices.push_back(l_mat.GetTranslation());
            ++l_iNumBone;
        }
        else
        {
            l_mat = (l_pMe->GetWorldTransform() * (*m_pNodeInversePoseMatrixVector)[l_pMe->m_iJointIndex]);
        }
        l_vPoints.push_back(l_mat.GetTranslation());
    }
    if (l_vPoints.size() == 0)
    {
        return;
    }
    l_mat = this->GetWorldTransform();
    UseShaderProgram(NO_TEXTURE_SHADER);
    auto l_pCamera = cCameraController::GetInstance()->GetCurrentCamera();
    if (l_pCamera)
    {
        auto l_matWVP = l_pCamera->GetWorldViewglTFProjection();
        SetupShaderViewProjectionMatrix(l_matWVP, false);
    }
    GLRender::RenderLine((float*)&l_vAllVertices[0], (int)l_vAllVertices.size(), Vector4(0.f, 1.f, 0.5f, 1.f), 3, l_mat);
    GLRender::RenderPoints(&l_vPoints[0], (int)l_vPoints.size(), 5, Vector4(0.f, 1.f, 1.f, 1.f), l_mat);
}

void cSkinningMesh::AfterCloneSetBoneData(cglTFModelRenderNode* e_pData)
{
    std::vector<cglTFNodeData*> l_SkinningBoneVectorFromClon = m_SkinningBoneVector;
    auto l_SkinningBoneVectorFromModel = *e_pData->m_NodesVector.GetList();
    m_SkinningBoneVector.clear();
    for (auto l_IT : l_SkinningBoneVectorFromClon)
    {
        m_SkinningBoneVector.push_back(l_SkinningBoneVectorFromModel[l_IT->m_iNodeIndex]);
    }
    if (m_pMainRootBone)
    {
        m_pMainRootBone = e_pData->m_NodesVector[m_pMainRootBone->m_iNodeIndex];
    }
}

void		cSkinningMesh::DebugRender()
{
    this->RenderSkeleton();
}


cSkinningAnimInstanceClass::cSkinningAnimInstanceClass()
{
    this->SetName(L"cSkinningAnimTestClass");
}
cSkinningAnimInstanceClass::~cSkinningAnimInstanceClass()
{
    m_spAnimationInstanceManager = nullptr;
    m_spAniamationInstanceData = nullptr;
    m_pTargetMesh = nullptr;
}
void    cSkinningAnimInstanceClass::SetData(std::vector<std::shared_ptr<class cAnimationInstanceManager>>& e_Data, const char* e_strTargetAnimationName)
{
    if (e_Data.size())
    {
        m_spAnimationInstanceManager = e_Data[0];
        auto l_vPos = this->GetWorldPosition();
        std::tuple<std::shared_ptr<sAniamationInstanceData>, GLuint > l_TupleData = m_spAnimationInstanceManager->GetAnimationInstanceData(e_strTargetAnimationName);
        m_spAniamationInstanceData = std::get<0>(l_TupleData);
        m_uiProgramID = std::get<1>(l_TupleData);
        m_PositionVector = *m_spAnimationInstanceManager->GetMeshInstance()->GetTransforms();
        auto l_uiSize = m_spAniamationInstanceData->m_AnimationFrameAndTimeVector.size();
        for (auto i = 0; i < l_uiSize; ++i)
        {
            auto l_pCurrentData = m_spAniamationInstanceData->m_AnimationFrameAndTimeVector[i];
            l_pCurrentData->Update(frand(0, 3.f));
            m_spAniamationInstanceData->m_FrameIndexVector[i].iCurrent = l_pCurrentData->m_iCurrentFrame;
            m_spAniamationInstanceData->m_FrameIndexVector[i].iNext = l_pCurrentData->m_iNextFrame;
            m_spAniamationInstanceData->m_ToNextLerpTime[i] = l_pCurrentData->m_fNextFrameLerpTimes;
        }
        auto l_pTransformVector = m_spAnimationInstanceManager->GetMeshInstance()->GetTransforms();
        l_uiSize = l_pTransformVector->size();
        for (auto i = 0; i < l_uiSize; ++i)
        {
            auto l_vPos2 = m_PositionVector[i].GetTranslation();
            if (l_vPos.z != 0)
            {
                l_vPos2.z = frand(-5, -100);
                m_PositionVector[i].SetTranslation(l_vPos2);
            }
        }
    }

}
void cSkinningAnimInstanceClass::Update(float e_fElpaseTime)
{
    if (m_spAniamationInstanceData)
    {
        auto l_vPos = this->GetWorldPosition();
        auto l_uiSize = m_spAniamationInstanceData->m_AnimationFrameAndTimeVector.size();
        for (auto i = 0; i < l_uiSize; ++i)
        {
            auto l_pCurrentData = m_spAniamationInstanceData->m_AnimationFrameAndTimeVector[i];
            l_pCurrentData->Update(e_fElpaseTime);
            m_spAniamationInstanceData->m_FrameIndexVector[i].iCurrent = l_pCurrentData->m_iCurrentFrame;
            m_spAniamationInstanceData->m_FrameIndexVector[i].iNext = l_pCurrentData->m_iNextFrame;
            m_spAniamationInstanceData->m_ToNextLerpTime[i] = l_pCurrentData->m_fNextFrameLerpTimes;
        }
    }
}
void cSkinningAnimInstanceClass::Render()
{
    if (m_spAnimationInstanceManager)
    {
        auto l_spMeshInstance = m_spAnimationInstanceManager->GetMeshInstance();
        l_spMeshInstance->SetInstanceTransforms(m_PositionVector);
        auto l_pSkinningMesh = m_spAnimationInstanceManager->GetTaargetMesh();
        l_pSkinningMesh->Render(m_spAnimationInstanceManager, m_spAniamationInstanceData);
    }
}
//woman
//NodeIndex:40	Joint index : 0
//NodeIndex : 29	Joint index : 1
//NodeIndex : 28	Joint index : 2
//NodeIndex : 27	Joint index : 3
//NodeIndex : 2	Joint index : 4
//NodeIndex : 1	Joint index : 5
//NodeIndex : 0	Joint index : 6
//NodeIndex : 14	Joint index : 7
//NodeIndex : 13	Joint index : 8
//NodeIndex : 12	Joint index : 9
//NodeIndex : 11	Joint index : 10
//NodeIndex : 6	Joint index : 11
//NodeIndex : 5	Joint index : 12
//NodeIndex : 4	Joint index : 13
//NodeIndex : 3	Joint index : 14
//NodeIndex : 10	Joint index : 15
//NodeIndex : 9	Joint index : 16
//NodeIndex : 8	Joint index : 17
//NodeIndex : 7	Joint index : 18
//NodeIndex : 26	Joint index : 19
//NodeIndex : 25	Joint index : 20
//NodeIndex : 24	Joint index : 21
//NodeIndex : 23	Joint index : 22
//NodeIndex : 18	Joint index : 23
//NodeIndex : 17	Joint index : 24
//NodeIndex : 16	Joint index : 25
//NodeIndex : 15	Joint index : 26
//NodeIndex : 22	Joint index : 27
//NodeIndex : 21	Joint index : 28
//NodeIndex : 20	Joint index : 29
//NodeIndex : 19	Joint index : 30
//NodeIndex : 34	Joint index : 31
//NodeIndex : 33	Joint index : 32
//NodeIndex : 32	Joint index : 33
//NodeIndex : 31	Joint index : 34
//NodeIndex : 30	Joint index : 35
//NodeIndex : 39	Joint index : 36
//NodeIndex : 38	Joint index : 37
//NodeIndex : 37	Joint index : 38
//NodeIndex : 36	Joint index : 39
//NodeIndex : 35	Joint index : 40


//NodeIndex:2	Joint index : 0
//NodeIndex : 3	Joint index : 1
//NodeIndex : 4	Joint index : 2
//NodeIndex : 5	Joint index : 3
//NodeIndex : 6	Joint index : 4
//NodeIndex : 7	Joint index : 5
//NodeIndex : 8	Joint index : 6
//NodeIndex : 9	Joint index : 7
//NodeIndex : 10	Joint index : 8
//NodeIndex : 11	Joint index : 9
//NodeIndex : 12	Joint index : 10
//NodeIndex : 13	Joint index : 11
//NodeIndex : 14	Joint index : 12
//NodeIndex : 15	Joint index : 13
//NodeIndex : 16	Joint index : 14
//NodeIndex : 17	Joint index : 15
//NodeIndex : 18	Joint index : 16
//NodeIndex : 19	Joint index : 17
//NodeIndex : 20	Joint index : 18
//NodeIndex : 21	Joint index : 19
//NodeIndex : 22	Joint index : 20
//NodeIndex : 23	Joint index : 21
//NodeIndex : 24	Joint index : 22
//NodeIndex : 25	Joint index : 23