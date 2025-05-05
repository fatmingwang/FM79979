#pragma once
#include <string>
#include "../AllCoreInclude.h"
enum class eLightType
{
    eLT_DIRECTIONAL = 0,
    eLT_POINT,
    eLT_SPOT,
    eLT_MAX
};

// Structure to hold light properties
struct sLightData
{
    eLightType  m_eType = eLightType::eLT_MAX;
    float       m_fIntensity = 1.0f;
    float       m_fRange = 0.0f;         // Only for point and spot
    float       m_fInnerConeAngle = 0.0f; // Only for spot
    float       m_fOuterConeAngle = 0.0f;
    Vector3     m_vPosition = Vector3::Zero;
    Vector3     m_vDirection = Vector3(0.0f, 0.0f, -1.0f); // Default down
    Vector3     m_vColor = Vector3(1.0f, 1.f, 1.f);
};

class cglTFLight:public NamedTypedObject
{
private:
    std::vector< sLightData>        m_LightDataVector;
    void                            CreateDefaulights();
public:
    DEFINE_TYPE_INFO();
    void                            LoadLightsFromGLTF(const tinygltf::Model& model);
    static bool                     IsLightExists(const tinygltf::Model& model);
};

class cLighCollector:public NamedTypedObject, public cSingltonTemplate<NamedTypedObject>
{
    std::vector< sLightData> m_LightDataVector;
public:
    DEFINE_TYPE_INFO();
    const   std::vector<sLightData>& GetLights() const;
    void    SetLight(int e_iIndex, sLightData e_sLightData);
    void    AddLight(sLightData e_sLightData);
    void    Render();
};


class cLighFrameData:public Frame
{
public:
    DEFINE_TYPE_INFO();
    sLightData m_LightData;
};


//std140 Padding Rules(WebGL 2 follows them strictly)
//Type	Aligned Size
//float	4 bytes
//vec2	8 bytes
//vec3 / vec4	16 bytes
//struct	round up members to 16 bytes each

//GLSL
//layout(std140) uniform LightBlock
//{
//    int numLights;
//    vec3 pad; // std140 padding to align to 16 bytes
//    struct Light
//    {
//        vec3 position;
//        float intensity;
//        vec3 color;
//        float pad1;
//    } lights[256];
//};


//struct alignas(16) GpuLight
//{
//    glm::vec3 position;
//    float intensity;   // 16 bytes total
//    glm::vec3 color;
//    float pad1 = 0.0f; // 16 bytes total
//};
//
//struct alignas(16) LightBlock
//{
//    int numLights;
//    float pad[3]; // pad to 16 bytes
//    GpuLight lights[256];
//};
//bind UBO
//GLuint lightUBO;
//glGenBuffers(1, &lightUBO);
//glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
//glBufferData(GL_UNIFORM_BUFFER, sizeof(LightBlock), nullptr, GL_DYNAMIC_DRAW);
//glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightUBO); // bind point 0
//
//GLuint blockIndex = glGetUniformBlockIndex(programID, "LightBlock");
//glUniformBlockBinding(programID, blockIndex, 0);


//update light data
//LightBlock lightBlock;
//lightBlock.numLights = numDynamicLights;
//for (int i = 0; i < numDynamicLights; ++i)
//{
//    lightBlock.lights[i].position = lights[i].position;
//    lightBlock.lights[i].color = lights[i].color;
//    lightBlock.lights[i].intensity = lights[i].intensity;
//}
//
//glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightBlock), &lightBlock);
// 
// 
//for openGL check cpu limit
//int maxUniformBlockSize = 0;
//glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize); // in bytes
//int maxLights = (maxUniformBlockSize - 16) / sizeof(GpuLight);  // 16 for header
//std::cout << "Max lights supported in UBO: " << maxLights << std::endl;