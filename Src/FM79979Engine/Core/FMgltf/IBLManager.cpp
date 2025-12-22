#include "../AllCoreInclude.h"
#include "IBLManager.h"
#include "stb_image.h"
#include "../../imgui/imgui.h"

// Singleton instance
cIBLManager* cIBLManager::s_pInstance = nullptr;
cIBLManager* g_pIBLManager = nullptr;

// ============================================================================
// IBL Shader Sources
// ============================================================================

static const char* g_strEquirectToCubemapVS = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 localPos;
uniform mat4 uProjection;
uniform mat4 uView;
void main()
{
    localPos = aPos;
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}
)";

static const char* g_strEquirectToCubemapFS = R"(
#version 330 core
out vec4 FragColor;
in vec3 localPos;
uniform sampler2D uEquirectangularMap;
const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}
void main()
{
    vec2 uv = SampleSphericalMap(normalize(localPos));
    vec3 color = texture(uEquirectangularMap, uv).rgb;
    FragColor = vec4(color, 1.0);
}
)";

static const char* g_strIrradianceVS = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 localPos;
uniform mat4 uProjection;
uniform mat4 uView;
void main()
{
    localPos = aPos;
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}
)";

static const char* g_strIrradianceFS = R"(
#version 330 core
out vec4 FragColor;
in vec3 localPos;
uniform samplerCube uEnvironmentMap;
const float PI = 3.14159265359;
void main()
{
    vec3 N = normalize(localPos);
    vec3 irradiance = vec3(0.0);
    
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up         = normalize(cross(N, right));
    
    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;
            irradiance += texture(uEnvironmentMap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    FragColor = vec4(irradiance, 1.0);
}
)";

static const char* g_strPrefilterVS = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 localPos;
uniform mat4 uProjection;
uniform mat4 uView;
void main()
{
    localPos = aPos;
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}
)";

static const char* g_strPrefilterFS = R"(
#version 330 core
out vec4 FragColor;
in vec3 localPos;
uniform samplerCube uEnvironmentMap;
uniform float uRoughness;
const float PI = 3.14159265359;

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10;
}

vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness * roughness;
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
    vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

void main()
{
    vec3 N = normalize(localPos);
    vec3 R = N;
    vec3 V = R;
    const uint SAMPLE_COUNT = 1024u;
    float totalWeight = 0.0;
    vec3 prefilteredColor = vec3(0.0);
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H = ImportanceSampleGGX(Xi, N, uRoughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);
        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0)
        {
            prefilteredColor += texture(uEnvironmentMap, L).rgb * NdotL;
            totalWeight += NdotL;
        }
    }
    prefilteredColor = prefilteredColor / totalWeight;
    FragColor = vec4(prefilteredColor, 1.0);
}
)";

static const char* g_strBrdfVS = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
out vec2 TexCoords;
void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}
)";

static const char* g_strBrdfFS = R"(
#version 330 core
out vec2 FragColor;
in vec2 TexCoords;
const float PI = 3.14159265359;

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10;
}

vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness * roughness;
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
    vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float a = roughness;
    float k = (a * a) / 2.0;
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec2 IntegrateBRDF(float NdotV, float roughness)
{
    vec3 V;
    V.x = sqrt(1.0 - NdotV*NdotV);
    V.y = 0.0;
    V.z = NdotV;
    float A = 0.0;
    float B = 0.0;
    vec3 N = vec3(0.0, 0.0, 1.0);
    const uint SAMPLE_COUNT = 1024u;
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);
        float NdotL = max(L.z, 0.0);
        float NdotH = max(H.z, 0.0);
        float VdotH = max(dot(V, H), 0.0);
        if(NdotL > 0.0)
        {
            float G = GeometrySmith(N, V, L, roughness);
            float G_Vis = (G * VdotH) / (NdotH * NdotV);
            float Fc = pow(1.0 - VdotH, 5.0);
            A += (1.0 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }
    A /= float(SAMPLE_COUNT);
    B /= float(SAMPLE_COUNT);
    return vec2(A, B);
}

void main()
{
    vec2 integratedBRDF = IntegrateBRDF(TexCoords.x, TexCoords.y);
    FragColor = integratedBRDF;
}
)";

