
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


std::map<std::string, cglTFModel*> cglTFModel::m_sNameAndglTFModelMap;
std::map<int64, GLuint>          cglTFModel::m_FVFAndShaderProgramsMap;
TYPDE_DEFINE_MARCO(cglTFModel);

class cMyTinyGLTF :public tinygltf::TinyGLTF
{
public:
    std::string GetBaseDir(const std::string& filepath)
    {
        if (filepath.find_last_of("/\\") != std::string::npos)
            return filepath.substr(0, filepath.find_last_of("/\\") + 1);
        return "";
    }
    bool LoadBinaryFromFile(tinygltf::Model* model, std::string* err,
                                      std::string* warn,
                                      const std::string& filename,
                                      unsigned int check_sections = tinygltf::SectionCheck::REQUIRE_VERSION)
    {
        std::stringstream ss;
    
        // Open file in binary mode
        NvFile* file = MyFileOpen(filename.c_str(), "rb");
        if (!file)
        {
            ss << "Failed to open file: " << filename << std::endl;
            if (err)
            {
                (*err) = ss.str();
            }
            return false;
        }
    
        // Get file size
    
        NvFSeek(file, 0, SEEK_END);
    
        long file_size = NvFTell(file);
        NvFSeek(file, 0, SEEK_SET);
    
        if (file_size <= 0)
        {
            ss << "Empty or invalid file: " << filename << std::endl;
            if (err)
            {
                (*err) = ss.str();
            }
            NvFClose(file);
            return false;
        }
    
        // Read file data
        std::vector<unsigned char> data(file_size);
        size_t read_bytes = NvFRead(data.data(), 1, file_size, file);
        if (read_bytes != static_cast<size_t>(file_size))
        {
            ss << "Failed to read file: " << filename << std::endl;
            if (err)
            {
                (*err) = ss.str();
            }
            NvFClose(file);
            return false;
        }
    
        NvFClose(file);
    
        std::string basedir = GetBaseDir(filename);
    
        bool ret = LoadBinaryFromMemory(model, err, warn, &data.at(0),
                                        static_cast<unsigned int>(data.size()),
                                        basedir, check_sections);
    
        return ret;
    }

    bool LoadASCIIFromFile(tinygltf::Model* model, std::string* err,
                                     std::string* warn, const std::string& filename,
                                    unsigned int check_sections = tinygltf::SectionCheck::REQUIRE_VERSION)
    {
        std::stringstream ss;

        // Open file in binary mode
        FILE* file = MyFileOpen(filename.c_str(), "rb");
        if (!file)
        {
            ss << "Failed to open file: " << filename << std::endl;
            if (err)
            {
                (*err) = ss.str();
            }
            return false;
        }

        // Get file size
        NvFSeek(file, 0, SEEK_END);
        long file_size = NvFTell(file);
        NvFSeek(file, 0, SEEK_SET);

        if (file_size <= 0)
        {
            ss << "Empty or invalid file: " << filename << std::endl;
            if (err)
            {
                (*err) = ss.str();
            }
            NvFClose(file);
            return false;
        }

        // Read file data
        std::vector<unsigned char> data(file_size);
        size_t read_bytes = NvFRead(data.data(), 1, file_size, file);
        if (read_bytes != static_cast<size_t>(file_size))
        {
            ss << "Failed to read file: " << filename << std::endl;
            if (err)
            {
                (*err) = ss.str();
            }
            NvFClose(file);
            return false;
        }

        NvFClose(file);

        size_t sz = data.size();
        if (sz == 0)
        {
            if (err)
            {
                (*err) = "Empty file.";
            }
            return false;
        }
        std::string basedir = GetBaseDir(filename);

        bool ret = LoadASCIIFromString(
            model, err, warn, reinterpret_cast<const char*>(&data.at(0)),
            static_cast<unsigned int>(data.size()), basedir, check_sections);

        return ret;
    }
};

//because scene will delete it
cglTFModel::cglTFModel()
{
    m_NodesVector.SetFromResource(true);
}

cglTFModel::~cglTFModel()
{
    m_sNameAndglTFModelMap.erase(this->GetCharName());;
    this->Destory();
}

