#include "tiny_gltf.h"
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

cAnimationMesh::cAnimationMesh()
    : m_pMainRootBone(nullptr), m_pAllBonesMatrixForSkinned(nullptr), m_pCurrentAnimationData(nullptr)
{
}

cBone* cAnimationMesh::FindBoneByIndex(int e_iIndex)
{
    for (int i = 0; i < m_SkinningBoneVector.Count(); ++i)
    {
        if (m_SkinningBoneVector[i]->m_iJointIndex == e_iIndex)
        {
            return m_SkinningBoneVector[i];
        }
    }
    return nullptr;
}

cAnimationMesh::~cAnimationMesh()
{
    int l_iCount = m_SkinningBoneVector.Count();
    for (int i = 0; i < l_iCount; i++)
    {
        auto l_pData = m_SkinningBoneVector[i];
        if (l_pData)
        {
            l_pData->SetParent(nullptr);
        }
    }
    m_SkinningBoneVector.Destroy();
    SAFE_DELETE_ARRAY(m_pAllBonesMatrixForSkinned);
    DELETE_MAP(m_NameAndAnimationMap)
}


void	DumpBoneIndexDebugInfo(cBone*e_pBone,bool e_bDoNextSibling, bool e_bRoot)
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
    l_strDebugInfo += ValueToString(e_pBone->m_iJointIndex);
    //FMLog::LogWithFlag(l_str, CORE_LOG_FLAG);
    FMLog::Log(l_strDebugInfo.c_str(), false);
    if (e_pBone->GetFirstChild())
    {
        DumpBoneIndexDebugInfo((cBone*)e_pBone->GetFirstChild(), true, false);
    }

    if (e_bDoNextSibling && e_pBone->GetNextSibling())
    {
        DumpBoneIndexDebugInfo((cBone*)e_pBone->GetNextSibling(),e_bDoNextSibling, false);
    }
    if (e_bRoot)
    {
        FMLOG("Total count:%d", l_siCount);
        l_siCount = 0;
    }
}

void cAnimationMesh::LoadAnimations(const tinygltf::Model& model)
{
    std::map<int, cBone*> nodeToBoneMap;    
    std::map<const tinygltf::Node*, cBone*> l_tinyglTFNodeAndJointIndexMap;
    //first generate all bones from all nodes
    for (size_t i = 0; i < model.nodes.size(); i++)
    {
        const tinygltf::Node& node = model.nodes[i];
        std::wstring boneName = std::wstring(node.name.begin(), node.name.end());
        if (boneName.length() == 0)
        {
            boneName = ValueToStringW(i);
        }
        cBone*bone = new cBone(boneName.c_str(), i);
        bool l_bSameName = m_SkinningBoneVector.AddObject(bone);
        assert(l_bSameName&&"node not allow to has same name!?");
        l_tinyglTFNodeAndJointIndexMap[&node] = bone;
        nodeToBoneMap[i] = bone;
    }
    //get noe from scene to setup hirerachy and transform.
    for (auto l_Scene :model.scenes)
    {
        for (int i = 0; i < l_Scene.nodes.size(); ++i)
        {
            auto l_iNodeID = l_Scene.nodes[i];
            const tinygltf::Node& node = model.nodes[l_iNodeID];
            loadNode(node, model, nullptr, l_tinyglTFNodeAndJointIndexMap);
        }
    }

    // Load skins
    loadSkins(model, nodeToBoneMap);

    // Load animations
    loadAnimations(model, nodeToBoneMap);

    if (!m_pAllBonesMatrixForSkinned)
    {
        m_pAllBonesMatrixForSkinned = new cMatrix44[m_SkinningBoneVector.Count()];
    }
    if (this->m_pMainRootBone)
    {
        
        DumpBoneIndexDebugInfo(this->m_pMainRootBone, false, true);
    }
}

