
#include "../AllCoreInclude.h"
#include <vector>
#include <string>
#include <map>
#include "tiny_gltf.h"
#include <iostream>
#include <sstream>
#include "glTFModel.h"

TYPDE_DEFINE_MARCO(cglTFModel);

cglTFModel::~cglTFModel()
{
    DELETE_MAP(m_NameAndAnimationMap)
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
}

std::string cglTFModel::GenerateVertexShader(unsigned int fvfFlags)
{
    std::string shaderCode = R"(
        #version 330 core
        #ifdef GL_ES
        precision mediump float;
        #endif

        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec3 aNormal;
        layout(location = 7) in vec2 aTexCoord;
    )";

    // Adding tangents if necessary
    if (fvfFlags & FVF_TANGENT_FLAG)
    {
        shaderCode += R"(
        layout(location = 3) in vec3 aTangent;)";
    }

    // Adding binormals if necessary
    if (fvfFlags & FVF_BITAGENT_FLAG)
    {
        shaderCode += R"(
        layout(location = 4) in vec3 aBinormal;)";
    }

    // Adding joint indices and weights if necessary
    if (fvfFlags & FVF_SKINNING_WEIGHT_FLAG)
    {
        shaderCode += R"(
        layout(location = 5) in vec4 aWeights;)";
    }

    if (fvfFlags & FVF_SKINNING_BONE_INDEX_FLAG)
    {
        shaderCode += R"(
        layout(location = 6) in ivec4 aJoints;)";
    }

    // Normalizing the vectors and transforming to view space
    shaderCode += R"(
        uniform mat4 inMat4Model;
        uniform mat4 inMat4View;
        uniform mat4 inMat4Projection;
        uniform vec3 inVec3LightPosition;

        out vec2 toFSVec2TexCoord;
        out vec3 toFSVec3Normal;
        out vec3 toFSVec3FragPos;
        out vec3 toFSVec3LightDir;
    )";

    if (fvfFlags & FVF_TANGENT_FLAG)
    {
        shaderCode += R"(
        out vec3 toFSVec3Tangent;)";
    }

    if (fvfFlags & FVF_BITAGENT_FLAG)
    {
        shaderCode += R"(
        out vec3 toFSVec3Binormal;)";
    }

    shaderCode += R"(
        const int MAX_BONES = 100;
        uniform mat4 uBoneTransforms[MAX_BONES];
    )";

    shaderCode += R"(
        void main() 
        {
            vec4 worldPosition = inMat4Model * vec4(aPosition, 1.0);
            toFSVec3FragPos = worldPosition.xyz;
    )";

    if ((fvfFlags & FVF_SKINNING_WEIGHT_FLAG) && (fvfFlags & FVF_SKINNING_BONE_INDEX_FLAG))
    {
        shaderCode += R"(
            // Skinning transformation
            mat4 skinMatrix = aWeights.x * uBoneTransforms[aJoints.x] +
                              aWeights.y * uBoneTransforms[aJoints.y] +
                              aWeights.z * uBoneTransforms[aJoints.z] +
                              aWeights.w * uBoneTransforms[aJoints.w];
            vec4 skinnedPosition = skinMatrix * vec4(aPosition, 1.0);;
            worldPosition = inMat4Projection*inMat4View*inMat4Model* skinnedPosition;

        )";
    }

    shaderCode += R"(
            // Passing light direction to fragment shader
            toFSVec3LightDir = normalize(inVec3LightPosition - toFSVec3FragPos);
    )";


    if (fvfFlags & FVF_TANGENT_FLAG)
    {
        shaderCode += R"(
            // Transform tangent if present
            toFSVec3Tangent = mat3(transpose(inverse(inMat4Model))) * aTangent;
        )";
    }

    if (fvfFlags & FVF_BITAGENT_FLAG)
    {
        shaderCode += R"(
            // Transform binormal if present
            toFSVec3Binormal = mat3(transpose(inverse(inMat4Model))) * aBinormal;
        )";
    }

    shaderCode += R"(
            // Pass texture coordinates
            toFSVec2TexCoord = aTexCoord;
        )";

    if ((fvfFlags & FVF_SKINNING_WEIGHT_FLAG) && (fvfFlags & FVF_SKINNING_BONE_INDEX_FLAG))
    {
        shaderCode += R"(    gl_Position = worldPosition;
        )";
    }
    else
    {
        shaderCode += R"(
            gl_Position = inMat4Projection * inMat4View * worldPosition;
            // Transform normal to world space and then to view space
            toFSVec3Normal = mat3(transpose(inverse(inMat4Model))) * aNormal;
        )";
    }
    shaderCode += R"(
        })";
