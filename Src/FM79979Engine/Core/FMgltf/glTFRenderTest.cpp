#include "../AllCoreInclude.h"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tiny_gltf.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>

// Helper function to load a texture
GLuint loadTexture(const tinygltf::Image& image)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}


//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

// Define attribute locations
enum AttributeLocation
{
    POSITION = 0,
    NORMAL = 1,
    TEXCOORD_0 = 2,
    // Add more attribute locations as needed
};

// Create a mapping from attribute names to attribute locations
std::unordered_map<std::string, GLuint> attributeLocationMap = {
    {"POSITION", POSITION},
    {"NORMAL", NORMAL},
    {"TEXCOORD_0", TEXCOORD_0},
    // Add more mappings as needed
};

// Vertex shader code
const char* vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

// Fragment shader code
const char* fragmentShaderSource = R"(
#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
)";

GLuint CompileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

GLuint CreateShaderProgram(const char* vertexSource, const char* fragmentSource)
{
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void LoadModel(tinygltf::Model& model, const std::string& filename)
{
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    std::string ext = filename.substr(filename.find_last_of(".") + 1);
    bool ret = false;

    if (ext == "gltf")
    {
        ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    }
    else if (ext == "glb")
    {
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
    }
    else
    {
        std::cerr << "Unsupported file extension: " << ext << std::endl;
        return;
    }

    if (!warn.empty())
    {
        std::cout << "Warn: " << warn << std::endl;
    }
    if (!err.empty())
    {
        std::cerr << "Err: " << err << std::endl;
    }
    if (!ret)
    {
        std::cerr << "Failed to load glTF: " << filename << std::endl;
    }
}

std::unordered_map<int, GLuint> bufferViewToVBO;

void InitBuffers(const tinygltf::Model& model)
{
    for (size_t i = 0; i < model.bufferViews.size(); ++i)
    {
        const tinygltf::BufferView& bufferView = model.bufferViews[i];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(bufferView.target, vbo);
        glBufferData(bufferView.target, bufferView.byteLength, &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);

        bufferViewToVBO[i] = vbo;
    }
}
namespace FATMING_CORE
{
    extern cBaseShader* g_pCurrentShader;
}

Vector3 l_vPos(0, 0, 30);
void DrawModel(const tinygltf::Model& model, GLuint shaderProgram)
{
    if (g_pCurrentShader)
    {
        g_pCurrentShader->Unuse();
        g_pCurrentShader = nullptr;
    }
    glUseProgram(shaderProgram);

    float l_fNearPlane = 1000.0f * 1e-4f;
    float l_fFarPlane = 20000.f;
    // Set up the transformation matrices
	cMatrix44 l_mat = cMatrix44::Identity;
    cMatrix44	l_matViewMatrix = cMatrix44::LookAtMatrix(l_vPos, Vector3(0, 0, 0), Vector3(0, 1, 0));
    Projection l_Projection;
    l_Projection.SetFovYAspect(XM_PIDIV4, (float)1920 / (float)1080, l_fNearPlane, l_fFarPlane);
    //glm::mat4 modelMatrix = glm::mat4(1.0f);
    //glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, l_mat);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, l_matViewMatrix);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, l_Projection.GetMatrix());

    for (const auto& mesh : model.meshes)
    {
        for (const auto& primitive : mesh.primitives)
        {
            const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
            const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferViewToVBO[indexAccessor.bufferView]);

            for (const auto& attrib : primitive.attributes)
            {
                const tinygltf::Accessor& accessor = model.accessors[attrib.second];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];

                glBindBuffer(GL_ARRAY_BUFFER, bufferViewToVBO[accessor.bufferView]);

                GLint size = 1;
                if (accessor.type == TINYGLTF_TYPE_SCALAR) size = 1;
                else if (accessor.type == TINYGLTF_TYPE_VEC2) size = 2;
                else if (accessor.type == TINYGLTF_TYPE_VEC3) size = 3;
                else if (accessor.type == TINYGLTF_TYPE_VEC4) size = 4;

                GLenum type = GL_FLOAT;
                if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_BYTE) type = GL_BYTE;
                else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) type = GL_UNSIGNED_BYTE;
                else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_SHORT) type = GL_SHORT;
                else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) type = GL_UNSIGNED_SHORT;
                else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_INT) type = GL_INT;
                else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) type = GL_UNSIGNED_INT;
                else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) type = GL_FLOAT;

                GLuint location = attributeLocationMap[attrib.first];
                glVertexAttribPointer(location, size, type, accessor.normalized ? GL_TRUE : GL_FALSE, bufferView.byteStride, (void*)accessor.byteOffset);
                glEnableVertexAttribArray(location);
            }

            glDrawElements(primitive.mode, indexAccessor.count, indexAccessor.componentType, (void*)indexAccessor.byteOffset);
        }
    }
}

tinygltf::Model model;
GLuint shaderProgram;

//int glTFInit()
//{
//    shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
//    LoadModel(model, "glTFModel/Duck.gltf");
//    InitBuffers(model);
//    return 1;
//}
//
//void GlTFRender()
//{
//    DrawModel(model, shaderProgram);
//}
//