void cAnimationMesh::loadSkins(const tinygltf::Model& model, std::map<int, cBone*>& nodeToBoneMap)
{
    for (const auto& skin : model.skins)
    {
        m_JointOrderVector = skin.joints;
        if (skin.skeleton != -1)
        {
            m_pMainRootBone = nodeToBoneMap[skin.skeleton];
        }
        //assert(m_pMainRootBone&&"can't find root");
        if (skin.inverseBindMatrices > -1)
        {
            const auto& accessor = model.accessors[skin.inverseBindMatrices];
            const auto& bufferView = model.bufferViews[accessor.bufferView];
            const auto& buffer = model.buffers[bufferView.buffer];
            const float* data = reinterpret_cast<const float*>(buffer.data.data() + bufferView.byteOffset + accessor.byteOffset);
            assert(accessor.count == skin.joints.size()&&"inverse matrix not match");
            for (size_t i = 0; i < skin.joints.size(); ++i)
            {
                int jointIndex = skin.joints[i];
                auto l_pBone = this->FindBoneByIndex(jointIndex);
                if(l_pBone)
                {
                    l_pBone->m_matInvBindPose = cMatrix44(data + i * 16);
                }
                else
                {
                    FMLOG("Error no such joint:%d", i);
                }
            }
        }
    }
}

void cAnimationMesh::loadNode(const tinygltf::Node& node, const tinygltf::Model& model, cBone* parentBone, std::map<const tinygltf::Node*, cBone*> e_tinyglTFNodeAndJointIndexMap)
{
    cBone* bone = nullptr;
    auto l_IT2 = e_tinyglTFNodeAndJointIndexMap.find(&node);
    if (l_IT2 != e_tinyglTFNodeAndJointIndexMap.end())
    {
        bone = l_IT2->second;
    }
    else
    {
        int notfound = 0;
        assert(0 && "can't find node index by name");
        return;
    }

    if (parentBone)
    {
#ifdef DEBUG
        if (parentBone->m_iJointIndex == 3 &&
            bone->m_iJointIndex == 4)
        {
            //for debug
            int a = 0;
        }
#endif
        parentBone->AddChildToLast(bone);
    }

    cMatrix44 localTransform = cMatrix44::Identity;

    if (node.translation.size() == 3)
    {
        Vector3 translation((float)node.translation[0], (float)node.translation[1], (float)node.translation[2]);
        localTransform = cMatrix44::TranslationMatrix(translation);
    }

    if (node.rotation.size() == 4)
    {
        Quaternion rotation((float)node.rotation[0], (float)node.rotation[1], (float)node.rotation[2], (float)node.rotation[3]);
        localTransform *= rotation.ToMatrix();
    }

    if (node.scale.size() == 3)
    {
        Vector3 scale((float)node.scale[0], (float)node.scale[1], (float)node.scale[2]);
        localTransform *= cMatrix44::ScaleMatrix(scale);
    }

    if (node.matrix.size() == 16)
    {
        cMatrix44 nodeMatrix = cMatrix44(node.matrix.data());
        localTransform = nodeMatrix;
    }

    bone->SetLocalTransform(localTransform);

    for (int childIndex : node.children)
    {
        loadNode(model.nodes[childIndex], model, bone, e_tinyglTFNodeAndJointIndexMap);
    }
}