#ifdef DEBUG
    FMLOG(shaderCode.c_str());
#endif
    return shaderCode;
}


std::string cglTFModel::GenerateFragmentShader(unsigned int fvfFlags)
{
    std::string shaderCode = R"(
        #version 330 core
        #ifdef GL_ES
        precision mediump float;
        #endif

        out vec4 FragColor;
        in vec2 toFSVec2TexCoord;
        in vec3 toFSVec3Normal;
        in vec3 toFSVec3FragPos;
        in vec3 toFSVec3LightDir;)";
    if (fvfFlags & FVF_TANGENT_FLAG)
    {
        shaderCode += R"(
        in vec3 toFSVec3Tangent;)";
    }

    if (fvfFlags & FVF_BITAGENT_FLAG)
    {
        shaderCode += R"(
        in vec3 toFSVec3Binormal;)";
    }
    shaderCode += R"(
        // Declare the uniform for fvfFlags passed from C++ 
        uniform uint fvfFlags;

        // Define constants for flags, from shader.h
        const uint FVF_TANGENT_FLAG = 1u << 4;
        const uint FVF_BITAGENT_FLAG = 1u << 5;
        const uint FVF_NORMAL_FLAG = 1u << 1;  // Flag for normal
        const uint FVF_NORMAL_MAP = 1u << 6;  // Flag for normal map

        // Add normal map if necessary
        uniform sampler2D normalMap;
        uniform sampler2D texture1; // Base color texture
        uniform sampler2D textureRoughness; // Roughness texture
        uniform sampler2D textureMetallic; // Metallic texture

        // Light properties
        uniform vec3 inVec3LightColor;
        uniform vec3 inVec3LightPosition;
        uniform vec3 inVec3ViewPosition;

        // Directional light properties
        uniform vec3 dirLightDirection;
        uniform vec3 dirLightColor;

        // Phong lighting model
        vec3 ComputeLighting(vec3 normal, vec3 lightDir, vec3 viewDir) 
        {
            vec3 ambient = 0.1 * inVec3LightColor;  // Ambient lighting

            // Diffuse shading (Lambert's cosine law)
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * inVec3LightColor;

            // Specular reflection (Blinn-Phong)
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);  // Hardcoded shininess
            vec3 specular = spec * inVec3LightColor;

            return ambient + diffuse + specular;  // Returning the combined light contribution
        }

        // Directional light calculation
        vec3 ComputeDirectionalLight(vec3 normal, vec3 viewDir) 
        {
            vec3 ambient = 0.1 * dirLightColor;  // Ambient lighting

            // Diffuse shading (Lambert's cosine law)
            float diff = max(dot(normal, -dirLightDirection), 0.0);
            vec3 diffuse = diff * dirLightColor;

            // Specular reflection (Blinn-Phong)
            vec3 reflectDir = reflect(dirLightDirection, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);  // Hardcoded shininess
            vec3 specular = spec * dirLightColor;

            return ambient + diffuse + specular;  // Returning the combined light contribution
        }
    )";

    shaderCode += R"(
        void main() 
        {
            // Fetch base color texture
            vec3 color = texture(texture1, toFSVec2TexCoord).rgb;
    )";

    if (fvfFlags & FVF_NORMAL_FLAG)
    {
        shaderCode += R"(
            // Normal map processing (if flag is set)
            vec3 normal = normalize(toFSVec3Normal);
            if (bool(fvfFlags & FVF_NORMAL_MAP)) 
            {  // Check if FVF_NORMAL_MAP bit is set
                vec3 normalMapColor = texture(normalMap, toFSVec2TexCoord).rgb;
                normalMapColor = normalize(normalMapColor * 2.0 - 1.0);  // Convert from [0, 1] to [-1, 1]

                        )";
        if (fvfFlags & FVF_BITAGENT_FLAG)
        {
            shaderCode += R"(
                // Transform normal from tangent space to world space
                mat3 TBN = mat3(normalize(toFSVec3Tangent), normalize(toFSVec3Binormal), normalize(toFSVec3Normal));
                normal = normalize(TBN * normalMapColor);
            )";
        }
        shaderCode += R"(
            }
            // Calculate the view direction
            vec3 viewDir = normalize(inVec3ViewPosition - toFSVec3FragPos);

            // Calculate lighting
            vec3 lighting = ComputeLighting(normal, toFSVec3LightDir, viewDir);

            // Calculate directional lighting
            vec3 dirLighting = ComputeDirectionalLight(normal, viewDir);

            // Apply texture and lighting effects
            FragColor = vec4(color * (lighting + dirLighting), 1.0);
        )";
    }
    else
    {
        shaderCode += R"(
            // Skip lighting calculations and directly set FragColor
            FragColor = vec4(color, 1.0);
        )";
    }

    if ((fvfFlags & FVF_TEX0_FLAG)==0)
    {
        shaderCode += R"(
            FragColor = vec4(1, 1, 0, 1.0);)";
    }
    shaderCode += R"(
        })";