// ============================================================================
// Cube vertices for rendering to cubemap faces
// ============================================================================
static float g_CubeVertices[] = {
    // Back face
    -1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    // Front face
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    // Left face
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    // Right face
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    // Bottom face
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    // Top face
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
};

// Quad vertices for BRDF LUT
static float g_QuadVertices[] = {
    // positions        // texture coords
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};

// ============================================================================
// Implementation
// ============================================================================

cIBLManager::cIBLManager()
{
}

cIBLManager::~cIBLManager()
{
    Cleanup();
}

cIBLManager* cIBLManager::GetInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new cIBLManager();
        g_pIBLManager = s_pInstance;
    }
    return s_pInstance;
}

void cIBLManager::DestroyInstance()
{
    if (s_pInstance)
    {
        delete s_pInstance;
        s_pInstance = nullptr;
        g_pIBLManager = nullptr;
    }
}

void cIBLManager::Initialize()
{
    if (m_bIsInitialized)
        return;
    
    SetupCube();
    SetupQuad();
    CreateIBLShaders();
    
    // Generate BRDF LUT once (it's view-independent)
    GenerateBRDFLut();
    
    m_bIsInitialized = true;
    FMLOG("IBL Manager initialized");
}

void cIBLManager::SetupCube()
{
    glGenVertexArrays(1, &m_uiCubeVAO);
    glGenBuffers(1, &m_uiCubeVBO);
    glBindVertexArray(m_uiCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_uiCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_CubeVertices), g_CubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

void cIBLManager::SetupQuad()
{
    glGenVertexArrays(1, &m_uiQuadVAO);
    glGenBuffers(1, &m_uiQuadVBO);
    glBindVertexArray(m_uiQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_uiQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_QuadVertices), g_QuadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

void cIBLManager::RenderCube()
{
    glBindVertexArray(m_uiCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void cIBLManager::RenderQuad()
{
    glBindVertexArray(m_uiQuadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

GLuint cIBLManager::CreateShaderProgram(const char* vertSrc, const char* fragSrc)
{
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSrc, nullptr);
    glCompileShader(vertShader);
    
    GLint success;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertShader, 512, nullptr, infoLog);
        FMLOG("IBL Vertex Shader Error: %s", infoLog);
    }
    
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSrc, nullptr);
    glCompileShader(fragShader);
    
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
        FMLOG("IBL Fragment Shader Error: %s", infoLog);
    }
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        FMLOG("IBL Shader Link Error: %s", infoLog);
    }
    
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    
    return program;
}

void cIBLManager::CreateIBLShaders()
{
    m_uiEquirectToCubemapShader = CreateShaderProgram(g_strEquirectToCubemapVS, g_strEquirectToCubemapFS);
    m_uiIrradianceShader = CreateShaderProgram(g_strIrradianceVS, g_strIrradianceFS);
    m_uiPrefilterShader = CreateShaderProgram(g_strPrefilterVS, g_strPrefilterFS);
    m_uiBrdfShader = CreateShaderProgram(g_strBrdfVS, g_strBrdfFS);
}