void cAnimationMesh::loadAnimations(const tinygltf::Model& model, std::map<int, cBone*>& nodeToBoneMap)
{
    for (const auto& animation : model.animations)
    {
        sAnimationData* l_pAnimationData = new sAnimationData();
        sAnimationData& animationData = *l_pAnimationData;
        animationData.m_fMinKeyTime = FLT_MAX;
        animationData.m_fMaxKeyTime = FLT_MIN;
        animationData.m_fCurrentTime = 0.0f;
        animationData.m_fStartTime = 0.0f;
        animationData.m_fEndTime = 0.0f;

        for (const auto& channel : animation.channels)
        {
            const auto& sampler = animation.samplers[channel.sampler];
            const auto& targetNode = model.nodes[channel.target_node];

            auto it = nodeToBoneMap.find(channel.target_node);
            if (it == nodeToBoneMap.end())
            {
                continue;
            }

            cBone* bone = it->second;
            FloatToSRTMap& keyframes = animationData.m_BoneIDAndAnimationData[bone];

            const auto& inputAccessor = model.accessors[sampler.input];
            const auto& inputBufferView = model.bufferViews[inputAccessor.bufferView];
            const auto& inputBuffer = model.buffers[inputBufferView.buffer];
            const float* inputData = reinterpret_cast<const float*>(inputBuffer.data.data() + inputBufferView.byteOffset + inputAccessor.byteOffset);

            const auto& outputAccessor = model.accessors[sampler.output];
            const auto& outputBufferView = model.bufferViews[outputAccessor.bufferView];
            const auto& outputBuffer = model.buffers[outputBufferView.buffer];
            const float* outputData = reinterpret_cast<const float*>(outputBuffer.data.data() + outputBufferView.byteOffset + outputAccessor.byteOffset);

            for (size_t i = 0; i < inputAccessor.count; ++i)
            {
                float time = inputData[i];
                SRT& srt = keyframes[time];

                if (channel.target_path == "translation")
                {
                    srt.translation = Vector3(outputData[i * 3], outputData[i * 3 + 1], outputData[i * 3 + 2]);
                    srt.iSRTFlag |= (1 << 3); // Set translation flag
                }
                else if (channel.target_path == "rotation")
                {
                    srt.rotation = Quaternion(outputData[i * 4 ], outputData[i * 4+1], outputData[i * 4 + 2], outputData[i * 4 + 3]);
                    srt.iSRTFlag |= (1 << 2); // Set rotation flag
                }
                else if (channel.target_path == "scale")
                {
                    srt.scale = Vector3(outputData[i * 3], outputData[i * 3 + 1], outputData[i * 3 + 2]);
                    srt.iSRTFlag |= (1 << 1); // Set scale flag
                }
            }

            if (!keyframes.empty())
            {
                animationData.m_fMinKeyTime = min(animationData.m_fMinKeyTime, keyframes.begin()->first);
                animationData.m_fMaxKeyTime = max(animationData.m_fMaxKeyTime, keyframes.rbegin()->first);
                animationData.m_fStartTime = animationData.m_fMinKeyTime;
                animationData.m_fEndTime = animationData.m_fMaxKeyTime;
            }
        }

        m_NameAndAnimationMap[animation.name] = l_pAnimationData;
    }
}



void cAnimationMesh::SetCurrentAnimation(const std::string& animationName)
{
    auto it = m_NameAndAnimationMap.find(animationName);
    if (it != m_NameAndAnimationMap.end())
    {
        m_CurrentAnimation = animationName;
        m_pCurrentAnimationData = it->second;
        RefreshAnimationData();
    }
}

