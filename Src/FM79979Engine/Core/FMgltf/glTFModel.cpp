
#include "../AllCoreInclude.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include "glTFModel.h"
#include "LazyShaderGenerator.h"
#include "glTFLight.h"
#include "glTFCamera.h"

TYPDE_DEFINE_MARCO(cglTFModel);

cglTFModel::~cglTFModel()
{
    this->Destory();
}


GLuint cglTFModel::CreateShader(int64 fvfFlags, int e_iNumMorphTarget)
{
    std::string vertexCode = GenerateVertexShaderWithFVF(fvfFlags, e_iNumMorphTarget);
    std::string fragmentCode = GenerateFragmentShaderWithFVF(fvfFlags);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vShaderCode = vertexCode.c_str();
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    LAZY_DO_GL_COMMAND_AND_GET_ERROR(glCompileShader(vertexShader));

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fShaderCode = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    LAZY_DO_GL_COMMAND_AND_GET_ERROR(glCompileShader(fragmentShader));

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        FMLOG(infoLog);
        assert(0&&"shader compile error");
    }
    else
    {
#ifdef DEBUG
        PopulateUniform(shaderProgram);
        PopulateAttribute(shaderProgram);
#endif
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void cglTFModel::InternalLoadNode(const tinygltf::Node& node, const tinygltf::Model& model, cglTFNodeData* parentBone, std::map<const tinygltf::Node*, cglTFNodeData*>& e_tinyglTFNodeAndJointIndexMap, bool e_bCalculateBiNormal)
{
    cglTFNodeData* bone = nullptr;
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
        parentBone->AddChild(bone);
    }

    cMatrix44   l_matNodeTransform = cMatrix44::Identity;
    sSRT         l_SRT;
    //because gltf matrix is column so take trs to make it right?
    if (node.translation.size() == 3)
    {
        Vector3 translation((float)node.translation[0], (float)node.translation[1], (float)node.translation[2]);
        l_matNodeTransform *= cMatrix44::TranslationMatrix(translation);
        l_SRT.vTranslation = translation;
        l_SRT.iSRTFlag |= SRT_TRANSLATION_FLAG;
    }
    if (node.rotation.size() == 4)
    {
        Quaternion rotation((float)node.rotation[0], (float)node.rotation[1], (float)node.rotation[2], (float)node.rotation[3]);
        l_matNodeTransform *= rotation.ToMatrix();
        l_SRT.qRotation = rotation;
        l_SRT.iSRTFlag |= SRT_ROTATION_FLAG;
    }
    if (node.scale.size() == 3)
    {
        Vector3 scale((float)node.scale[0], (float)node.scale[1], (float)node.scale[2]);
        l_matNodeTransform *= cMatrix44::ScaleMatrix(scale);
        l_SRT.vScale = scale;
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
    shared_ptr<cMesh>l_pMesh = nullptr;
    if (node.skin != -1)
    {
        l_pMesh = GenerateAnimationMesh(model.skins[node.skin], model.meshes[node.mesh], model, e_bCalculateBiNormal);
    }
    else
    if (node.mesh != -1)
    {
        if (node.mesh < model.meshes.size())
        {
            l_pMesh = GenerateMesh(model.meshes[node.mesh], model, e_bCalculateBiNormal);
        }
        auto l_strMeshName = model.meshes[node.mesh].name;
        //bone->m_strTargetMeshName = l_strMeshName;
    }
    bone->m_pMesh = l_pMesh;
    //for (int childIndex : node.children)
    //{
    //    loadNode(model.nodes[childIndex], model, bone, e_tinyglTFNodeAndJointIndexMap);
    //}
}

void cglTFModel::LoadNodes(const tinygltf::Model& model, bool e_bCalculateBiNormal)
{
    std::map<int, cglTFNodeData*> &l_NodeIndexAndBoneMap = m_NodeIndexAndBoneMap;
    std::map<const tinygltf::Node*, cglTFNodeData*> l_tinyglTFNodeAndJointIndexMap;
    for (size_t i = 0; i < model.nodes.size(); i++)
    {
        const tinygltf::Node& node = model.nodes[i];
        std::wstring boneName = std::wstring(node.name.begin(), node.name.end());
        if (boneName.length() == 0)
        {
            boneName = ValueToStringW(i);
        }
        cglTFNodeData* l_pglTFNodeData = new cglTFNodeData(node, (int)i);
        bool l_bSameName = m_NodesVector.AddObject(l_pglTFNodeData);
        assert(l_bSameName && "node not allow to has same name!?");
        l_tinyglTFNodeAndJointIndexMap[&node] = l_pglTFNodeData;
        l_NodeIndexAndBoneMap[(int)i] = l_pglTFNodeData;
    }
    for (size_t i = 0; i < model.nodes.size(); i++)
    {
        const tinygltf::Node& node = model.nodes[i];
        InternalLoadNode(node, model, nullptr, l_tinyglTFNodeAndJointIndexMap, e_bCalculateBiNormal);
        auto l_pBone = l_tinyglTFNodeAndJointIndexMap[&node];
        for (auto l_iChildIndex : node.children)
        {
            auto l_pChildBone = l_tinyglTFNodeAndJointIndexMap[&model.nodes[l_iChildIndex]];
            l_pBone->AddChild(l_pChildBone);
        }
    }
}

void cglTFModel::PopulateUniform(int e_iProgram)
{
    std::map<std::string, unsigned int>  l_NameAndUniformLocationMap;
    int count = -1;
    glUseProgram(e_iProgram);
    glGetProgramiv(e_iProgram, GL_ACTIVE_UNIFORMS, &count);

    for (int i = 0; i < count; ++i)
    {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveUniform(e_iProgram, i, sizeof(name), &length, &size, &type, name);
        int uniform = glGetUniformLocation(e_iProgram, name);
        if (uniform >= 0)
        {
            std::string uniformName = name;
            std::size_t found = uniformName.find('[');
            if (found != std::string::npos)
            {
                uniformName.erase(uniformName.begin() + found, uniformName.end());
                // Populate subscripted names too
                unsigned int uniformIndex = 0;
                while (true)
                {
                    memset(name, 0, sizeof(char) * 256);
                    sprintf(name, "%s[%d]", uniformName.c_str(), uniformIndex++);
                    int uniformLocation = glGetUniformLocation(e_iProgram, name);
                    if (uniformLocation < 0)
                    {
                        break;
                    }
                    l_NameAndUniformLocationMap[name] = uniformLocation;
                }
            }
            l_NameAndUniformLocationMap[uniformName] = uniform;
        }
    }
    int a = 0;
}

void cglTFModel::PopulateAttribute(int e_iProgram)
{
    int count = -1;
    int length;
    char name[128];
    int size;
    GLenum type;
    std::map<std::string, unsigned int>  l_NameAndAttributeLocationMap;

    glUseProgram(e_iProgram);
    glGetProgramiv(e_iProgram, GL_ACTIVE_ATTRIBUTES, &count);

    for (int i = 0; i < count; ++i)
    {
        memset(name, 0, sizeof(char) * 128);
        glGetActiveAttrib(e_iProgram, (GLuint)i, 128, &length, &size, &type, name);
        int attrib = glGetAttribLocation(e_iProgram, name);
        if (attrib >= 0)
        {
            l_NameAndAttributeLocationMap[name] = attrib;
        }
    }
    int a = 0;
}


bool ExtractFloatValues(const tinygltf::Model& model, int accessorIndex, std::vector<float>& values)
{
    if (accessorIndex < 0 || accessorIndex >= model.accessors.size()) return false;
    const tinygltf::Accessor& accessor = model.accessors[accessorIndex];
    const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

    const float* data = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
    values.assign(data, data + accessor.count);
    return true;
}

void cglTFModel::loadAnimations(const tinygltf::Model& model)
{
    for (const auto& animation : model.animations)
    {
        sAnimationData* l_pAnimationData = new sAnimationData();
        sAnimationData& animationData = *l_pAnimationData;
        //animationData.m_fMinKeyTime = FLT_MAX;
        //animationData.m_fMaxKeyTime = FLT_MIN;
        animationData.m_fCurrentTime = 0.0f;
        animationData.m_fStartTime = 0.0f;
        animationData.m_fEndTime = 0.0f;

        for (const auto& channel : animation.channels)
        {
            channel.target_path;
            const auto& sampler = animation.samplers[channel.sampler];
            const auto& targetNode = model.nodes[channel.target_node];
			//"STEP" are allowd just do not do interpolation,but I am lazy to implement it.
            if (sampler.interpolation != "LINEAR")
            {
                FMLOG("animation only support lineat format");
                continue;
            }
            auto it = m_NodeIndexAndBoneMap.find(channel.target_node);
            if (it == m_NodeIndexAndBoneMap.end())
            {
                continue;
            }

            cglTFNodeData* bone = it->second;

            const auto& inputAccessor = model.accessors[sampler.input];
            const auto& inputBufferView = model.bufferViews[inputAccessor.bufferView];
            const auto& inputBuffer = model.buffers[inputBufferView.buffer];
            const float* inputData = reinterpret_cast<const float*>(inputBuffer.data.data() + inputBufferView.byteOffset + inputAccessor.byteOffset);

            const auto& outputAccessor = model.accessors[sampler.output];
            const auto& outputBufferView = model.bufferViews[outputAccessor.bufferView];
            const auto& outputBuffer = model.buffers[outputBufferView.buffer];
            const float* outputData = reinterpret_cast<const float*>(outputBuffer.data.data() + outputBufferView.byteOffset + outputAccessor.byteOffset);
            animationData.m_iTargetNodeIndex = channel.target_node;
            //morph
            if (channel.target_path == "weights")
            {
                size_t l_uiFrameCount = inputAccessor.count;
                size_t l_MorphTargetCount = outputAccessor.count / l_uiFrameCount;
                std::map<float, std::vector<float>>& l_Map = animationData.m_TimaAndMorphWeightMap;
                for (int i = 0; i < l_uiFrameCount; ++i)
                {
                    float time = inputData[i];
                    std::vector<float> l_WeightVector(l_MorphTargetCount);
                    std::memcpy(l_WeightVector.data(), outputData + i * l_MorphTargetCount, l_MorphTargetCount * sizeof(float));
                    l_Map[time] = l_WeightVector;
                }
            }
            else
            {
                for (size_t i = 0; i < inputAccessor.count; ++i)
                {
                    float time = inputData[i];
                    FloatToSRTMap& keyframes = animationData.m_BoneIDAndAnimationData[bone];
                    sSRT& srt = keyframes[time];
                    if (channel.target_path == "translation")
                    {
                        srt.vTranslation = Vector3(outputData + (i * 3));
                        srt.iSRTFlag |= SRT_TRANSLATION_FLAG; // Set translation flag
                    }
                    else if (channel.target_path == "rotation")
                    {
                        srt.qRotation = Quaternion(outputData + (i * 4));
                        //srt.rotation.Normalize();
                        srt.iSRTFlag |= SRT_ROTATION_FLAG; // Set rotation flag
                    }
                    else if (channel.target_path == "scale")
                    {
                        srt.vScale = Vector3(outputData + (i * 3));
                        srt.iSRTFlag |= SRT_SCALE_FLAG; // Set scale flag
                    }
                }
            }

            if (inputAccessor.count)
            {
                float l_fBeginTime = inputData[0];
                float l_fEndTime = inputData[inputAccessor.count-1];
                animationData.m_fMinKeyTime = min(animationData.m_fMinKeyTime, l_fBeginTime);
                animationData.m_fMaxKeyTime = max(animationData.m_fMaxKeyTime, l_fEndTime);
                animationData.m_fStartTime = animationData.m_fMinKeyTime;
                animationData.m_fEndTime = animationData.m_fMaxKeyTime;
            }
        }

        m_NameAndAnimationMap[animation.name] = l_pAnimationData;
    }
    m_AnimationClip.SetBoneAndAnimationData(this);
}

void cglTFModel::AssignMeshAttributes(cMesh* e_pMesh, const  tinygltf::Mesh& e_Mesh, const  tinygltf::Model& e_Model, bool e_bCalculateBiNormal)
{
    e_pMesh->SetName(e_Mesh.name.c_str());
    cMesh* l_pMesh = e_pMesh;
    for (const auto& primitive : e_Mesh.primitives)
    {
        l_pMesh->LoadAttributesAndInitBuffer(e_Model, primitive, e_bCalculateBiNormal);
        // Get or create the appropriate shader program for the sub-mesh
        for (auto l_pSubMesh : l_pMesh->m_SubMeshesVector)
        {
            l_pSubMesh->m_iShaderProgram = GetShaderProgram(l_pSubMesh->m_i64FVFFlag, l_pSubMesh->m_iNumMorphTarget);
            // Load textures for each material
            if (primitive.material >= 0 && primitive.material < e_Model.materials.size())
            {
                l_pMesh->LoadMaterial(e_Model, e_Model.materials[primitive.material], l_pSubMesh->m_iShaderProgram);
            }
        }
    }
    if (e_Mesh.weights.size())
    {
		l_pMesh->SetMorphingWeights(e_Mesh.weights);
    }
}

shared_ptr<cMesh> cglTFModel::GenerateMesh(const tinygltf::Mesh& e_Mesh, const tinygltf::Model& e_Model, bool e_bCalculateBiNormal)
{
    auto l_IT = m_NameAndMeshes.find(e_Mesh.name);
    if (l_IT != m_NameAndMeshes.end())
    {
        return l_IT->second;
    }
    shared_ptr<cMesh>l_pMesh = std::make_shared<cMesh>();
    AssignMeshAttributes(l_pMesh.get(), e_Mesh, e_Model, e_bCalculateBiNormal);
    m_NameAndMeshes[e_Mesh.name] = l_pMesh;
    return l_pMesh;
}

shared_ptr<cMesh> cglTFModel::GenerateAnimationMesh(const tinygltf::Skin& e_Skin, const tinygltf::Mesh& e_Mesh, const  tinygltf::Model& e_Model, bool e_bCalculateBiNormal)
{
    auto l_IT = m_AnimationMeshMap.find(e_Mesh.name);
    if (l_IT != m_AnimationMeshMap.end())
    {
        return l_IT->second;
    }
    shared_ptr<cSkinningMesh>l_pSkinningMesh = std::make_shared<cSkinningMesh>();
    AssignMeshAttributes(l_pSkinningMesh.get(), e_Mesh, e_Model, e_bCalculateBiNormal);
    l_pSkinningMesh->LoadJointsData(e_Skin,this,e_Model);
    m_AnimationMeshMap[e_Mesh.name] = l_pSkinningMesh;
    return l_pSkinningMesh;
}

GLuint cglTFModel::GetShaderProgram(int64 fvfFlags, int e_iNumMorphTarget)
{
    // Check if the shader program for this FVF already exists
    auto l_IT = m_FVFAndShaderProgramsMap.find(fvfFlags);
    if (l_IT != m_FVFAndShaderProgramsMap.end())
    {
        return l_IT->second; // Return existing shader program
    }

    // If not, create a new shader program
    GLuint shaderProgram = CreateShader(fvfFlags, e_iNumMorphTarget);
    m_FVFAndShaderProgramsMap[fvfFlags] = shaderProgram;  // Store in the map
    return shaderProgram;
}


cMatrix44 GetNodeMatrix(const tinygltf::Node& node)
{
    cMatrix44 matrix = cMatrix44::Identity;
    // Apply scale
    if (node.scale.size() == 3)
    {
        Vector3 scale((float)node.scale[0], (float)node.scale[1], (float)node.scale[2]);
        matrix *= cMatrix44::ScaleMatrix(scale);
    }
    // Apply rotation
    if (node.rotation.size() == 4)
    {

        Quaternion rotation((float)node.rotation[3], (float)node.rotation[0], (float)node.rotation[1], (float)node.rotation[2]);
        matrix *= rotation.ToMatrix();
    }
    // Apply translation
    if (node.translation.size() == 3)
    {
        Vector3 translation((float)node.translation[0], (float)node.translation[1], (float)node.translation[2]);
        matrix = cMatrix44::TranslationMatrix(translation);
    }

    // Apply matrix if present
    if (node.matrix.size() == 16)
    {
        cMatrix44 nodeMatrix = cMatrix44(node.matrix.data());
        matrix *= nodeMatrix;
    }

    return matrix;
}

bool cglTFModel::LoadFromGLTF(const std::string& filename, bool e_bCalculateBiNormal)
{
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;

    std::string err;
    std::string warn;
    bool success = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
    if (!success)
    {
        // If loading as binary fails, try loading as ASCII
        success = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    }
    if (!success)
    {
        std::cerr << "Error loading GLTF file: " << err << std::endl;
        return false;
    }
    //m_pLight->
    LoadNodes(model,e_bCalculateBiNormal);
    loadAnimations(model);
    if (cglTFLight::IsLightExists(model))
    {
        m_pLight = std::make_shared<cglTFLight>();
        m_pLight->LoadLightsFromGLTF(model);
    }
	if (cglTFCamera::IsCameraExists(model))
	{
		m_pCamera = std::make_shared<cglTFCamera>();
		m_pCamera->LoadCamerasFromGLTF(model,&this->m_NodeIndexAndBoneMap);
	}
    return true;
}

// Initialize buffers for all meshes
void cglTFModel::InitBuffers()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cglTFModel::Update(float e_fElpaseTime)
{
    if (m_NameAndAnimationMap.size())
    {
        // Ensure the current animation data is valid
        if (!this->m_AnimationClip.m_pCurrentAnimationData)
        {
            if (m_NameAndAnimationMap.size())
            {
                auto l_Animation = m_NameAndAnimationMap.begin();
                if (m_NameAndAnimationMap.size() > 1)
                {
                    //++l_Animation;
                    //++l_Animation;
                }
                this->SetCurrentAnimation(l_Animation->first);
            }
        }
        bool l_bDoBlendingTest = false;
        if (!l_bDoBlendingTest)
        {
            this->m_AnimationClip.Update(e_fElpaseTime);
        }
        else
        {
            auto l_Animation = m_NameAndAnimationMap.begin();
            std::string l_strAnimation1 = l_Animation->first;
            ++l_Animation;
            ++l_Animation;
            std::string l_strAnimation2 = (++l_Animation)->first;
            this->m_AnimationClip.BlendClips(e_fElpaseTime, "Running", l_strAnimation2.c_str(), true, true, 0.9f);
        }
    }
}

void cglTFModel::Render()
{
    cBaseShader* l_pShader = GetCurrentShader();
    if (l_pShader)
    {
        l_pShader->Unuse();
    }
    UseShaderProgram(L"qoo79979");
    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //glCullFace(GL_FRONT);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    int l_iNum = this->m_NodesVector.Count();
    for (int i = 0; i < l_iNum; ++i)
    {
        auto l_pNode = m_NodesVector[i];
        l_pNode->Update(0.016f);
        l_pNode->Render();
    }
    //for (auto& meshPair : m_NameAndMeshes)
    //{
    //    meshPair.second->Render();
    //}
    //for (auto& meshPair : m_AnimationMeshMap)
    //{
    //    meshPair.second->Update(0.016f);
    //    meshPair.second->Render();
    //}
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cglTFModel::Destory()
{
    int l_iCount = m_NodesVector.Count();
    for (int i = 0; i < l_iCount; i++)
    {
        auto l_pData = m_NodesVector[i];
        if (l_pData)
        {
            l_pData->SetParent(nullptr);
        }
    }
    m_NodesVector.Destroy();
    DELETE_MAP(m_NameAndAnimationMap);
}

void cglTFModel::SetCurrentAnimation(const std::string& e_strAnimationName)
{
    this->m_AnimationClip.SetAnimation(e_strAnimationName.c_str(), true);
}

cglTFModel* g_pglTFModel = nullptr;

int glTFInit()
{
    if(!g_pglTFModel)
    {
        g_pglTFModel = new cglTFModel();
    }
    cglTFModel& g_glTFModel = *g_pglTFModel;
    //g_glTFModel.LoadFromGLTF("glTFModel/Duck.gltf",false);
    //g_glTFModel.LoadFromGLTF("glTFModel/Lantern.gltf",true);
    // 
    //g_glTFModel.LoadFromGLTF("glTFModel/Avocado.gltf", true);
    //g_glTFModel.LoadFromGLTF("glTFModel/CesiumMilkTruck.glb", true);
    //g_glTFModel.LoadFromGLTF("glTFModel/Fox.gltf", true);
    //morphing
    //g_glTFModel.LoadFromGLTF("glTFModel/AnimatedMorphCube.glb", true);
    //g_glTFModel.LoadFromGLTF("glTFModel/CarConcept.gltf", false);
    //g_glTFModel.LoadFromGLTF("glTFModel/glTF/ABeautifulGame.gltf", true);
    
    //g_glTFModel.LoadFromGLTF("glTFModel/SimpleSkin.gltf", true);
    g_glTFModel.LoadFromGLTF("glTFModel/Woman.gltf", true);
    
    //g_glTFModel.LoadFromGLTF("glTFModel/Buggy.gltf", false);
    //g_glTFModel.LoadFromGLTF("glTFModel/AnimatedCube.gltf", false);
    //g_glTFModel.LoadFromGLTF("glTFModel/BoxAnimated.gltf", false);
    
   
    //g_glTFModel.InitBuffers();
    return 1;
}

void GlTFRender()
{
    cCameraController::GetInstance()->Update(cGameApp::m_sTimeAndFPS.fElpaseTime);
    if (g_pglTFModel)
    {
        g_pglTFModel->Update(0.016f);
        g_pglTFModel->Render();
    }
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    if (g_pglTFModel)
    {
        for (auto& meshPair : g_pglTFModel->m_AnimationMeshMap)
        {
            if (meshPair.second)
            {
                meshPair.second->RenderSkeleton();
            }
        }
    }
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GlTFDestory()
{
    //    DrawModel(model, shaderProgram);
    cTextureManager::ClearSharedTextureReferenceMap();
    SAFE_DELETE(g_pglTFModel);
}

