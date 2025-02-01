
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
        void main() {
            vec4 worldPosition = inMat4Model * vec4(aPosition, 1.0);
            toFSVec3FragPos = worldPosition.xyz;

            // Passing light direction to fragment shader
            toFSVec3LightDir = normalize(inVec3LightPosition - toFSVec3FragPos);

            // Transform normal to world space and then to view space
            toFSVec3Normal = mat3(transpose(inverse(inMat4Model))) * aNormal;
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

            gl_Position = inMat4Projection * inMat4View * worldPosition;
        }
    )";

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

            // Normal map processing (if flag is set)
            vec3 normal = normalize(toFSVec3Normal);
            if (bool(fvfFlags & FVF_NORMAL_MAP)) {  // Check if FVF_NORMAL_MAP bit is set
                vec3 normalMapColor = texture(normalMap, toFSVec2TexCoord).rgb;
                normalMapColor = normalize(normalMapColor * 2.0 - 1.0);  // Convert from [0, 1] to [-1, 1]
    )";

    if ((fvfFlags & FVF_TANGENT_FLAG) && (fvfFlags & FVF_BITAGENT_FLAG))
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

            // Apply texture and lighting effects
            FragColor = vec4(color * lighting, 1.0);
        }
    )";

    return shaderCode;
}







GLuint cScene::CreateShader(unsigned int fvfFlags)
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
        assert(meshPair.primitives.size()==1 && "no sub mesh because I am lazy to do different FVF fuck.");
        cMesh mesh;
        for (const auto& primitive : meshPair.primitives)
        {
            mesh.LoadAttributes(model, primitive, e_bCalculateBiNormal);

            // Load textures for each material
            if (primitive.material >= 0 && primitive.material < model.materials.size())
            {
                mesh.LoadTextures(model, model.materials[primitive.material]);
            }
        }
        // Get or create the appropriate shader program
        mesh.m_uiShaderProgram = GetShaderProgram(mesh.m_uiFVFFlags);
        meshes[meshPair.name] = mesh;
    }

    return true;
}

// Initialize buffers for all meshes
void cScene::InitBuffers()
{
    for (auto& meshPair : meshes)
    {
        meshPair.second.InitBuffer();
    }
}

// Draw the scene
void cScene::Draw()
{
    for (auto& meshPair : meshes)
    {
        meshPair.second.Draw();
    }
}

cScene g_cScene;
int glTFInit()
{
    g_cScene.LoadFromGLTF("glTFModel/Duck.gltf",true);
    g_cScene.InitBuffers();
    return 1;
}

void GlTFRender()
{
    //    DrawModel(model, shaderProgram);
    g_cScene.Draw();
}