void cAnimationMesh::UpdateAnimation(float deltaTime)
{
    if (m_pCurrentAnimationData)
    {
        float newTime(m_pCurrentAnimationData->m_fCurrentTime);
        // Handle time advance based on animation playback options
        if ((m_pCurrentAnimationData->m_fCurrentTime + deltaTime) > m_pCurrentAnimationData->m_fEndTime)
        {
            float endMinusStart = m_pCurrentAnimationData->m_fEndTime - m_pCurrentAnimationData->m_fStartTime;
            if (endMinusStart > 0.001)
            {
                //switch(m_animEndHandling)
                //{
                //case LOOP:
                //    {
                float overTime = (m_pCurrentAnimationData->m_fCurrentTime + deltaTime - m_pCurrentAnimationData->m_fEndTime);
                newTime = m_pCurrentAnimationData->m_fStartTime + (float)fmod(overTime, endMinusStart);
                //    }
                //    break;
                //case BLOCK:
                //default:
                //    newTime = m_endTime;
                //    break;
                //}
            }
            else
            {
                newTime = m_pCurrentAnimationData->m_fCurrentTime;
            }
        }
        else
        {
            newTime = m_pCurrentAnimationData->m_fCurrentTime + deltaTime;
        };
        m_pCurrentAnimationData->m_fCurrentTime = newTime;
        auto l_Vector = m_SkinningBoneVector.GetList();
        for(int i=0;i< m_SkinningBoneVector.Count();++i)
        {
            cBone* l_pBone = m_SkinningBoneVector[i];
            l_pBone->EvaluateLocalXForm(m_pCurrentAnimationData->m_fCurrentTime);
            //l_pBone->SetWorldTransform(cMatrix44::Identity);
        }
    }
    int boneCount = m_SkinningBoneVector.Count();
    std::vector<cMatrix44> l_AllBonesMatrixForSkinned1;
    std::vector<cMatrix44> l_AllBonesMatrixForSkinned2;
    l_AllBonesMatrixForSkinned1.resize(boneCount);
    l_AllBonesMatrixForSkinned2.resize(boneCount);
    for (int i = 0; i < boneCount; ++i)
    {
        m_pAllBonesMatrixForSkinned[i] = cMatrix44::Identity;
    }
    //std::ostringstream oss;
    boneCount = m_JointOrderVector.size();
    for (int i = 0; i < boneCount; ++i)
    {
        int l_iBoneIndex = m_JointOrderVector[i];
        cBone* bone = FindBoneByIndex(l_iBoneIndex);
        if (bone)
        {
            if (bone->m_iJointIndex == -1)
            {
                continue;
            }
            auto l_matWorldTransform = bone->GetWorldTransform();
            auto l_mat = l_matWorldTransform * bone->m_matInvBindPose;
            l_mat = bone->m_matInvBindPose*l_matWorldTransform;
            //l_mat = bone->m_matInvBindPose;
            //l_mat = l_matWorldTransform;
            //if (bone->m_iJointIndex < boneCount)
            {
                //m_pAllBonesMatrixForSkinned[bone->m_iJointIndex] = l_mat;
                m_pAllBonesMatrixForSkinned[l_iBoneIndex] = l_mat;
                //l_AllBonesMatrixForSkinned1[bone->m_iJointIndex] = l_mat;
                //oss << "Bone " << i << " Index: " << bone->m_iJointIndex << "\n";
                //oss << "Inverse Bind Pose: " << ValueToString(bone->m_matInvBindPose).c_str() << "\n";
                //oss << "bone animation worldt ransform: " << ValueToString(l_matWorldTransform).c_str() << "\n";
                //oss << "final Transform to shader code: " << ValueToString(l_mat).c_str() << "\n";

            }
            //else
            {
                //int a = 0;
            }
        }
        else
        {
            int a = 0;
        }
    }
    //FMLOG(oss.str().c_str());
    //for (int i = 0; i < m_JointOrderVector.size(); ++i)
    //{
    //    cBone* bone = FindBoneByIndex(m_JointOrderVector[i]);
    //    if (bone)
    //    {
    //        auto l_matWorldTransform = bone->GetWorldTransform();
    //        auto l_mat = l_matWorldTransform *bone->m_matInvBindPose;
    //        //auto l_mat = bone->m_matInvBindPose;
    //        if (bone->m_iJointIndex < boneCount)
    //        {
    //            m_pAllBonesMatrixForSkinned[i] = l_mat;// 
    //            l_AllBonesMatrixForSkinned2[i] = l_mat;

    //        }
    //        else
    //        {
    //            int a = 0;
    //        }
    //    }
    //}

}

void cAnimationMesh::RefreshAnimationData()
{
    if (m_pCurrentAnimationData)
    {
        m_pCurrentAnimationData->m_fCurrentTime = 0;
        for (auto l_IT : m_pCurrentAnimationData->m_BoneIDAndAnimationData)
        {
            l_IT.first->SetFormKeyFrames(l_IT.second);
        }

    }
}

void cAnimationMesh::Update(float elapsedTime)
{
    // Ensure the current animation data is valid
    if (!m_pCurrentAnimationData)
    {
        auto l_Animation = m_NameAndAnimationMap.begin();
        //++l_Animation;
        //this->SetCurrentAnimation(l_Animation->first);
    }
    if (m_pCurrentAnimationData)
    {
        m_pCurrentAnimationData->m_fCurrentTime += elapsedTime;
    }
    UpdateAnimation(elapsedTime);
}

