#include "tiny_gltf.h"
#include "glTFAnimationMesh.h"
#include "glTFModel.h"
#include <set>
#include <unordered_set>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <cfloat>
#include <iostream>
#include <sstream>

bool g_bApplyInverseBindPose = true;

cSkinningMesh::cSkinningMesh()
    : m_pMainRootBone(nullptr), m_pAllBonesMatrixForSkinned(nullptr)
{
}

cSkinningMesh::~cSkinningMesh()
{
    SAFE_DELETE_ARRAY(m_pAllBonesMatrixForSkinned);
}


void	DumpBoneIndexDebugInfo(cglTFNodeData*e_pBone,bool e_bDoNextSibling, bool e_bRoot)
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
        DumpBoneIndexDebugInfo((cglTFNodeData*)e_pBone->GetNextSibling(),e_bDoNextSibling, false);
    }
    if (e_bRoot)
    {
        FMLOG("Total count:%d", l_siCount);
        l_siCount = 0;
    }
}

void cSkinningMesh::LoadAnimations(const tinygltf::Skin& e_Skin, cglTFModel* e_pModel, tinygltf::Model& e_Model)
{
    if (e_Skin.skeleton != -1)
    {
        m_pMainRootBone = e_pModel->m_NodesVector[e_Skin.skeleton];
    }
    m_JointOrderVector = e_Skin.joints;
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
                m_NodeInversePoseMatrixVector.push_back(cMatrix44(data + i * 16));
            }
            else
            {
                FMLOG("Error no such joint:%d", i);
            }
        }
    }
    if (!m_pAllBonesMatrixForSkinned)
    {        
        m_pAllBonesMatrixForSkinned = new cMatrix44[e_Skin.joints.size()];
    }
    RefreshAnimationData();
    if (this->m_pMainRootBone)
    {
        DumpBoneIndexDebugInfo(this->m_pMainRootBone, false, true);
    }
}



void cSkinningMesh::UpdateJointsMatrix()
{
    int boneCount = (int)this->m_JointOrderVector.size();
    for (int i = 0; i < boneCount; ++i)
    {
        //int l_iBoneIndex = m_JointOrderVector[i];
        cglTFNodeData* bone = m_SkinningBoneVector[i];
        if (bone)
        {
            assert(i == bone->m_iJointIndex);
            assert(m_JointOrderVector[i] == bone->m_iNodeIndex);
            if (bone->m_iJointIndex == -1)
            {
                continue;
            }
            auto l_matWorldTransform = bone->GetWorldTransform();
            auto l_mat = l_matWorldTransform;
            if (g_bApplyInverseBindPose)
            {
                l_mat = l_matWorldTransform * m_NodeInversePoseMatrixVector[i];
            }
            //m_pAllBonesMatrixForSkinned[bone->m_iJointIndex] = l_mat;
            m_pAllBonesMatrixForSkinned[i] = l_mat;
            //m_pAllBonesMatrixForSkinned[m_JointOrderVector[i]] = l_mat;
        }
        else
        {
            int a = 0;
        }
    }
}

void cSkinningMesh::RefreshAnimationData()
{
    int boneCount = (int)this->m_JointOrderVector.size();
    for (int i = 0; i < boneCount; ++i)
    {
        auto l_pBone = m_SkinningBoneVector[i];
        m_pAllBonesMatrixForSkinned[i] = l_pBone->m_StartNodeWorldTransform;
    }
}

void cSkinningMesh::Update(float e_fElpaseTime)
{
    UpdateJointsMatrix();
}

