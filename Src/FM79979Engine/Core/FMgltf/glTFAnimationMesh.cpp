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

cAnimationMesh::~cAnimationMesh()
{
    int l_iCount = m_AllNodeConvertToBoneBoneVector.Count();
    for (int i = 0; i < l_iCount; i++)
    {
        auto l_pData = m_AllNodeConvertToBoneBoneVector[i];
        if (l_pData)
        {
            l_pData->SetParent(nullptr);
        }
    }
    m_AllNodeConvertToBoneBoneVector.Destroy();
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
    std::map<int, cBone*> l_NodeIndexAndBoneMap;
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
        cBone*bone = new cBone(boneName.c_str(),(int)i);
        bool l_bSameName = m_AllNodeConvertToBoneBoneVector.AddObject(bone);
        assert(l_bSameName&&"node not allow to has same name!?");
        l_tinyglTFNodeAndJointIndexMap[&node] = bone;
        l_NodeIndexAndBoneMap[(int)i] = bone;
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
    int l_iCount = (int)this->m_SkinningBoneVector.size();
    for (int i = 0; i < l_iCount; ++i)
    {
        m_SkinningBoneVector[i]->m_StartNodeWorldTransform = m_SkinningBoneVector[i]->GetWorldTransform();
    }
    
    // Load skins
    loadSkins(model, l_NodeIndexAndBoneMap);

    // Load animations
    loadAnimations(model, l_NodeIndexAndBoneMap);

    if (!m_pAllBonesMatrixForSkinned)
    {        
        m_pAllBonesMatrixForSkinned = new cMatrix44[this->m_SkinningBoneVector.size()];
        
    }
    if (this->m_pMainRootBone)
    {
        DumpBoneIndexDebugInfo(this->m_pMainRootBone, false, true);
    }
}

void cAnimationMesh::loadSkins(const tinygltf::Model& model, std::map<int, cBone*>& e_NodeIndexAndBoneMap)
{
    for (const auto& skin : model.skins)
    {
        m_JointOrderVector = skin.joints;
        if (skin.skeleton != -1)
        {
            m_pMainRootBone = e_NodeIndexAndBoneMap[skin.skeleton];
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
                int l_NodeIndex = skin.joints[i];
                auto l_pBone = this->m_AllNodeConvertToBoneBoneVector[l_NodeIndex];
                if(l_pBone)
                {
                    m_SkinningBoneVector.push_back(l_pBone);
                    l_pBone->m_iJointIndex = (int)i;
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

void cAnimationMesh::loadNode(const tinygltf::Node& node, const tinygltf::Model& model, cBone* parentBone, std::map<const tinygltf::Node*, cBone*>& e_tinyglTFNodeAndJointIndexMap)
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

    cMatrix44   l_matNodeTransform = cMatrix44::Identity;
    SRT         l_SRT;
    //because gltf matrix is column so take trs to make it right?
    if (node.translation.size() == 3)
    {
        Vector3 translation((float)node.translation[0], (float)node.translation[1], (float)node.translation[2]);
        l_matNodeTransform = cMatrix44::TranslationMatrix(translation);
        l_SRT.translation = translation;
        l_SRT.iSRTFlag |= SRT_TRANSLATION_FLAG;
    }

    if (node.rotation.size() == 4)
    {
        Quaternion rotation((float)node.rotation[0], (float)node.rotation[1], (float)node.rotation[2], (float)node.rotation[3]);
        l_matNodeTransform *= rotation.ToMatrix();
        l_SRT.rotation = rotation;
        l_SRT.iSRTFlag |= SRT_ROTATION_FLAG;
    }
    if (node.scale.size() == 3)
    {
        Vector3 scale((float)node.scale[0], (float)node.scale[1], (float)node.scale[2]);
        l_matNodeTransform *= cMatrix44::ScaleMatrix(scale);
        l_SRT.scale = scale;
        l_SRT.iSRTFlag |= SRT_SCALE_FLAG;
    }

    if (node.matrix.size() == 16)
    {
        cMatrix44 nodeMatrix = cMatrix44(node.matrix.data());
        l_matNodeTransform = nodeMatrix;
    }
    //11
    bone->m_StartNodeTransform = l_matNodeTransform;
    bone->m_StartSRT = l_SRT;
    bone->SetLocalTransform(l_matNodeTransform);

    for (int childIndex : node.children)
    {
        loadNode(model.nodes[childIndex], model, bone, e_tinyglTFNodeAndJointIndexMap);
    }
}

void cAnimationMesh::loadAnimations(const tinygltf::Model& model, std::map<int, cBone*>& e_NodeIndexAndBoneMap)
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
            if (sampler.interpolation != "LINEAR")
            {
                continue;
            }
            auto it = e_NodeIndexAndBoneMap.find(channel.target_node);
            if (it == e_NodeIndexAndBoneMap.end())
            {
                continue;
            }

            cBone* bone = it->second;
            //cBone* bone = this->m_SkinningBoneVector[channel.target_node];
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
                    srt.translation = Vector3(outputData + (i * 3));
                    srt.iSRTFlag |= SRT_TRANSLATION_FLAG; // Set translation flag
                }
                else if (channel.target_path == "rotation")
                {
                    srt.rotation = Quaternion(outputData +(i * 4) ); 
                    //srt.rotation.Normalize();
                    srt.iSRTFlag |= SRT_ROTATION_FLAG; // Set rotation flag
                }
                else if (channel.target_path == "scale")
                {
                    srt.scale = Vector3(outputData +(i * 3));
                    srt.iSRTFlag |= SRT_SCALE_FLAG; // Set scale flag
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
        if (this->m_pMainRootBone)
        {
            this->UpdateNode(m_pMainRootBone, m_pCurrentAnimationData->m_fCurrentTime);
        }
        else
        {
            int boneCount = (int)this->m_JointOrderVector.size();
            for (int i = 0; i < boneCount; ++i)
            {
                cBone* bone = m_SkinningBoneVector[i];
                if (bone)
                {
                    bone->EvaluateLocalXForm(m_pCurrentAnimationData->m_fCurrentTime);
                }
            }
        }
    }
    int boneCount = (int)this->m_JointOrderVector.size();
    for (int i = 0; i < boneCount; ++i)
    {
        //int l_iBoneIndex = m_JointOrderVector[i];
        cBone* bone = m_SkinningBoneVector[i];
        if (bone)
        {
            assert(i == bone->m_iJointIndex);
            if (bone->m_iJointIndex == -1)
            {
                continue;
            }
            auto l_matWorldTransform = bone->GetWorldTransform();
            //auto l_mat = l_matWorldTransform *bone->m_matInvBindPose;
            auto l_mat = l_matWorldTransform * bone->m_matInvBindPose;
            m_pAllBonesMatrixForSkinned[i] = l_mat;
            //m_pAllBonesMatrixForSkinned[i] = l_matWorldTransform;
        }
        else
        {
            int a = 0;
        }
    }
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
        int boneCount = (int)this->m_JointOrderVector.size();
        for (int i = 0; i < boneCount; ++i)
        {
            auto l_pBone = m_SkinningBoneVector[i];
            m_pAllBonesMatrixForSkinned[i] = l_pBone->m_StartNodeWorldTransform;
        }
    }
}