void	cAnimationMesh::UpdateNode(cBone* e_pBone, float e_fTime)
{
    e_pBone->EvaluateLocalXForm(e_fTime);
    cBone* l_pBone = (cBone*)e_pBone->GetFirstChild();
    if (l_pBone != nullptr)
    {
        UpdateNode(l_pBone, e_fTime);
    }
    l_pBone = (cBone*)e_pBone->GetNextSibling();
    if (l_pBone != nullptr)
    {
        UpdateNode(l_pBone, e_fTime);
    }
}

void cAnimationMesh::Draw()
{
    static float angle = 0.0f;
    static float lightAngle = 0.0f;
    static float l_fCameraZPosition = -6;
    lightAngle += 0.01f;
    angle += 0.01f;    
    cMatrix44 conversionMatrix = cMatrix44::Identity;
    conversionMatrix.m[2][2] = -1.0f;
    // Update the bone matrices for skinning

    auto l_vPos = this->GetWorldPosition();
    //l_vPos.y = 5;
    // Iterate through sub-meshes and draw each one
    for (auto& subMesh : subMeshes)
    {
        // Use the shader program specific to this sub-mesh
        glUseProgram(subMesh.shaderProgram);

        // Set model, view, projection matrices
        GLuint modelLoc = glGetUniformLocation(subMesh.shaderProgram, "inMat4Model");
        GLuint viewLoc = glGetUniformLocation(subMesh.shaderProgram, "inMat4View");
        GLuint projLoc = glGetUniformLocation(subMesh.shaderProgram, "inMat4Projection");

        cMatrix44 modelMatrix = cMatrix44::TranslationMatrix(l_vPos);
        cMatrix44 viewMatrix;// = cMatrix44::LookAtMatrix(Vector3(0, -0, l_fCameraZPosition), Vector3(0, 0, 0), Vector3(0, 1, 0));
        subMesh.GetProperCameraPosition(viewMatrix);

        viewMatrix.GetTranslation().z *= -1;
        Projection projectionMatrix;
        projectionMatrix.SetFovYAspect(XM_PIDIV4, (float)1920 / (float)1080, 0.1f, 10000.0f);

        modelMatrix = conversionMatrix * modelMatrix;
        cMatrix44 rotationMatrix = cMatrix44::YAxisRotationMatrix(angle);
        modelMatrix = rotationMatrix * modelMatrix;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projectionMatrix.GetMatrix());

        // Set light and view position uniforms
        GLuint lightColorLoc = glGetUniformLocation(subMesh.shaderProgram, "inVec3LightColor");
        GLuint lightPosLoc = glGetUniformLocation(subMesh.shaderProgram, "inVec3LightPosition");
        GLuint viewPosLoc = glGetUniformLocation(subMesh.shaderProgram, "inVec3ViewPosition");

        Vector3 lightColor(1.0f, 1.0f, 1.0f);
        Vector3 lightPos(100.0f * cos(lightAngle), 0.0f, 100.0f * sin(lightAngle));
        Vector3 viewPos(0.0f, 0.0f, 30.0f);

        glUniform3fv(lightColorLoc, 1, lightColor);
        glUniform3fv(lightPosLoc, 1, lightPos);
        glUniform3fv(viewPosLoc, 1, viewPos);

        // Set directional light uniforms
        GLuint dirLightDirLoc = glGetUniformLocation(subMesh.shaderProgram, "dirLightDirection");
        GLuint dirLightColorLoc = glGetUniformLocation(subMesh.shaderProgram, "dirLightColor");

        Vector3 dirLightDirection(-0.2f, -0.2f, 1.f);
        Vector3 dirLightColor(0.5f, 0.5f, 0.5f);

        glUniform3fv(dirLightDirLoc, 1, dirLightDirection);
        glUniform3fv(dirLightColorLoc, 1, dirLightColor);
        // Pass the bone matrices to the shader
        GLuint boneMatricesLocation = glGetUniformLocation(subMesh.shaderProgram, "uBoneTransforms");
        glUniformMatrix4fv(boneMatricesLocation, m_SkinningBoneVector.Count(), GL_FALSE, (float*)m_pAllBonesMatrixForSkinned);

        // Bind textures
        for (size_t i = 0; i < m_uiTextureIDVector.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + (GLenum)i);
            glBindTexture(GL_TEXTURE_2D, m_uiTextureIDVector[i]);
        }
        GLuint texture1Loc = glGetUniformLocation(subMesh.shaderProgram, "texture1");
        glUniform1i(texture1Loc, 0);

        // Bind normal map texture if available
        if (!m_uiNormalTextureIDVector.empty())
        {
            glActiveTexture(GL_TEXTURE0 + (GLenum)m_uiTextureIDVector.size());
            glBindTexture(GL_TEXTURE_2D, m_uiNormalTextureIDVector[0]);
            GLuint normalMapLoc = glGetUniformLocation(subMesh.shaderProgram, "normalMap");
            glUniform1i(normalMapLoc, (GLint)m_uiTextureIDVector.size());
        }

        // Bind the vertex array and draw the sub-mesh
        glBindVertexArray(subMesh.vao);
        EnableVertexAttributes(subMesh.fvfFlags);
        MY_GLDRAW_ELEMENTS(GL_TRIANGLES, (GLsizei)subMesh.indexBuffer.size(), GL_UNSIGNED_INT, 0);
    }
}