cMatrix44 g_PVMat;
cMatrix44 g_ModelMat;
void cSkinningMesh::Render()
{
    if (m_SkinningBoneVector.size() == 0)
    {
        cMesh::Render();
    }
    static float angle = 0.0f;
    static float lightAngle = 0.0f;
    static float l_fCameraZPosition = -6;
    lightAngle += 0.01f;
    angle += 0.01f;    
    //angle = 90;
    cMatrix44 conversionMatrix = cMatrix44::Identity;
    //conversionMatrix.m[2][2] = -1.0f;
    // Update the bone matrices for skinning

    auto l_matTransoform = this->GetWorldTransform();
    //l_vPos.y = 5;
    // Iterate through sub-meshes and draw each one
    for (auto& l_pSubMesh : this->m_SubMeshesVector)
    {
        // Use the shader program specific to this sub-mesh
        glUseProgram(l_pSubMesh->shaderProgram);

        // Set model, view, projection matrices
        GLuint modelLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "inMat4Model");
        GLuint viewLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "inMat4View");
        GLuint projLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "inMat4Projection");

        cMatrix44 modelMatrix = l_matTransoform;
        cMatrix44 viewMatrix;// = cMatrix44::LookAtMatrix(Vector3(0, -0, l_fCameraZPosition), Vector3(0, 0, 0), Vector3(0, 1, 0));
        l_pSubMesh->GetProperCameraPosition(viewMatrix);

        viewMatrix.GetTranslation().z *= -1;
        Projection projectionMatrix;
        projectionMatrix.SetFovYAspect(XM_PIDIV4, (float)1920 / (float)1080, 0.1f, 10000.0f);

        modelMatrix = conversionMatrix * modelMatrix;
        cMatrix44 rotationMatrix = cMatrix44::YAxisRotationMatrix(angle);
        modelMatrix = rotationMatrix * modelMatrix;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projectionMatrix.GetMatrix());
        g_PVMat = projectionMatrix.GetMatrix() * viewMatrix;
        g_ModelMat = modelMatrix;

        // Set light and view position uniforms
        GLuint lightColorLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "inVec3LightColor");
        GLuint lightPosLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "inVec3LightPosition");
        GLuint viewPosLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "inVec3ViewPosition");

        Vector3 lightColor(1.0f, 1.0f, 1.0f);
        Vector3 lightPos(100.0f * cos(lightAngle), 0.0f, 100.0f * sin(lightAngle));
        Vector3 viewPos(0.0f, 0.0f, 30.0f);

        glUniform3fv(lightColorLoc, 1, lightColor);
        glUniform3fv(lightPosLoc, 1, lightPos);
        glUniform3fv(viewPosLoc, 1, viewPos);

        // Set directional light uniforms
        GLuint dirLightDirLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "dirLightDirection");
        GLuint dirLightColorLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "dirLightColor");

        Vector3 dirLightDirection(-0.2f, -0.2f, 1.f);
        Vector3 dirLightColor(0.5f, 0.5f, 0.5f);

        glUniform3fv(dirLightDirLoc, 1, dirLightDirection);
        glUniform3fv(dirLightColorLoc, 1, dirLightColor);
        // Pass the bone matrices to the shader
        GLuint boneMatricesLocation = glGetUniformLocation(l_pSubMesh->shaderProgram, "uBoneTransforms");
        glUniformMatrix4fv(boneMatricesLocation, (GLsizei)m_SkinningBoneVector.size(), GL_FALSE, (float*)m_pAllBonesMatrixForSkinned);
        ApplyMaterial();;

        // Bind the vertex array and draw the sub-mesh
        glBindVertexArray(l_pSubMesh->m_uiVAO);
        EnableVertexAttributes(l_pSubMesh->m_iFVFFlag);
        MY_GLDRAW_ELEMENTS(GL_TRIANGLES, (GLsizei)l_pSubMesh->m_IndexBuffer.size(), GL_UNSIGNED_INT, 0);
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
                l_mat = l_pParent->GetWorldTransform() * m_NodeInversePoseMatrixVector[l_pParent->m_iJointIndex];
            }
            l_vAllVertices.push_back(l_mat.GetTranslation());
            //me
            l_mat = (l_pMe->GetWorldTransform());
            if (!g_bApplyInverseBindPose)
            {
                l_mat = (l_pMe->GetWorldTransform() * m_NodeInversePoseMatrixVector[l_pMe->m_iJointIndex]);
            }
            l_vAllVertices.push_back(l_mat.GetTranslation());
            ++l_iNumBone;
        }
        else
        {
            l_mat = (l_pMe->GetWorldTransform() * m_NodeInversePoseMatrixVector[l_pMe->m_iJointIndex]);
        }
        l_vPoints.push_back(l_mat.GetTranslation());
    }
    if (l_vPoints.size() == 0)
    {
        return;
    }
    //l_mat = g_ModelMat * cMatrix44::ZAxisRotationMatrix(D3DXToRadian(180));
    l_mat = g_ModelMat ;
    UseShaderProgram(NO_TEXTURE_SHADER);
    SetupShaderViewProjectionMatrix(g_PVMat, false);
    GLRender::RenderLine((float*)&l_vAllVertices[0], (int)l_vAllVertices.size() , Vector4(0.f, 1.f, 0.5f, 1.f), 3, l_mat);
    GLRender::RenderPoints(&l_vPoints[0], (int)l_vPoints.size(), 5, Vector4(0.f, 1.f, 1.f, 1.f), l_mat);
}