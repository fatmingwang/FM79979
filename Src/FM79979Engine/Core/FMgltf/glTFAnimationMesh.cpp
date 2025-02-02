#include "tiny_gltf.h"
#include "glTFAnimationMesh.h"
#include <set>
#include <unordered_set>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <cfloat>

cAnimationMesh::cAnimationMesh()
    : m_pMainRootBone(nullptr), m_pAllBonesMatrixForSkinned(nullptr), m_pCurrentAnimationData(nullptr)
{
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
}

int FindRootNode(const tinygltf::Model& model, const tinygltf::Animation& animation)
{
    std::unordered_set<int> animatedNodes;

    // Step 1: Collect all nodes affected by this animation
    for (const auto& channel : animation.channels)
    {
        if (channel.target_node >= 0)
        {
            animatedNodes.insert(channel.target_node);
        }
    }

    if (animatedNodes.empty())
    {
        return -1; // No animated nodes
    }

    // Step 2: Find the root-most node
    std::unordered_set<int> allNodes;
    for (const auto& node : model.nodes)
    {
        allNodes.insert(&node - &model.nodes[0]); // Store node indices
    }

    // Step 3: Traverse up the hierarchy to find the root
    for (int nodeIndex : animatedNodes)
    {
        int parent = nodeIndex;
        while (true)
        {
            bool foundParent = false;
            for (size_t i = 0; i < model.nodes.size(); ++i)
            {
                const auto& node = model.nodes[i];
                for (int child : node.children)
                {
                    if (child == parent)
                    {
                        parent = (int)i;
                        foundParent = true;
                        break;
                    }
                }
                if (foundParent) break;
            }
            if (!foundParent) break; // We found the root
        }
        return parent; // Return the root node of the animation
    }

    return -1; // No valid root found
}

void cAnimationMesh::LoadAnimations(const tinygltf::Model& model)
{
    std::map<int, cBone*> nodeToBoneMap;
    std::unordered_set<int> skinningJoints;
    int l_iRootIndex = -1;

    // Collect all joints used in skins
    for (const auto& skin : model.skins)
    {
        for (int jointIndex : skin.joints)
        {
            skinningJoints.insert(jointIndex);
        }
    }

    // Create bones and set their names for nodes that are part of the skinning joints
    for (size_t i = 0; i < model.nodes.size(); ++i)
    {
        if (skinningJoints.find((int)i) == skinningJoints.end())
        {
            continue; // Skip nodes that are not part of the skinning joints
        }

        const auto& node = model.nodes[i];
        if (node.name.empty())
        {
            continue;
        }

        cBone* bone = new cBone(std::wstring(node.name.begin(), node.name.end()).c_str());
        nodeToBoneMap[i] = bone;
        m_SkinningBoneVector.AddObject(bone);
    }

    l_iRootIndex = FindRootNode(model, model.animations[0]);

    // Set parent-child relationships and local transforms
    for (size_t i = 0; i < model.nodes.size(); ++i)
    {
        if (skinningJoints.find((int)i) == skinningJoints.end())
        {
            continue; // Skip nodes that are not part of the skinning joints
        }

        const auto& node = model.nodes[i];
        if (node.name.empty())
        {
            continue;
        }

        auto it = nodeToBoneMap.find(i);
        if (it == nodeToBoneMap.end())
        {
            continue;
        }

        cBone* bone = it->second;

        // Set children
        for (int childIndex : node.children)
        {
            if (skinningJoints.find(childIndex) == skinningJoints.end())
            {
                continue; // Skip child nodes that are not part of the skinning joints
            }

            auto childIt = nodeToBoneMap.find(childIndex);
            if (childIt != nodeToBoneMap.end())
            {
                cBone* childBone = childIt->second;
                childBone->SetParent(bone);
            }
        }

        // Set local transform
        cMatrix44 localTransform = cMatrix44::Identity;

        // Apply translation
        if (node.translation.size() == 3)
        {
            Vector3 translation((float)node.translation[0], (float)node.translation[1], (float)node.translation[2]);
            localTransform = cMatrix44::TranslationMatrix(translation);
        }

        // Apply rotation
        if (node.rotation.size() == 4)
        {
            Quaternion rotation((float)node.rotation[3], (float)node.rotation[0], (float)node.rotation[1], (float)node.rotation[2]);
            localTransform *= rotation.ToMatrix();
        }

        // Apply scale
        if (node.scale.size() == 3)
        {
            Vector3 scale((float)node.scale[0], (float)node.scale[1], (float)node.scale[2]);
            localTransform *= cMatrix44::ScaleMatrix(scale);
        }

        // Apply matrix if present
        if (node.matrix.size() == 16)
        {
            cMatrix44 nodeMatrix = cMatrix44(node.matrix.data());
            localTransform *= nodeMatrix;
        }

        bone->SetLocalTransform(localTransform);

        // Assign the root bone
        if (i == l_iRootIndex)
        {
            m_pMainRootBone = bone;
        }
    }

    // Load all animations
    for (const auto& animation : model.animations)
    {
        sAnimationData animationData;
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
            FloatTocMatrix44Map keyframes;

            // Load keyframe times
            const auto& inputAccessor = model.accessors[sampler.input];
            const auto& inputBufferView = model.bufferViews[inputAccessor.bufferView];
            const auto& inputBuffer = model.buffers[inputBufferView.buffer];
            const float* inputData = reinterpret_cast<const float*>(inputBuffer.data.data() + inputBufferView.byteOffset + inputAccessor.byteOffset);

            // Load keyframe transforms
            const auto& outputAccessor = model.accessors[sampler.output];
            const auto& outputBufferView = model.bufferViews[outputAccessor.bufferView];
            const auto& outputBuffer = model.buffers[outputBufferView.buffer];
            const float* outputData = reinterpret_cast<const float*>(outputBuffer.data.data() + outputBufferView.byteOffset + outputAccessor.byteOffset);

            for (size_t i = 0; i < inputAccessor.count; ++i)
            {
                cMatrix44 transform;
                memcpy(&transform, outputData + i * 16, sizeof(cMatrix44));
                keyframes[inputData[i]] = transform;
            }

            bone->SetFormKeyFrames(keyframes);
            animationData.m_BoneIDAndAnimationData[channel.target_node] = keyframes;

            // Update min and max key times
            if (!keyframes.empty())
            {
                animationData.m_fMinKeyTime = (animationData.m_fMinKeyTime, keyframes.begin()->first);
                animationData.m_fMaxKeyTime = (animationData.m_fMaxKeyTime, keyframes.rbegin()->first);
            }
        }

        m_NameAndAnimationMap[animation.name] = animationData;
    }

    RefreshAnimationData();
}

