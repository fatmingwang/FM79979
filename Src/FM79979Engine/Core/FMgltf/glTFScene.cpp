
#include "../AllCoreInclude.h"
#include <vector>
#include <string>
#include <map>
#include "tiny_gltf.h"
#include <iostream>
#include <sstream>
#include "glTFScene.h"


std::string cScene::GenerateVertexShader(unsigned int fvfFlags)
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
        shaderCode += "layout(location = 3) in vec3 aTangent;\n";
    }

    // Adding binormals if necessary
    if (fvfFlags & FVF_BITAGENT_FLAG)
    {
        shaderCode += "layout(location = 4) in vec3 aBinormal;\n";
    }

    // Adding joint indices and weights if necessary
    if (fvfFlags & FVF_SKINNING_WEIGHT_FLAG)
    {
        shaderCode += "layout(location = 5) in vec4 aWeights;\n";
    }

    if (fvfFlags & FVF_SKINNING_BONE_INDEX_FLAG)
    {
        shaderCode += "layout(location = 6) in ivec4 aJoints;\n";
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
        shaderCode += "out vec3 toFSVec3Tangent;\n";
    }

    if (fvfFlags & FVF_BITAGENT_FLAG)
    {
        shaderCode += "out vec3 toFSVec3Binormal;\n";
    }

    shaderCode += R"(
        const int MAX_BONES = 100;
        uniform mat4 uBoneTransforms[MAX_BONES];
    )";

    shaderCode += R"(
        void main() {
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
        })";
    }
    else
    {
        shaderCode += R"(    gl_Position = inMat4Projection * inMat4View * worldPosition;
                             // Transform normal to world space and then to view space
                             toFSVec3Normal = mat3(transpose(inverse(inMat4Model))) * aNormal;
        })";
    }

    return shaderCode;
}


std::string cScene::GenerateFragmentShader(unsigned int fvfFlags)
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
        in vec3 toFSVec3LightDir;

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
        vec3 ComputeLighting(vec3 normal, vec3 lightDir, vec3 viewDir) {
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
        vec3 ComputeDirectionalLight(vec3 normal, vec3 viewDir) {
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

    if (fvfFlags & FVF_TANGENT_FLAG)
    {
        shaderCode += "in vec3 toFSVec3Tangent;\n";
    }

    if (fvfFlags & FVF_BITAGENT_FLAG)
    {
        shaderCode += "in vec3 toFSVec3Binormal;\n";
    }

    shaderCode += R"(
        void main() {
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
        })";
    }
    else
    {
        shaderCode += R"(
            // Skip lighting calculations and directly set FragColor
            FragColor = vec4(color, 1.0);
            //FragColor = vec4(1,0,0, 1.0);
        })";
    }

    return shaderCode;
}







GLuint cScene::CreateShader(unsigned int fvfFlags)
{
    std::string vertexCode = GenerateVertexShader(fvfFlags);
    std::string fragmentCode = GenerateFragmentShader(fvfFlags);
    FMLOG(vertexCode.c_str());
    FMLOG(fragmentCode.c_str());
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
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

GLuint cScene::GetShaderProgram(unsigned int fvfFlags)
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

    // Apply translation
    if (node.translation.size() == 3)
    {
        Vector3 translation((float)node.translation[0], (float)node.translation[1], (float)node.translation[2]);
        matrix = cMatrix44::TranslationMatrix(translation);
    }

    // Apply rotation
    if (node.rotation.size() == 4)
    {
        
        Quaternion rotation((float)node.rotation[3], (float)node.rotation[0], (float)node.rotation[1], (float)node.rotation[2]);
        matrix *= rotation.ToMatrix();
    }

    // Apply scale
    if (node.scale.size() == 3)
    {
        Vector3 scale((float)node.scale[0], (float)node.scale[1], (float)node.scale[2]);
        matrix *= cMatrix44::ScaleMatrix(scale);
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
bool cScene::LoadFromGLTF(const std::string& filename, bool e_bCalculateBiNormal)
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

    for (const auto& meshPair : model.meshes)
    {
        cMesh* mesh = nullptr;
        if (!model.animations.empty())
        {
            mesh = new cAnimationMesh();
        }
        else
        {
            mesh = new cMesh();
        }

        for (const auto& primitive : meshPair.primitives)
        {
            //mesh->LoadAttributes(model, primitive, e_bCalculateBiNormal);
            mesh->LoadAttributesAndInitBuffer(model, primitive, e_bCalculateBiNormal);

            // Load textures for each material
            if (primitive.material >= 0 && primitive.material < model.materials.size())
            {
                mesh->LoadTextures(model, model.materials[primitive.material]);
            }
            // Get or create the appropriate shader program for the sub-mesh
            for (auto l_pSubMesh : mesh->m_SubMeshesVector)
            {
                l_pSubMesh->shaderProgram = GetShaderProgram(l_pSubMesh->m_iFVFFlag);
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

        if (cAnimationMesh* animMesh = dynamic_cast<cAnimationMesh*>(mesh))
        {
            animMesh->LoadAnimations(model);
            m_AnimationMeshMap[meshPair.name] = (cAnimationMesh*)mesh;
        }
        else
        {
            meshes[meshPair.name] = mesh;
        }
    }
    return true;
}

// Initialize buffers for all meshes
void cScene::InitBuffers()
{
    //for (auto& meshPair : meshes)
    //{
    //    meshPair.second->InitBuffer();
    //}
    //for (auto& meshPair : m_AnimationMeshMap)
    //{
    //    meshPair.second->InitBuffer();
    //}
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Draw the scene
void cScene::Draw()
{
    cBaseShader* l_pShader = GetCurrentShader();
    if (l_pShader)
    {
        l_pShader->Unuse();
    }
    UseShaderProgram(L"qoo79979");
    // Enable backface culling
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glCullFace(GL_FRONT);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    for (auto& meshPair : meshes)
    {
        //meshPair.second.SetLocalPosition(Vector3(l_iIndex,0,0));
        meshPair.second->Draw();
    }
    for (auto& meshPair : m_AnimationMeshMap)
    {
        //meshPair.second.SetLocalPosition(Vector3(l_iIndex,0,0));
        meshPair.second->Update(0.016f);
        meshPair.second->Draw();
        //meshPair.second->RenderBindPose();
    }
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    for (auto& meshPair : m_AnimationMeshMap)
    {
        if (meshPair.second)
        {
            meshPair.second->RenderSkeleton();
        }
    }
    
}

void cScene::Destory()
{
    DELETE_MAP(meshes);
    DELETE_MAP(m_AnimationMeshMap);
}


cScene g_cScene;

int glTFInit()
{
    //g_cScene.LoadFromGLTF("glTFModel/Duck.gltf",false);
    //g_cScene.LoadFromGLTF("glTFModel/Lantern.gltf",true);
    // 
    //g_cScene.LoadFromGLTF("glTFModel/Avocado.gltf", true);
    //g_cScene.LoadFromGLTF("glTFModel/Fox.gltf", true);
    //g_cScene.LoadFromGLTF("glTFModel/SimpleSkin.gltf", true);
    
    //g_cScene.LoadFromGLTF("glTFModel/Buggy.gltf", false);
    g_cScene.LoadFromGLTF("glTFModel/AnimatedCube.gltf", false);
    
    
    g_cScene.InitBuffers();
    return 1;
}

void GlTFRender()
{
    //    DrawModel(model, shaderProgram);
    g_cScene.Draw();
}

void GlTFDestory()
{
    //    DrawModel(model, shaderProgram);
    g_cScene.Destory();
}