void cAnimationMesh::RenderBindPose()
{
    cMesh::Draw();
    return;
    static float angle = 0.0f;
    static float lightAngle = 0.0f;
    static float l_fCameraZPosition = -6;
    lightAngle += 0.01f;
    angle += 0.01f;
    auto l_vPos = this->GetLocalPosition();
    for (auto& subMesh : subMeshes)
    {
        // Use the shader program specific to this sub-mesh
        glUseProgram(subMesh.shaderProgram);

        // Set model, view, projection matrices
        GLuint modelLoc = glGetUniformLocation(subMesh.shaderProgram, "inMat4Model");
        GLuint viewLoc = glGetUniformLocation(subMesh.shaderProgram, "inMat4View");
        GLuint projLoc = glGetUniformLocation(subMesh.shaderProgram, "inMat4Projection");

        cMatrix44 modelMatrix = cMatrix44::TranslationMatrix(l_vPos);
        cMatrix44 viewMatrix = cMatrix44::LookAtMatrix(Vector3(0, -0, l_fCameraZPosition), Vector3(0, 0, 0), Vector3(0, 1, 0));
        subMesh.GetProperCameraPosition(viewMatrix);

        viewMatrix.GetTranslation().z *= -1;
        Projection projectionMatrix;
        projectionMatrix.SetFovYAspect(XM_PIDIV4, (float)1920 / (float)1080, 0.1f, 10000.0f);

        cMatrix44 conversionMatrix = cMatrix44::Identity;
        conversionMatrix.m[2][2] = -1.0f;

        modelMatrix = conversionMatrix * modelMatrix;
        cMatrix44 rotationMatrix = cMatrix44::YAxisRotationMatrix(angle);
        modelMatrix = rotationMatrix * modelMatrix;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projectionMatrix.GetMatrix());

        // Set light and view position uniforms
        GLuint lightColorLoc = glGetUniformLocation(subMesh.shaderProgram, "inVec3LightColor");
        GLuint lightPosLoc = glGetUniformLocation(subMesh.shaderProgram, "inVec3LightPosition");
        GLuint viewPosLoc = glGetUniformLocation(subMesh.shaderProgram, "inVec3ViewPosition");

        Vector3 lightColor(1.0f, 1.0f, 1.0f);
        Vector3 lightPos(100.0f * cos(lightAngle), 0.0f, 100.0f * sin(lightAngle));
        Vector3 viewPos(0.0f, 0.0f, 30.0f);

        glUniform3fv(lightColorLoc, 1, lightColor);
        glUniform3fv(lightPosLoc, 1, lightPos);
        glUniform3fv(viewPosLoc, 1, viewPos);

        // Set directional light uniforms
        GLuint dirLightDirLoc = glGetUniformLocation(subMesh.shaderProgram, "dirLightDirection");
        GLuint dirLightColorLoc = glGetUniformLocation(subMesh.shaderProgram, "dirLightColor");

        Vector3 dirLightDirection(-0.2f, -0.2f, 1.f);
        Vector3 dirLightColor(0.5f, 0.5f, 0.5f);

        glUniform3fv(dirLightDirLoc, 1, dirLightDirection);
        glUniform3fv(dirLightColorLoc, 1, dirLightColor);

        // Bind textures
        for (size_t i = 0; i < m_uiTextureIDVector.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + (GLenum)i);
            glBindTexture(GL_TEXTURE_2D, m_uiTextureIDVector[i]);
        }
        GLuint texture1Loc = glGetUniformLocation(subMesh.shaderProgram, "texture1");
        glUniform1i(texture1Loc, 0);

        // Bind normal map texture if available
        if (!m_uiNormalTextureIDVector.empty())
        {
            glActiveTexture(GL_TEXTURE0 + (GLenum)m_uiTextureIDVector.size());
            glBindTexture(GL_TEXTURE_2D, m_uiNormalTextureIDVector[0]);
            GLuint normalMapLoc = glGetUniformLocation(subMesh.shaderProgram, "normalMap");
            glUniform1i(normalMapLoc, (GLint)m_uiTextureIDVector.size());
        }

        // Bind the vertex array and draw the sub-mesh
        glBindVertexArray(subMesh.vao);
        EnableVertexAttributes(subMesh.fvfFlags);
        MY_GLDRAW_ELEMENTS(GL_TRIANGLES, (GLsizei)subMesh.indexBuffer.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void cAnimationMesh::SetCurrentAnimationTime(float e_fCurrentTime)
{
    if (m_pCurrentAnimationData)
    {
        m_pCurrentAnimationData->m_fCurrentTime = e_fCurrentTime;
        //if (m_pCurrentAnimationData->m_fCurrentTime > m_pCurrentAnimationData->m_fEndTime)
        //{
        //    UpdateNodes(m_pCurrentAnimationData->m_fEndTime);
        //}
        //else
        //if (m_pCurrentAnimationData->m_fCurrentTime < m_pCurrentAnimationData->m_fStartTime)
        //{
        //    UpdateNodes(m_pCurrentAnimationData->m_fStartTime);
        //}
        //else
        //{
        //    UpdateNodes(m_pCurrentAnimationData->m_fCurrentTime);
        //}
    }
}

void cAnimationMesh::RenderSkeleton()
{
    if (!m_pMainRootBone)
    {
        return;
    }

    // Set up OpenGL for line and point rendering
    glUseProgram(0); // Use fixed-function pipeline
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glPointSize(5.0f);
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for bones

    // Recursive function to render bones
    std::function<void(cBone*)> renderBone = [&](cBone* bone)
        {
            if (!bone)
            {
                return;
            }

            cBone* parentBone = static_cast<cBone*>(bone->GetParent());
            if (parentBone)
            {
                // Draw line from parent to current bone
                glBegin(GL_LINES);
                Vector3 parentPos = parentBone->GetWorldTransform().GetTranslation();
                Vector3 bonePos = bone->GetWorldTransform().GetTranslation();
                glVertex3f(parentPos.x, parentPos.y, parentPos.z);
                glVertex3f(bonePos.x, bonePos.y, bonePos.z);
                glEnd();
            }

            // Draw point at current bone position
            glBegin(GL_POINTS);
            Vector3 bonePos = bone->GetWorldTransform().GetTranslation();
            glVertex3f(bonePos.x, bonePos.y, bonePos.z);
            glEnd();

            // Recursively render child bones
            cBone* childBone = static_cast<cBone*>(bone->GetFirstChild());
            while (childBone)
            {
                renderBone(childBone);
                childBone = static_cast<cBone*>(childBone->GetNextSibling());
            }
        };

    // Start rendering from the root bone
    renderBone(m_pMainRootBone);

    // Restore OpenGL state
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glLineWidth(1.0f);
    glPointSize(1.0f);
}