#ifdef DEBUG
    FMLOG(shaderCode.c_str());
#endif
    return shaderCode;
}







GLuint cglTFModel::CreateShader(unsigned int fvfFlags)
{
    std::string vertexCode = GenerateVertexShader(fvfFlags);
    std::string fragmentCode = GenerateFragmentShader(fvfFlags);
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

void cglTFModel::InternalLoadNode(const tinygltf::Node& node, const tinygltf::Model& model, cglTFNodeData* parentBone, std::map<const tinygltf::Node*, cglTFNodeData*>& e_tinyglTFNodeAndJointIndexMap)
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
        //parentBone->AddChildToLast(bone);
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
    //if (node.scale.size() == 3)
    //{
    //    Vector3 scale((float)node.scale[0], (float)node.scale[1], (float)node.scale[2]);
    //    l_matNodeTransform *= cMatrix44::ScaleMatrix(scale);
    //    l_SRT.scale = scale;
    //    l_SRT.iSRTFlag |= SRT_SCALE_FLAG;
    //}
    //if (node.rotation.size() == 4)
    //{
    //    Quaternion rotation((float)node.rotation[0], (float)node.rotation[1], (float)node.rotation[2], (float)node.rotation[3]);
    //    l_matNodeTransform *= rotation.ToMatrix();
    //    l_SRT.rotation = rotation;
    //    l_SRT.iSRTFlag |= SRT_ROTATION_FLAG;
    //}
    //if (node.translation.size() == 3)
    //{
    //    Vector3 translation((float)node.translation[0], (float)node.translation[1], (float)node.translation[2]);
    //    l_matNodeTransform *= cMatrix44::TranslationMatrix(translation);
    //    l_SRT.translation = translation;
    //    l_SRT.iSRTFlag |= SRT_TRANSLATION_FLAG;
    //}
    if (node.matrix.size() == 16)
    {
        cMatrix44 nodeMatrix = cMatrix44(node.matrix.data());
        l_matNodeTransform = nodeMatrix;
    }
    //11
    bone->m_StartNodeTransform = l_matNodeTransform;
    bone->m_StartSRT = l_SRT;
    bone->SetLocalTransform(l_matNodeTransform);

    //for (int childIndex : node.children)
    //{
    //    loadNode(model.nodes[childIndex], model, bone, e_tinyglTFNodeAndJointIndexMap);
    //}
}