bool cIBLManager::LoadFromHDRI(const char* e_strHDRIPath)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float* data = stbi_loadf(e_strHDRIPath, &width, &height, &nrComponents, 0);
    
    if (!data)
    {
        FMLOG("Failed to load HDRI: %s", e_strHDRIPath);
        return false;
    }
    
    FMLOG("Loaded HDRI: %s (%dx%d, %d components)", e_strHDRIPath, width, height, nrComponents);
    
    // Create HDR texture
    GLuint hdrTexture;
    glGenTextures(1, &hdrTexture);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_image_free(data);
    
    // Create environment cubemap
    glGenTextures(1, &m_uiEnvironmentCubemapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiEnvironmentCubemapID);
    for (int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 
                     m_iEnvironmentMapSize, m_iEnvironmentMapSize, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Set up projection and view matrices for capturing cubemap faces
    cMatrix44 captureProjection;
    //captureProjection.SetProjectionMatrix(90.0f, 1.0f, 0.1f, 10.0f);
    
    cMatrix44 captureViews[6];
    //captureViews[0].SetViewLookAtMatrix(Vector3(0, 0, 0), Vector3( 1, 0, 0), Vector3(0, -1, 0)); // +X
    //captureViews[1].SetViewLookAtMatrix(Vector3(0, 0, 0), Vector3(-1, 0, 0), Vector3(0, -1, 0)); // -X
    //captureViews[2].SetViewLookAtMatrix(Vector3(0, 0, 0), Vector3( 0, 1, 0), Vector3(0, 0,  1)); // +Y
    //captureViews[3].SetViewLookAtMatrix(Vector3(0, 0, 0), Vector3( 0,-1, 0), Vector3(0, 0, -1)); // -Y
    //captureViews[4].SetViewLookAtMatrix(Vector3(0, 0, 0), Vector3( 0, 0, 1), Vector3(0, -1, 0)); // +Z
    //captureViews[5].SetViewLookAtMatrix(Vector3(0, 0, 0), Vector3( 0, 0,-1), Vector3(0, -1, 0)); // -Z
    
    // Create framebuffer for rendering
    GLuint captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_iEnvironmentMapSize, m_iEnvironmentMapSize);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
    
    // Convert equirectangular to cubemap
    glUseProgram(m_uiEquirectToCubemapShader);
    glUniform1i(glGetUniformLocation(m_uiEquirectToCubemapShader, "uEquirectangularMap"), 0);
    glUniformMatrix4fv(glGetUniformLocation(m_uiEquirectToCubemapShader, "uProjection"), 1, GL_FALSE, captureProjection.m[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    
    glViewport(0, 0, m_iEnvironmentMapSize, m_iEnvironmentMapSize);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (int i = 0; i < 6; ++i)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_uiEquirectToCubemapShader, "uView"), 1, GL_FALSE, captureViews[i].m[0]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_uiEnvironmentCubemapID, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // Generate mipmaps for environment cubemap
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiEnvironmentCubemapID);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    // Cleanup HDR texture
    glDeleteTextures(1, &hdrTexture);
    glDeleteFramebuffers(1, &captureFBO);
    glDeleteRenderbuffers(1, &captureRBO);
    
    // Generate IBL maps
    GenerateIBLMaps();
    
    m_bIBLEnabled = true;
    FMLOG("IBL maps generated from HDRI");
    
    return true;
}

