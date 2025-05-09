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

//Structure to hold light properties
//std140 layout rules
struct alignas(16) sLightData
{
    Vector3     m_vPosition;        // Aligned to 16 bytes
    float       m_fIntensity;      // Aligned to 16 bytes (vec4 padding)

    Vector3     m_vDirection;      // Aligned to 16 bytes
    float       m_fRange;          // Aligned to 16 bytes (vec4 padding)

    Vector3     m_vColor;          // Aligned to 16 bytes
    float       m_fInnerConeAngle; // Aligned to 16 bytes (vec4 padding)

    float       m_fOuterConeAngle; // Aligned to 4 bytes
    int         m_eType;           // Aligned to 4 bytes (enum stored as int)
    float       _padding[2];       // Padding to align the structure to 16 bytes
    bool operator==(const sLightData& other) const
    {
        return m_eType == other.m_eType &&
            m_fIntensity == other.m_fIntensity &&
            m_fRange == other.m_fRange &&
            m_fInnerConeAngle == other.m_fInnerConeAngle &&
            m_fOuterConeAngle == other.m_fOuterConeAngle &&
            m_vPosition == other.m_vPosition &&
            m_vDirection == other.m_vDirection &&
            m_vColor == other.m_vColor;
    }
};

class cLighController:public NamedTypedObject, public cSingltonTemplate<cLighController>
{
    // Prepare the UBO data
    struct alignas(16) LightBlock
    {
        int numLights;       // Number of lights
        float pad[3];        // Padding to align to 16 bytes
        sLightData lights[256]; // Array of light data
    } m_LightBlock;
    GLuint m_uiLightUBO = -1;
    std::vector<sLightData> m_LightDataVector;
    cLighController();
    virtual ~cLighController();
public:
    DEFINE_TYPE_INFO();
    SINGLETON_BASIC_FUNCTION(cLighController);
    const   std::vector<sLightData>& GetLights() const;
    void    SetLight(int e_iIndex, sLightData e_sLightData);
    void    AddLight(sLightData& e_sLightData);
    void    RemoveLight(sLightData& e_sLightData);
    void    Render(GLuint e_uiProgramID);
};


class cLighFrameData:public Frame
{
    sLightData m_LightData;
public:
    DEFINE_TYPE_INFO();
	cLighFrameData(sLightData e_LightData)
    {
		m_LightData = e_LightData; 
	}
	virtual ~cLighFrameData(){ }
    virtual void Render()override;
    virtual void EndRender()override;
	sLightData&  GetLightData()
	{
		return m_LightData;
	}
};

class cglTFLight :public NamedTypedObject
{
    private:
    std::vector< sLightData>        m_LightDataVector;
    public:
    DEFINE_TYPE_INFO();
    void                            LoadLightsFromGLTF(const tinygltf::Model& model);
    static cLighFrameData           CreateDirectionLight();
    static bool                     IsLightExists(const tinygltf::Model& model);
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