std::map<std::string, sAnimationData*> cglTFModel::CloneNameAndAnimationMap(cglTFModelRenderNode* e_pglTFModelRenderNode)
{
	std::map<std::string, sAnimationData*> l_CloneMap;
    for(auto l_IT : m_NameAndAnimationMap)
    {
        sAnimationData*l_pClone = sAnimationData::CloneFromModel(e_pglTFModelRenderNode,l_IT.second);
        l_CloneMap[l_IT.first] = l_pClone;
	}
    return l_CloneMap;
}

void DumpShaderCompilerInfo(GLuint e_uiShader)
{
    GLint compiled;
    glGetShaderiv(e_uiShader, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE)
    {
        // Get the length of the log
        GLint logLength = 0;
        glGetShaderiv(e_uiShader, GL_INFO_LOG_LENGTH, &logLength);

        if (logLength > 0)
        {
            std::vector<GLchar> log(logLength);
            glGetShaderInfoLog(e_uiShader, logLength, nullptr, log.data());
            printf("Shader compile error:\n%s\n", log.data());
        }
        else
        {
            printf("Shader compile failed but no log available.\n");
        }
    }
    else
    {
        printf("Shader compile ok\n");
    }
}

GLuint cglTFModel::CreateShader(int64 fvfFlags, int e_iNumMorphTarget)
{
    std::string vertexCode = GenerateVertexShaderWithFVF(fvfFlags, e_iNumMorphTarget);
	//printf("vertex shader code:\n%s", vertexCode.c_str());
    //std::string fragmentCode = GenerateFragmentShaderWithFVF(fvfFlags);
    //printf("fs shader code:\n%s", fragmentCode.c_str());
    // Fragment Shader source (GLSL ES 3.00)
    std::string fragmentCode = R"(#version 300 es
            precision mediump float;
            out vec4 FragColor;
            void main() {
                FragColor = vec4(1.0, 0.0, 0.0, 1.0);
            }
        )";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vShaderCode = vertexCode.c_str();
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    LAZY_DO_GL_COMMAND_AND_GET_ERROR(glCompileShader(vertexShader));
    DumpShaderCompilerInfo(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fShaderCode = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    LAZY_DO_GL_COMMAND_AND_GET_ERROR(glCompileShader(fragmentShader));
    DumpShaderCompilerInfo(fragmentShader);

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
//#ifdef DEBUG
        PopulateUniform(shaderProgram);
        PopulateAttribute(shaderProgram);
//#endif
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void cglTFModel::InternalLoadNode(const tinygltf::Node& e_pNode, const tinygltf::Model& model, cglTFNodeData* parentBone, std::map<const tinygltf::Node*, cglTFNodeData*>& e_tinyglTFNodeAndJointIndexMap, bool e_bCalculateBiNormal)
{
    cglTFNodeData* l_pBone = nullptr;
    auto l_IT2 = e_tinyglTFNodeAndJointIndexMap.find(&e_pNode);
    if (l_IT2 != e_tinyglTFNodeAndJointIndexMap.end())
    {
        l_pBone = l_IT2->second;
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
            l_pBone->m_iJointIndex == 4)
        {
            //for debug
            int a = 0;
        }
#endif
        parentBone->AddChild(l_pBone);
    }

    cMatrix44   l_matNodeTransform = cMatrix44::Identity;
    sSRT         l_SRT;
    //because gltf matrix is column so take trs to make it right?
    if (e_pNode.translation.size() == 3)
    {
        Vector3 translation((float)e_pNode.translation[0], (float)e_pNode.translation[1], (float)e_pNode.translation[2]);
        l_matNodeTransform *= cMatrix44::TranslationMatrix(translation);
        l_SRT.vTranslation = translation;
        l_SRT.iSRTFlag |= SRT_TRANSLATION_FLAG;
    }
    if (e_pNode.rotation.size() == 4)
    {
        Quaternion rotation((float)e_pNode.rotation[0], (float)e_pNode.rotation[1], (float)e_pNode.rotation[2], (float)e_pNode.rotation[3]);
        l_matNodeTransform *= rotation.ToMatrix();
        l_SRT.qRotation = rotation;
        l_SRT.iSRTFlag |= SRT_ROTATION_FLAG;
    }
    if (e_pNode.scale.size() == 3)
    {
        Vector3 scale((float)e_pNode.scale[0], (float)e_pNode.scale[1], (float)e_pNode.scale[2]);
        l_matNodeTransform *= cMatrix44::ScaleMatrix(scale);
        l_SRT.vScale = scale;
        l_SRT.iSRTFlag |= SRT_SCALE_FLAG;
    }
    if (e_pNode.matrix.size() == 16)
    {
        cMatrix44 nodeMatrix = cMatrix44(e_pNode.matrix.data());
        l_matNodeTransform = nodeMatrix;
    }
    //11
    l_pBone->m_StartNodeTransform = l_matNodeTransform;
    l_pBone->m_StartSRT = l_SRT;
    l_pBone->SetLocalTransform(l_matNodeTransform);
    cMesh*l_pMesh = nullptr;
    if (e_pNode.skin != -1)
    {
        l_pMesh = GenerateAnimationMesh(model.skins[e_pNode.skin], e_pNode, model.meshes[e_pNode.mesh], model, e_bCalculateBiNormal);
    }
    else
    if (e_pNode.mesh != -1)
    {
        if (e_pNode.mesh < model.meshes.size())
        {
            l_pMesh = GenerateMesh(e_pNode,model.meshes[e_pNode.mesh], model, e_bCalculateBiNormal);
        }
        //auto l_strMeshName = model.meshes[e_pNode.mesh].name;
        ////bone->m_strTargetMeshName = l_strMeshName;
    }
    l_pBone->SetMesh(l_pMesh,e_pNode,model, m_iInstanceValue);
    auto l_pMeshInstance = l_pBone->GetMeshInstance();
    if (l_pMeshInstance)
    {
        if (l_pMesh->Type() == cSkinningMesh::TypeID)
        {
            std::shared_ptr<cAnimationInstanceManager>l_spAnimationInstanceManager = std::make_shared<cAnimationInstanceManager>(&this->m_AnimationClip, l_pMeshInstance, l_pMesh->GetSubMeshShaderProgramID(), (cSkinningMesh*)l_pMesh);
            m_sAnimationInstanceManagerVector.push_back(l_spAnimationInstanceManager);
        }
    }
    
    if (l_pMesh)
    {
        m_ContainMeshglTFNodeDataVector.push_back(l_pBone);
    }
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
        cglTFNodeData*l_pglTFNodeData = new cglTFNodeData(node, (int)i);
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
    // Assume 'model' is a tinygltf::Model
    int sceneIndex = model.defaultScene; // or model.scene (older versions)
    if (sceneIndex < 0 || sceneIndex >= model.scenes.size())
    {
        // fallback: use scene 0
        sceneIndex = 0;
    }
    const tinygltf::Scene& scene = model.scenes[sceneIndex];

    // scene.nodes is a vector of root node indices
    for (int l_iNodeIndex : scene.nodes)
    {
        auto l_IT = m_NodeIndexAndBoneMap.find(l_iNodeIndex);
        if (!l_IT->second->GetParent())
        {
            l_IT->second->SetParent(this);
        }
    }
}