void cIBLManager::GenerateIBLMaps()
{
    if (m_uiEnvironmentCubemapID == 0)
    {
        FMLOG("Error: No environment cubemap loaded");
        return;
    }
    
    // Set up projection and view matrices
    cMatrix44 captureProjection;
    //captureProjection.SetProjectionMatrix(90.0f, 1.0f, 0.1f, 10.0f);
    
    cMatrix44 captureViews[6];
    //captureViews[0].SetViewLookAtMatrix(Vector3(0, 0, 0), Vector3( 1, 0, 0), Vector3(0, -1, 0));
    //captureViews[1].SetViewLookAtMatrix(Vector3(0, 0, 0), Vector3(-1, 0, 0), Vector3(0, -1, 0));
    //captureViews[2].SetViewLookAtMatrix(Vector3(0, 0, 0), Vector3( 0, 1, 0), Vector3(0, 0,  1));
    //captureViews[3].SetViewLookAtMatrix(Vector3(0, 0, 0), Vector3( 0,-1, 0), Vector3(0, 0, -1));
    //captureViews[4].SetViewLookAtMatrix(Vector3(0, 0, 0), Vector3( 0, 0, 1), Vector3(0, -1, 0));
    //captureViews[5].SetViewLookAtMatrix(Vector3(0, 0, 0), Vector3( 0, 0,-1), Vector3(0, -1, 0));
    
    GLuint captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);
    
    // =========================================================================
    // Generate Diffuse Irradiance Map
    // =========================================================================
    glGenTextures(1, &m_uiDiffuseIrradianceMapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiDiffuseIrradianceMapID);
    for (int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 
                     m_iIrradianceMapSize, m_iIrradianceMapSize, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_iIrradianceMapSize, m_iIrradianceMapSize);
    
    glUseProgram(m_uiIrradianceShader);
    glUniform1i(glGetUniformLocation(m_uiIrradianceShader, "uEnvironmentMap"), 0);
    glUniformMatrix4fv(glGetUniformLocation(m_uiIrradianceShader, "uProjection"), 1, GL_FALSE, captureProjection.m[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiEnvironmentCubemapID);
    
    glViewport(0, 0, m_iIrradianceMapSize, m_iIrradianceMapSize);
    for (int i = 0; i < 6; ++i)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_uiIrradianceShader, "uView"), 1, GL_FALSE, captureViews[i].m[0]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_uiDiffuseIrradianceMapID, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    FMLOG("Diffuse irradiance map generated");
    
    // =========================================================================
    // Generate Pre-filtered Specular Environment Map
    // =========================================================================
    glGenTextures(1, &m_uiPrefilteredEnvMapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiPrefilteredEnvMapID);
    for (int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 
                     m_iPrefilteredMapSize, m_iPrefilteredMapSize, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    glUseProgram(m_uiPrefilterShader);
    glUniform1i(glGetUniformLocation(m_uiPrefilterShader, "uEnvironmentMap"), 0);
    glUniformMatrix4fv(glGetUniformLocation(m_uiPrefilterShader, "uProjection"), 1, GL_FALSE, captureProjection.m[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiEnvironmentCubemapID);
    
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    int maxMipLevels = 5;
    m_fMaxReflectionLOD = (float)(maxMipLevels - 1);
    for (int mip = 0; mip < maxMipLevels; ++mip)
    {
        int mipWidth = (int)(m_iPrefilteredMapSize * std::pow(0.5, mip));
        int mipHeight = (int)(m_iPrefilteredMapSize * std::pow(0.5, mip));
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);
        
        float roughness = (float)mip / (float)(maxMipLevels - 1);
        glUniform1f(glGetUniformLocation(m_uiPrefilterShader, "uRoughness"), roughness);
        
        for (int i = 0; i < 6; ++i)
        {
            glUniformMatrix4fv(glGetUniformLocation(m_uiPrefilterShader, "uView"), 1, GL_FALSE, captureViews[i].m[0]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_uiPrefilteredEnvMapID, mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            RenderCube();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    FMLOG("Pre-filtered specular map generated");
    
    glDeleteFramebuffers(1, &captureFBO);
    glDeleteRenderbuffers(1, &captureRBO);
}

void cIBLManager::GenerateBRDFLut()
{
    // Create BRDF LUT texture
    glGenTextures(1, &m_uiBrdfLutID);
    glBindTexture(GL_TEXTURE_2D, m_uiBrdfLutID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, m_iBrdfLutSize, m_iBrdfLutSize, 0, GL_RG, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLuint captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_iBrdfLutSize, m_iBrdfLutSize);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiBrdfLutID, 0);
    
    glViewport(0, 0, m_iBrdfLutSize, m_iBrdfLutSize);
    glUseProgram(m_uiBrdfShader);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderQuad();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &captureFBO);
    glDeleteRenderbuffers(1, &captureRBO);
    
    FMLOG("BRDF LUT generated");
}

GLuint cIBLManager::BindIBLTextures(GLuint e_uiShaderProgram, GLuint e_uiStartTextureUnit)
{
    if (!m_bIBLEnabled || !m_bIsInitialized)
        return e_uiStartTextureUnit;
    
    GLuint unit = e_uiStartTextureUnit;
    
    // Diffuse Irradiance Map
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiDiffuseIrradianceMapID);
    glUniform1i(glGetUniformLocation(e_uiShaderProgram, "uDiffuseIrradianceMap"), unit);
    unit++;
    
    // Pre-filtered Environment Map
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiPrefilteredEnvMapID);
    glUniform1i(glGetUniformLocation(e_uiShaderProgram, "uPrefilteredEnvMap"), unit);
    unit++;
    
    // BRDF LUT
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_uiBrdfLutID);
    glUniform1i(glGetUniformLocation(e_uiShaderProgram, "uBrdfLUT"), unit);
    unit++;
    
    // Max reflection LOD
    glUniform1f(glGetUniformLocation(e_uiShaderProgram, "uMaxReflectionLOD"), m_fMaxReflectionLOD);
    
    return unit;
}