void cglTFModel::LoadNodes(const tinygltf::Model& model, bool e_bCalculateBiNormal)
{
    std::map<int, cglTFNodeData*> &l_NodeIndexAndBoneMap = m_NodeIndexAndBoneMap;
    std::map<const tinygltf::Node*, cglTFNodeData*> l_tinyglTFNodeAndJointIndexMap;
    //first generate all bones from all nodes
    for (size_t i = 0; i < model.nodes.size(); i++)
    {
        const tinygltf::Node& node = model.nodes[i];
        std::wstring boneName = std::wstring(node.name.begin(), node.name.end());
        if (boneName.length() == 0)
        {
            boneName = ValueToStringW(i);
        }
        cglTFNodeData* bone = new cglTFNodeData(node, (int)i);
        cMesh* l_pMesh = nullptr;
        if (node.skin != -1)
        {
            cMesh*l_pMesh = new cSkinningMesh();
            if (node.skin < model.skins.size())
            {
                //model.skins[node.skin];
            }
        }
        if (node.mesh != -1)
        {
            if (node.mesh < model.meshes.size())
            {
                l_pMesh = GenerateMesh(model.meshes[node.mesh], model, e_bCalculateBiNormal);
            }
            auto l_strMeshName = model.meshes[node.mesh].name;
            //bone->m_strTargetMeshName = l_strMeshName;
        }
        bool l_bSameName = m_NodesVector.AddObject(bone);
        assert(l_bSameName && "node not allow to has same name!?");
        l_tinyglTFNodeAndJointIndexMap[&node] = bone;
        l_NodeIndexAndBoneMap[(int)i] = bone;
    }
    for (size_t i = 0; i < model.nodes.size(); i++)
    {
        const tinygltf::Node& node = model.nodes[i];
        InternalLoadNode(node, model, nullptr, l_tinyglTFNodeAndJointIndexMap);
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
    int count = -1;
    int length;
    char name[128];
    int size;
    GLenum type;
    char testName[256];
    std::map<std::string, unsigned int>  l_NameAndUniformLocationMap;

    glUseProgram(e_iProgram);
    glGetProgramiv(e_iProgram, GL_ACTIVE_UNIFORMS, &count);

    for (int i = 0; i < count; ++i)
    {
        memset(name, 0, sizeof(char) * 128);
        glGetActiveUniform(e_iProgram, (GLuint)i, 128, &length, &size, &type, name);

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
                    memset(testName, 0, sizeof(char) * 256);
                    sprintf(testName, "%s[%d]", uniformName.c_str(), uniformIndex++);
                    int uniformLocation = glGetUniformLocation(e_iProgram, testName);
                    if (uniformLocation < 0)
                    {
                        break;
                    }
                    l_NameAndUniformLocationMap[testName] = uniformLocation;
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
            const auto& sampler = animation.samplers[channel.sampler];
            const auto& targetNode = model.nodes[channel.target_node];
            if (sampler.interpolation != "LINEAR")
            {
                FMLOG("animation only support lineat format")
                    continue;
            }
            auto it = m_NodeIndexAndBoneMap.find(channel.target_node);
            if (it == m_NodeIndexAndBoneMap.end())
            {
                continue;
            }

            cglTFNodeData* bone = it->second;
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

void cglTFModel::loadSkins(const tinygltf::Model& model)
{
    int l_iCount = this->m_NodesVector.Count();
    for (int i = 0; i < l_iCount;++i)
    {

    }
    for (const auto& skin : model.skins)
    {
        //skin.joints
        
    }
}

void cglTFModel::AssignMeshAttributes(cMesh* e_pMesh, const  tinygltf::Mesh& e_Mesh, const  tinygltf::Model& e_Model, bool e_bCalculateBiNormal)
{
    const auto& meshPair = e_Mesh;
    cMesh* l_pMesh = e_pMesh;
    for (const auto& primitive : meshPair.primitives)
    {
        l_pMesh->LoadAttributesAndInitBuffer(e_Model, primitive, e_bCalculateBiNormal);
        // Get or create the appropriate shader program for the sub-mesh
        for (auto l_pSubMesh : l_pMesh->m_SubMeshesVector)
        {
            l_pSubMesh->shaderProgram = GetShaderProgram(l_pSubMesh->m_iFVFFlag);
            // Load textures for each material
            if (primitive.material >= 0 && primitive.material < e_Model.materials.size())
            {
                l_pMesh->LoadMaterial(e_Model, e_Model.materials[primitive.material], l_pSubMesh->shaderProgram);
            }
        }
    }
}

cMesh* cglTFModel::GenerateMesh(const tinygltf::Mesh& e_Mesh, const tinygltf::Model& e_Model, bool e_bCalculateBiNormal)
{
    const auto& meshPair = e_Mesh;
    cMesh* l_pMesh = new cMesh();
    AssignMeshAttributes(l_pMesh, e_Mesh, e_Model, e_bCalculateBiNormal);
    m_NameAndMeshes[e_Mesh.name] = l_pMesh;
    return l_pMesh;
}

cMesh* cglTFModel::GenerateAnimationMesh(const tinygltf::Skin& e_Skin, const tinygltf::Mesh& e_Mesh, tinygltf::Model& e_Model, bool e_bCalculateBiNormal)
{
    cSkinningMesh*l_pSkinningMesh = new cSkinningMesh();
    AssignMeshAttributes(l_pSkinningMesh, e_Mesh, e_Model, e_bCalculateBiNormal);
    l_pSkinningMesh->LoadAnimations(e_Skin,this,e_Model);
    m_AnimationMeshMap[e_Mesh.name] = l_pSkinningMesh;
    return nullptr;
}

GLuint cglTFModel::GetShaderProgram(unsigned int fvfFlags)
{
    // Check if the shader program for this FVF already exists
    auto it = shaderPrograms.find(fvfFlags);
    if (it != shaderPrograms.end())
    {
        return it->second; // Return existing shader program
    }

    // If not, create a new shader program
    GLuint shaderProgram = CreateShader(fvfFlags);
    shaderPrograms[fvfFlags] = shaderProgram;  // Store in the map
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
// Load the scene from a GLTF file
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
    LoadNodes(model,e_bCalculateBiNormal);
    //for (const auto& meshPair : model.meshes)
    for (int i=0;i<model.meshes.size();++i)
    {
        const auto& meshPair = model.meshes[i];
        cMesh* mesh = nullptr;
        if (!model.animations.empty())
        {
            mesh = new cSkinningMesh();
        }
        else
        {
            mesh = new cMesh();
        }

        for (const auto& primitive : meshPair.primitives)
        {
            //mesh->LoadAttributes(model, primitive, e_bCalculateBiNormal);
            mesh->LoadAttributesAndInitBuffer(model, primitive, e_bCalculateBiNormal);
            // Get or create the appropriate shader program for the sub-mesh
            for (auto l_pSubMesh : mesh->m_SubMeshesVector)
            {
                l_pSubMesh->shaderProgram = GetShaderProgram(l_pSubMesh->m_iFVFFlag);
                // Load textures for each material
                if (primitive.material >= 0 && primitive.material < model.materials.size())
                {
                    mesh->LoadMaterial(model, model.materials[primitive.material], l_pSubMesh->shaderProgram);
                }
            }
            // Apply the node transformation to the mesh
            for (const auto& node : model.nodes)
            {
                if (node.mesh == std::distance(model.meshes.begin(), std::find(model.meshes.begin(), model.meshes.end(), meshPair)))
                {
                    cMatrix44 nodeMatrix = GetNodeMatrix(node);
                    mesh->SetLocalTransform(nodeMatrix);
                    break;
                }
            }
        }

        if (cSkinningMesh* animMesh = dynamic_cast<cSkinningMesh*>(mesh))
        {
            //animMesh->LoadAnimations(model, this->m_NodeIndexAndBoneMap, &this->m_NodesVector);
            m_AnimationMeshMap[meshPair.name] = (cSkinningMesh*)mesh;
        }
        else
        {
            m_NameAndMeshes[meshPair.name] = mesh;
        }
    }
    loadAnimations(model);
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

// Draw the scene
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
    for (auto& meshPair : m_NameAndMeshes)
    {
        meshPair.second->Render();
    }
    for (auto& meshPair : m_AnimationMeshMap)
    {
        meshPair.second->Update(0.016f);
        meshPair.second->Render();
    }
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cglTFModel::Destory()
{
    DELETE_MAP(m_NameAndMeshes);
    DELETE_MAP(m_AnimationMeshMap);
}

void cglTFModel::SetCurrentAnimation(const std::string& e_strAnimationName)
{
    this->m_AnimationClip.SetAnimation(e_strAnimationName.c_str(), true);
}

#include "./ThirdParty/Chapter10Sample02.h"
#include "./ThirdParty/Chapter10Sample01.h"
#include "./ThirdParty/Chapter12Sample03.h"

cglTFModel g_cScene;
Chapter12Sample03 g_Chapter10Sample01;
int glTFInit()
{
    //g_cScene.LoadFromGLTF("glTFModel/Duck.gltf",false);
    //g_cScene.LoadFromGLTF("glTFModel/Lantern.gltf",true);
    // 
    //g_cScene.LoadFromGLTF("glTFModel/Avocado.gltf", true);
    //g_cScene.LoadFromGLTF("glTFModel/CesiumMilkTruck.glb", true);
    //g_cScene.LoadFromGLTF("glTFModel/Fox.gltf", true);
    //g_cScene.LoadFromGLTF("glTFModel/SimpleSkin.gltf", true);
    //g_cScene.LoadFromGLTF("glTFModel/Woman.gltf", true);
    
    //g_cScene.LoadFromGLTF("glTFModel/Buggy.gltf", false);
    //g_cScene.LoadFromGLTF("glTFModel/AnimatedCube.gltf", false);
    g_cScene.LoadFromGLTF("glTFModel/BoxAnimated.gltf", false);
    
    
    g_Chapter10Sample01.Initialize();
    //g_cScene.InitBuffers();
    return 1;
}

void GlTFRender()
{
    //    DrawModel(model, shaderProgram);
    g_cScene.Render();
    g_Chapter10Sample01.Update(0.016f);
    g_Chapter10Sample01.Render(16/9);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    for (auto& meshPair : g_cScene.m_AnimationMeshMap)
    {
        if (meshPair.second)
        {
            meshPair.second->RenderSkeleton();
        }
    }
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GlTFDestory()
{
    //    DrawModel(model, shaderProgram);
    g_cScene.Destory();
    cTextureManager::ClearSharedTextureReferenceMap();
}