void cglTFModel::PopulateUniform(int e_iProgram)
{
    std::map<std::string, unsigned int>  l_NameAndUniformLocationMap;
    int count = -1;
    glUseProgram(e_iProgram);
    glGetProgramiv(e_iProgram, GL_ACTIVE_UNIFORMS, &count);
    FMLOG("uniform count %d", count);
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
            l_NameAndUniformLocationMap[name] = uniform;
            FMLOG("%s,%d", name, uniform);
        }
        else
        {
            FMLOG("glGetUniformLocation failed  %s,%d", name, uniform);
        }
    }
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
    FMLOG("vertex attribute count %d", count);
    for (int i = 0; i < count; ++i)
    {
        memset(name, 0, sizeof(char) * 128);
        glGetActiveAttrib(e_iProgram, (GLuint)i, 128, &length, &size, &type, name);
        int attrib = glGetAttribLocation(e_iProgram, name);
        if (attrib >= 0)
        {
            l_NameAndAttributeLocationMap[name] = attrib;
            FMLOG("%s,%d", name, attrib);
        }
    }
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
        l_pAnimationData->m_pBoneVector = &this->m_NodesVector;
        //l_pAnimationData->m_fMinKeyTime = FLT_MAX;
        //l_pAnimationData->m_fMaxKeyTime = FLT_MIN;
        l_pAnimationData->m_fCurrentTime = 0.0f;
        l_pAnimationData->m_fStartTime = 0.0f;
        l_pAnimationData->m_fEndTime = 0.0f;

        for (const auto& channel : animation.channels)
        {
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
            l_pAnimationData->m_iTargetNodeIndex = channel.target_node;
            //morph
            if (channel.target_path == "weights")
            {
                size_t l_uiFrameCount = inputAccessor.count;
                size_t l_MorphTargetCount = outputAccessor.count / l_uiFrameCount;
                std::map<float, std::vector<float>>& l_Map = l_pAnimationData->m_TimaAndMorphWeightMap;
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
                    FloatToSRTMap& keyframes = (*l_pAnimationData->m_BoneIDAndAnimationData)[bone->m_iNodeIndex];
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
                    else if (channel.target_path == "matrix")
                    {
                        int a = 0;
                    }
                }
            }

            if (inputAccessor.count)
            {
                float l_fBeginTime = inputData[0];
                float l_fEndTime = inputData[inputAccessor.count-1];
                l_pAnimationData->m_fMinKeyTime = min(l_pAnimationData->m_fMinKeyTime, l_fBeginTime);
                l_pAnimationData->m_fMaxKeyTime = max(l_pAnimationData->m_fMaxKeyTime, l_fEndTime);
                l_pAnimationData->m_fStartTime = l_pAnimationData->m_fMinKeyTime;
                l_pAnimationData->m_fEndTime = l_pAnimationData->m_fMaxKeyTime;
            }
        }

        m_NameAndAnimationMap[animation.name] = l_pAnimationData;
    }
    m_AnimationClip.SetName(this->GetName());
    m_AnimationClip.SetBoneAndAnimationData(this);
}