void cIBLManager::RenderSkybox(GLuint e_uiShaderProgram, const float* e_pViewMatrix, const float* e_pProjectionMatrix)
{
    if (m_uiEnvironmentCubemapID == 0)
        return;
    
    glDepthFunc(GL_LEQUAL);
    glUseProgram(e_uiShaderProgram);
    
    glUniformMatrix4fv(glGetUniformLocation(e_uiShaderProgram, "uView"), 1, GL_FALSE, e_pViewMatrix);
    glUniformMatrix4fv(glGetUniformLocation(e_uiShaderProgram, "uProjection"), 1, GL_FALSE, e_pProjectionMatrix);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiEnvironmentCubemapID);
    glUniform1i(glGetUniformLocation(e_uiShaderProgram, "uEnvironmentMap"), 0);
    
    RenderCube();
    glDepthFunc(GL_LESS);
}

bool cIBLManager::LoadFromCubemapFiles(const char* e_strEnvCubemapPath,
                                        const char* e_strIrradianceMapPath,
                                        const char* e_strPrefilteredMapPath,
                                        const char* e_strBrdfLutPath)
{
    // TODO: Implement loading from pre-computed cubemap files
    // This would load DDS or KTX files directly
    FMLOG("LoadFromCubemapFiles not yet implemented");
    return false;
}

bool cIBLManager::LoadCubemapFromFaces(const char* e_strFolderPath, const char* e_strExtension)
{
    const char* faceNames[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
    
    glGenTextures(1, &m_uiEnvironmentCubemapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiEnvironmentCubemapID);
    
    for (int i = 0; i < 6; ++i)
    {
        std::string path = std::string(e_strFolderPath) + "/" + faceNames[i] + e_strExtension;
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            FMLOG("Failed to load cubemap face: %s", path.c_str());
            return false;
        }
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    GenerateIBLMaps();
    m_bIBLEnabled = true;
    
    return true;
}

void cIBLManager::RenderImGUI()
{
#ifdef DEBUG
    if (ImGui::CollapsingHeader("IBL Manager"))
    {
        ImGui::Checkbox("IBL Enabled", &m_bIBLEnabled);
        ImGui::SliderFloat("Max Reflection LOD", &m_fMaxReflectionLOD, 0.0f, 8.0f);
        ImGui::Text("Environment Map ID: %u", m_uiEnvironmentCubemapID);
        ImGui::Text("Diffuse Irradiance Map ID: %u", m_uiDiffuseIrradianceMapID);
        ImGui::Text("Prefiltered Env Map ID: %u", m_uiPrefilteredEnvMapID);
        ImGui::Text("BRDF LUT ID: %u", m_uiBrdfLutID);
    }
#endif
}

void cIBLManager::Cleanup()
{
    if (m_uiEnvironmentCubemapID) glDeleteTextures(1, &m_uiEnvironmentCubemapID);
    if (m_uiDiffuseIrradianceMapID) glDeleteTextures(1, &m_uiDiffuseIrradianceMapID);
    if (m_uiPrefilteredEnvMapID) glDeleteTextures(1, &m_uiPrefilteredEnvMapID);
    if (m_uiBrdfLutID) glDeleteTextures(1, &m_uiBrdfLutID);
    
    if (m_uiEquirectToCubemapShader) glDeleteProgram(m_uiEquirectToCubemapShader);
    if (m_uiIrradianceShader) glDeleteProgram(m_uiIrradianceShader);
    if (m_uiPrefilterShader) glDeleteProgram(m_uiPrefilterShader);
    if (m_uiBrdfShader) glDeleteProgram(m_uiBrdfShader);
    
    if (m_uiCubeVAO) glDeleteVertexArrays(1, &m_uiCubeVAO);
    if (m_uiCubeVBO) glDeleteBuffers(1, &m_uiCubeVBO);
    if (m_uiQuadVAO) glDeleteVertexArrays(1, &m_uiQuadVAO);
    if (m_uiQuadVBO) glDeleteBuffers(1, &m_uiQuadVBO);
    
    m_uiEnvironmentCubemapID = 0;
    m_uiDiffuseIrradianceMapID = 0;
    m_uiPrefilteredEnvMapID = 0;
    m_uiBrdfLutID = 0;
    m_bIsInitialized = false;
    m_bIBLEnabled = false;
}