void cAnimationMesh::SetCurrentAnimation(const std::string& animationName)
{
    auto it = m_NameAndAnimationMap.find(animationName);
    if (it != m_NameAndAnimationMap.end())
    {
        m_CurrentAnimation = animationName;
        m_pCurrentAnimationData = &it->second;
        RefreshAnimationData();
    }
}

void cAnimationMesh::UpdateAnimation(float deltaTime)
{
    if (m_pCurrentAnimationData)
    {
        m_pCurrentAnimationData->m_fCurrentTime += deltaTime;
        auto l_Vector = m_SkinningBoneVector.GetList();
        for (auto& bone : *l_Vector)
        {
            bone->EvaluateLocalXForm(m_pCurrentAnimationData->m_fCurrentTime);
        }
    }
}

void cAnimationMesh::RefreshAnimationData()
{
    if (m_pCurrentAnimationData)
    {
        cBone** l_ppBone = &(*m_SkinningBoneVector.GetList())[0];
        m_pCurrentAnimationData->m_fMinKeyTime = FLT_MAX;
        m_pCurrentAnimationData->m_fMaxKeyTime = FLT_MIN;
        int l_iNum = this->m_SkinningBoneVector.Count();
        for (int i = 0; i < l_iNum; ++i)
        {
            cBone* l_pBone = l_ppBone[i];
            m_pCurrentAnimationData->m_fMinKeyTime = (m_pCurrentAnimationData->m_fMinKeyTime, l_pBone->m_fMinKeyTime);
            m_pCurrentAnimationData->m_fMaxKeyTime = (m_pCurrentAnimationData->m_fMaxKeyTime, l_pBone->m_fMaxKeyTime);
        }
        if (m_pCurrentAnimationData->m_fMinKeyTime != FLT_MAX)
        {
            m_pCurrentAnimationData->m_fStartTime = m_pCurrentAnimationData->m_fMinKeyTime;
            m_pCurrentAnimationData->m_fEndTime = m_pCurrentAnimationData->m_fMaxKeyTime;
        }
        else
        {
            m_pCurrentAnimationData->m_fMinKeyTime = 0;
            m_pCurrentAnimationData->m_fMaxKeyTime = 0;
        }
        if (m_pCurrentAnimationData->m_fStartTime < 0)
            m_pCurrentAnimationData->m_fStartTime = 0;
        if (!m_pAllBonesMatrixForSkinned)
        {
            m_pAllBonesMatrixForSkinned = new cMatrix44[l_iNum];
        }
    }
}

void cAnimationMesh::Update(float elapsedTime)
{
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
    // Ensure the current animation data is valid
    if (!m_pCurrentAnimationData)
    {
        return;
    }
    static float angle = 0.0f;
    static float lightAngle = 0.0f;
    static float l_fCameraZPosition = -6;
    lightAngle += 0.01f;
    angle += 0.01f;
    cBaseShader* l_pShader = GetCurrentShader();
    if (l_pShader)
    {
        l_pShader->Unuse();
    }
    UseShaderProgram(L"qoo79979");
    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // Update the animation to the current time
    UpdateAnimation(m_pCurrentAnimationData->m_fCurrentTime);

    // Update the bone matrices for skinning
    int boneCount = m_SkinningBoneVector.Count();
    for (int i = 0; i < boneCount; ++i)
    {
        cBone* bone = m_SkinningBoneVector[i];
        if (bone)
        {
            m_pAllBonesMatrixForSkinned[i] = bone->GetWorldTransform() * bone->m_matInvBindPose;
        }
    }
    auto l_vPos = this->GetWorldPosition();
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
        // Pass the bone matrices to the shader
        GLuint boneMatricesLocation = glGetUniformLocation(subMesh.shaderProgram, "uBoneTransforms");
        glUniformMatrix4fv(boneMatricesLocation, boneCount, GL_FALSE, (const GLfloat*)m_pAllBonesMatrixForSkinned);

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

    // Unbind the shader
    glUseProgram(0);
}


void cAnimationMesh::JointUpdate(float elapsedTime)
{
    // Implement the joint update logic here
}

void cAnimationMesh::SetCurrentAnimationTime(float e_fCurrentTime)
{
    if (m_pCurrentAnimationData)
    {
        m_pCurrentAnimationData->m_fCurrentTime = e_fCurrentTime;
    }
}