void cglTFModel::AssignMeshAttributes(cMesh* e_pMesh, const tinygltf::Node& node, const  tinygltf::Mesh& e_Mesh, const  tinygltf::Model& e_Model, bool e_bCalculateBiNormal)
{
    e_pMesh->SetName(e_Mesh.name.c_str());
    cMesh* l_pMesh = e_pMesh;
    for (const auto& primitive : e_Mesh.primitives)
    {
        l_pMesh->LoadAttributesAndInitBuffer(e_Model, primitive, e_bCalculateBiNormal);
    }
    // Get or create the appropriate shader program for the sub-mesh
    int l_iIndex = 0;
    for (const auto& primitive : e_Mesh.primitives)
    {
        auto l_pSubMesh = l_pMesh->m_SubMeshesVector[l_iIndex];
        ++l_iIndex;
        // Load textures for each material
        shared_ptr<cMaterial>l_pMaterial;
        if (primitive.material >= 0 && primitive.material < e_Model.materials.size())
        {
            l_pMaterial = l_pMesh->LoadMaterial(e_Model, e_Model.materials[primitive.material], l_pSubMesh);
        }
        int64 l_i64TextureFlag = 0;
        if (l_pMaterial)
        {
            l_i64TextureFlag = l_pMaterial->GetTextureFVFFlag();
        }
        if(cglTFNodeData::ContainInstanceExtension(node) || m_iInstanceValue > 0)
        {
            l_pSubMesh->m_i64FVFFlag |= FVF_INSTANCING_FLAG;
            if (l_pMesh->Type() == cSkinningMesh::TypeID)
            {
                l_pSubMesh->m_i64FVFFlag |= FVF_ANIMATION_TEXTURE_FLAG;
            }
        }
        l_pSubMesh->m_iShaderProgramID = GetShaderProgram(l_pSubMesh->m_i64FVFFlag, l_i64TextureFlag, l_pSubMesh->m_iNumMorphTarget);
        if (l_pMaterial)
        {
            l_pMaterial->SetShaderProgramID(l_pSubMesh->m_iShaderProgramID);
        }
    }    
}

cMesh* cglTFModel::GenerateMesh(const tinygltf::Node& node,const tinygltf::Mesh& e_Mesh, const tinygltf::Model& e_Model, bool e_bCalculateBiNormal)
{
    auto l_IT = m_NameAndMeshes.find(e_Mesh.name);
    if (l_IT != m_NameAndMeshes.end())
    {
        return l_IT->second;
    }
    cMesh*l_pMesh = new cMesh();
    l_pMesh->SetName(e_Mesh.name.c_str());
    AssignMeshAttributes(l_pMesh, node,e_Mesh, e_Model, e_bCalculateBiNormal);
    m_NameAndMeshes[e_Mesh.name] = l_pMesh;
    return l_pMesh;
}