void cAnimationMesh::Update(float elapsedTime)
{
    // Ensure the current animation data is valid
    if (!m_pCurrentAnimationData)
    {
        auto l_Animation = m_NameAndAnimationMap.begin();
        if (m_NameAndAnimationMap.size() > 1)
        {
            ++l_Animation;
            ++l_Animation;
        }
        this->SetCurrentAnimation(l_Animation->first);
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
    //angle = 90;
    cMatrix44 conversionMatrix = cMatrix44::Identity;
    //conversionMatrix.m[2][2] = -1.0f;
    // Update the bone matrices for skinning

    auto l_vPos = this->GetWorldPosition();
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

        cMatrix44 modelMatrix = cMatrix44::TranslationMatrix(l_vPos);
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

        // Bind textures
        for (size_t i = 0; i < m_uiTextureIDVector.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + (GLenum)i);
            glBindTexture(GL_TEXTURE_2D, m_uiTextureIDVector[i]);
        }
        GLuint texture1Loc = glGetUniformLocation(l_pSubMesh->shaderProgram, "texture1");
        glUniform1i(texture1Loc, 0);

        // Bind normal map texture if available
        if (!m_uiNormalTextureIDVector.empty())
        {
            glActiveTexture(GL_TEXTURE0 + (GLenum)m_uiTextureIDVector.size());
            glBindTexture(GL_TEXTURE_2D, m_uiNormalTextureIDVector[0]);
            GLuint normalMapLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "normalMap");
            glUniform1i(normalMapLoc, (GLint)m_uiTextureIDVector.size());
        }

        // Bind the vertex array and draw the sub-mesh
        glBindVertexArray(l_pSubMesh->vao);
        EnableVertexAttributes(l_pSubMesh->m_iFVFFlag);
        MY_GLDRAW_ELEMENTS(GL_TRIANGLES, (GLsizei)l_pSubMesh->m_IndexBuffer.size(), GL_UNSIGNED_INT, 0);
    }
}

void cAnimationMesh::SetCurrentAnimationTime(float e_fCurrentTime)
{
    if (m_pCurrentAnimationData)
    {
        m_pCurrentAnimationData->m_fCurrentTime = e_fCurrentTime;
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