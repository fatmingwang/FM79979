#include "ShadowMap.h"
#include "LazyShaderGenerator.h"

cShadowMap* g_pShadowMap = nullptr;
cBaseShader* g_pDepthShader = nullptr;
namespace FATMING_CORE
{
    extern cNamedTypedObjectVector<cBaseShader>* g_pAll2DShaderList;
}

std::string GenerateShadowMapFragmentShader()
{
#if defined(WIN32)
    return "#version 330 core\nvoid main() {}";
#else
    return "#version 300 es\nprecision mediump float;\nvoid main() {}";
#endif
}

std::string GenerateShadowMapVertexShader()
{
    std::string shaderCode;
#if defined(WIN32)
    shaderCode += "#version 330 core\n";
#else
    shaderCode += R"(#version 300 es
precision mediump float;
precision highp int;
)";
#endif
    shaderCode += "#define USE_POSITION\n";
    shaderCode += R"(
layout(location = 0) in vec3 aPosition;
uniform mat4 uMat4Model;
uniform mat4 uLightViewProj;
void main()
{
    gl_Position = uLightViewProj * uMat4Model * vec4(aPosition, 1.0);
}
)";
    return shaderCode;
}

cShadowMap::cShadowMap() 
{
    g_pShadowMap = this;
    //g_strCommonDepthTexture
    //g_strCommonVS
    //std::string vs = GenerateShadowMapVertexShader();
    //std::string fs = GenerateShadowMapFragmentShader();
    //m_ShadowMapProgram = CreateOpenGLShaderProgram(vs, fs);
	g_pDepthShader = new cBaseShader(g_strCommonVS, g_strCommonDepthTextureFS,L"DepthTexture", true);
    auto l_strname = g_pDepthShader->GetName();
	g_pAll2DShaderList->AddObject(g_pDepthShader);
    int a = 0;
}
cShadowMap::~cShadowMap()
{
    g_pShadowMap = nullptr;
    Cleanup();
}

bool cShadowMap::Init(int width, int height)
{
    m_Width = width;
    m_Height = height;
    glGenFramebuffers(1, &m_Framebuffer);
    glGenTextures(1, &m_DepthTexture);
    glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0, 1.0, 0.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    bool status = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return status;
}

void cShadowMap::InitShadowMapProgram()
{
    std::string vs = GenerateShadowMapVertexShader();
    std::string fs = GenerateShadowMapFragmentShader();
    m_ShadowMapProgram = CreateOpenGLShaderProgram(vs, fs);
}

void cShadowMap::RenderFrameBufferAs2DImage(Vector2 e_vPos, Vector2 e_vSize)
{
    UseShaderProgram(g_pDepthShader->GetName());
    glEnable2D(m_Width, m_Height);
    //auto l_Result = glCheckFramebufferStatus(GL_FRAMEBUFFER);//GL_FRAMEBUFFER_COMPLETE
    //cTexture::ApplyImage(m_Framebuffer);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
    float	l_fTextureCoordinate[] = { 0,1,1,0 };
    //RenderQuadWithTextureAndColorAndCoordinate(e_vPos.x,e_vPos.y, 0.f, e_vSize.x, e_vSize.y, Vector4::One, l_fTextureCoordinate, Vector3::Zero);
    RenderQuadWithTextureAndColorAndCoordinate(e_vPos.x, e_vPos.y, 0.f, m_Width/4.f, m_Height/4.f, Vector4::One, l_fTextureCoordinate, Vector3::Zero);
}

void cShadowMap::BindForWriting()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
}

void cShadowMap::BindForReading(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
}

void cShadowMap::Cleanup()
{
    if (m_DepthTexture) glDeleteTextures(1, &m_DepthTexture);
    if (m_Framebuffer) glDeleteFramebuffers(1, &m_Framebuffer);
    if (m_ShadowMapProgram) glDeleteProgram(m_ShadowMapProgram);
    m_DepthTexture = 0;
    m_Framebuffer = 0;
    m_ShadowMapProgram = 0;
}