cMesh* cglTFModel::GenerateAnimationMesh(const tinygltf::Skin& e_Skin, const tinygltf::Node& node, const tinygltf::Mesh& e_Mesh, const  tinygltf::Model& e_Model, bool e_bCalculateBiNormal)
{
    auto l_IT = m_AnimationMeshMap.find(e_Mesh.name);
    if (l_IT != m_AnimationMeshMap.end())
    {
        return l_IT->second;
    }
    cSkinningMesh*l_pSkinningMesh = new cSkinningMesh();
    l_pSkinningMesh->SetName(e_Mesh.name.c_str());
    AssignMeshAttributes(l_pSkinningMesh, node, e_Mesh, e_Model, e_bCalculateBiNormal);
    l_pSkinningMesh->LoadJointsData(e_Skin,this,e_Model);
    m_AnimationMeshMap[e_Mesh.name] = l_pSkinningMesh;
    return l_pSkinningMesh;
}

GLuint cglTFModel::GetShaderProgram(int64 e_i64FVFFlags,int64 e_i64TextureFVF, int e_iNumMorphTarget)
{
    int64 l_i64FinalFVFFlags = e_i64FVFFlags | e_i64TextureFVF;
    //|= 1LL << l_iFVFIndex
    // Check if the shader program for this FVF already exists
    auto l_IT = m_FVFAndShaderProgramsMap.find(l_i64FinalFVFFlags);
    if (l_IT != m_FVFAndShaderProgramsMap.end())
    {
        return l_IT->second; // Return existing shader program
    }

    // If not, create a new shader program
    GLuint shaderProgram = CreateShader(l_i64FinalFVFFlags, e_iNumMorphTarget);
    m_FVFAndShaderProgramsMap[l_i64FinalFVFFlags] = shaderProgram;  // Store in the map
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

bool cglTFModel::LoadFromGLTF(const std::string& e_strFilename, bool e_bCalculateBiNormal,int e_iInstanceValue)
{
    this->SetName(e_strFilename.c_str());
	m_iInstanceValue = e_iInstanceValue;
    cMyTinyGLTF loader;
    tinygltf::Model model;

    std::string err;
    std::string warn;
    bool success = loader.LoadBinaryFromFile(&model, &err, &warn, e_strFilename);
    if (!success)
    {
        // If loading as binary fails, try loading as ASCII
        success = loader.LoadASCIIFromFile(&model, &err, &warn, e_strFilename);
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
	m_sNameAndglTFModelMap.insert(std::make_pair(e_strFilename, this));
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
                    ++l_Animation;
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
}

void cglTFModel::Destory()
{
    DELETE_MAP(m_NameAndMeshes);
    DELETE_MAP(m_NameAndAnimationMap);
    DELETE_MAP(m_AnimationMeshMap);
}

void cglTFModel::SetCurrentAnimation(const std::string& e_strAnimationName)
{
    this->m_AnimationClip.SetAnimation(e_strAnimationName.c_str(), true);
}

void cglTFModelRenderNode::SetCurrentAnimationTime(float e_fCurrentTime)
{
    this->m_AnimationClip.UpdateToTargetTime(e_fCurrentTime,true);
}

cglTFModelRenderNode* cglTFModel::ToRenderNode()
{
    //std::map<std::string, sAnimationData*>                  m_NameAndAnimationMap;
    auto* l_pRenderNode = new cglTFModelRenderNode();
	l_pRenderNode->m_NodesVector.CloneFromList(&m_NodesVector);
	l_pRenderNode->m_NodesVector.SetFromResource(true);
    //fuck different model use same resource.
    l_pRenderNode->m_NameAndAnimationMap = CloneNameAndAnimationMap(l_pRenderNode);
    l_pRenderNode->m_pSourceglTFModel = this;
    // Copy contain mesh node vector
    for (auto l_pNode : m_ContainMeshglTFNodeDataVector)
    {
        auto l_mat = l_pNode->GetWorldTransform();
        auto l_pSrcMesh = l_pNode->GetMesh();
        if (l_pSrcMesh)
        {
            cMesh*l_pMesh = nullptr;
            auto l_IT = l_pRenderNode->m_NameAndMeshes.find(l_pSrcMesh->GetCharName());
            if (l_IT == l_pRenderNode->m_NameAndMeshes.end())
            {//clone a new one
                l_pMesh = l_pSrcMesh->GetTypeClone();
                l_pRenderNode->m_NameAndMeshes[l_pSrcMesh->GetCharName()] = l_pMesh;
            }
            else
            {
                l_pMesh = l_IT->second;
            }
            auto l_pContainMeshNode = l_pRenderNode->m_NodesVector[l_pNode->m_iNodeIndex];
            auto l_spMeshInstance = l_pNode->GetMeshInstance();
			l_pContainMeshNode->SetMesh(l_pMesh, l_spMeshInstance);
            if (l_spMeshInstance)
            {
                if (l_pMesh->Type() == cSkinningMesh::TypeID)
                {
                    std::shared_ptr<cAnimationInstanceManager>l_spAnimationInstanceManager = std::make_shared<cAnimationInstanceManager>(&this->m_AnimationClip, l_spMeshInstance, l_pMesh->GetSubMeshShaderProgramID(), (cSkinningMesh*)l_pMesh);
                    m_sAnimationInstanceManagerVector.push_back(l_spAnimationInstanceManager);
                }
            }
            l_pRenderNode->m_ContainMeshglTFNodeDataVector.push_back(l_pRenderNode->m_NodesVector[l_pNode->m_iNodeIndex]);
            if (l_pMesh)
			{//
                l_pMesh->AfterCloneSetBoneData(l_pRenderNode);
            }
        }
    }
    // Set hierarchy for l_pRenderNode->m_NodesVector based on m_NodesVector
    for (int i = 0; i < m_NodesVector.Count(); ++i)
    {
        cglTFNodeData* srcNode = m_NodesVector[i];
        cglTFNodeData* srcParent = dynamic_cast<cglTFNodeData*>(srcNode->GetParent());
        l_pRenderNode->m_NodeIndexAndBoneMap[srcNode->m_iNodeIndex] = l_pRenderNode->m_NodesVector[srcNode->m_iNodeIndex];
        if (srcParent)
        {
            int parentIndex = srcParent->m_iNodeIndex;
            cglTFNodeData* dstNode = l_pRenderNode->m_NodesVector[i];
            cglTFNodeData* dstParent = l_pRenderNode->m_NodesVector[parentIndex];
            dstNode->SetParent(dstParent,false);
            auto l_mat = l_pRenderNode->m_NodesVector[i]->GetWorldTransform();
            int a = 0;
        }
        else
        {
            // If no parent, clear parent in the render node as well
            l_pRenderNode->m_NodesVector[i]->SetParent(l_pRenderNode);
        }
    }
    l_pRenderNode->m_AnimationClip.SetName(m_AnimationClip.GetName());
    l_pRenderNode->m_AnimationClip.SetBoneAndAnimationData(l_pRenderNode);
    return l_pRenderNode;
}

void cglTFModel::DeleteCachedFiles()
{
    auto l_Map = cglTFModel::m_sNameAndglTFModelMap;
    for (auto l_IT : l_Map)
    {
		Frame::DestoryWithChildren(l_IT.second);
    }
	cglTFModel::m_sNameAndglTFModelMap.clear();
}


void cglTFModelRenderNode::Update(float e_fElpaseTime)
{
    if (m_NameAndAnimationMap.size())
    {
        // Ensure the current animation data is valid
        if (!this->m_AnimationClip.m_pCurrentAnimationData)
        {
            if (m_NameAndAnimationMap.size())
            {
                int l_iIndex = 0;
				l_iIndex = rand() % m_NameAndAnimationMap.size();
                auto l_Animation = m_NameAndAnimationMap.begin();
				std::advance(l_Animation, l_iIndex);
                //l_Animation += l_iIndex;
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

void cglTFModelRenderNode::Render()
{

}

void cglTFModelRenderNode::SetCurrentAnimation(const std::string& e_strAnimationName)
{
    this->m_AnimationClip.SetAnimation(e_strAnimationName.c_str(), true);
    //cAnimTexture l_cAnimTexture(this->m_AnimationClip, e_strAnimationName.c_str());
    int a = 0;
}

void g_fRenderSkeleton(std::map<std::string, cSkinningMesh*>& e_Map)
{
    for (auto& meshPair : e_Map)
    {
        if (meshPair.second)
        {
            meshPair.second->RenderSkeleton();
        }
    }
}
