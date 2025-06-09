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

cSkinningMesh::cSkinningMesh(cSkinningMesh* e_pSkinningMesh)
{
    m_SkinningBoneVector = e_pSkinningMesh->m_SkinningBoneVector;
    m_pNodeInversePoseMatrixVector = e_pSkinningMesh->m_pNodeInversePoseMatrixVector;

    m_pMainRootBone = e_pSkinningMesh->m_pMainRootBone;
    m_SkinningBoneVector                = e_pSkinningMesh->m_SkinningBoneVector;
    m_pNodeInversePoseMatrixVector = e_pSkinningMesh->m_pNodeInversePoseMatrixVector;
    m_JointOrderVector = e_pSkinningMesh->m_JointOrderVector;
    //m_matMeshBindShapePose = e_pSkinningMesh->m_matMeshBindShapePose;
    m_AllBonesMatrixForSkinnedVector = e_pSkinningMesh->m_AllBonesMatrixForSkinnedVector;;
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
        DumpBoneIndexDebugInfo((cglTFNodeData*)e_pBone->GetFirstChild(), true, false);
    }

    if (e_bDoNextSibling && e_pBone->GetNextSibling())
    {
        DumpBoneIndexDebugInfo((cglTFNodeData*)e_pBone->GetNextSibling(), e_bDoNextSibling, false);
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
    m_JointOrderVector = std::make_shared<std::vector<int>>(e_Skin.joints);
    auto l_uiJointSize = e_Skin.joints.size();
    for (size_t i = 0; i < l_uiJointSize; ++i)
    {
    }
    if (e_Skin.inverseBindMatrices > -1)
    {
        const auto& accessor = e_Model.accessors[e_Skin.inverseBindMatrices];
        const auto& bufferView = e_Model.bufferViews[accessor.bufferView];
        const auto& buffer = e_Model.buffers[bufferView.buffer];
        const float* data = reinterpret_cast<const float*>(buffer.data.data() + (bufferView.byteOffset + accessor.byteOffset));
        //const float* matrixData = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

        assert(accessor.count == e_Skin.joints.size() && "inverse matrix not match");
        for (size_t i = 0; i < e_Skin.joints.size(); ++i)
        {
            int l_NodeIndex = e_Skin.joints[i];
            auto l_pNode = e_pModel->m_NodesVector[l_NodeIndex];
            m_SkinningBoneVector.push_back(l_pNode);
            assert(l_pNode->m_iNodeIndex == l_NodeIndex);
            if (l_pNode)
            {
                l_pNode->m_iJointIndex = (int)i;
                m_pNodeInversePoseMatrixVector->push_back(cMatrix44(data + i * 16));
            }
            else
            {
                FMLOG("Error no such joint:%d", i);
            }
        }
    }
    if (m_AllBonesMatrixForSkinnedVector.size() == 0)
    {
		m_AllBonesMatrixForSkinnedVector.resize(e_Skin.joints.size());
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
        //int l_iBoneIndex = m_JointOrderVector[i];
        cglTFNodeData* bone = m_SkinningBoneVector[i];
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
    GLuint boneMatricesLocation = glGetUniformLocation(e_pSubMesh->m_iShaderProgram, "uBoneTransforms");
    glUniformMatrix4fv(boneMatricesLocation, (GLsizei)m_SkinningBoneVector.size(), GL_FALSE, (float*)&m_AllBonesMatrixForSkinnedVector[0]);
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
    }
    auto l_matWorldTransoform = this->GetWorldTransform();
    for (auto& l_pSubMesh : this->m_SubMeshesVector)
    {
        SetSubMeshCommonUniformData(l_pSubMesh.get(), l_matWorldTransoform);
		cMesh::CallOpenGLDraw(l_pSubMesh.get());
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
        cglTFNodeData* l_pParent = (cglTFNodeData*)l_pMe->GetParent();
        if (l_pParent)
        {
            //parent
            l_mat = l_pParent->GetWorldTransform();
            if (!g_bApplyInverseBindPose)
            {
                l_mat = l_pParent->GetWorldTransform() * (*m_pNodeInversePoseMatrixVector)[l_pParent->m_iJointIndex];
            }
            l_vAllVertices.push_back(l_mat.GetTranslation());
            //me
            l_mat = (l_pMe->GetWorldTransform());
            if (!g_bApplyInverseBindPose)
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