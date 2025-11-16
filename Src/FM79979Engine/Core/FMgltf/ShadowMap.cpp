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
    // If already initialized, cleanup previous resources so we can recreate with new size
    if (m_DepthTexture || m_Framebuffer)
    {
        Cleanup();
    }

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
    // ensure no mip sampling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
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
    glEnable2D((float)m_Width, (float)m_Height);
    //glEnable2D(1920.f,1080.f);// 
    //auto l_Result = glCheckFramebufferStatus(GL_FRAMEBUFFER);//GL_FRAMEBUFFER_COMPLETE
    //cTexture::ApplyImage(m_Framebuffer);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
    float	l_fTextureCoordinate[] = { 0,1,1,0 };
    //RenderQuadWithTextureAndColorAndCoordinate(e_vPos.x,e_vPos.y, 0.f, e_vSize.x, e_vSize.y, Vector4::One, l_fTextureCoordinate, Vector3::Zero);
    RenderQuadWithTextureAndColorAndCoordinate(e_vPos.x, e_vPos.y, 0.f, (float)m_Width/4, (float)m_Height/4, Vector4::One, l_fTextureCoordinate, Vector3::Zero);
}



// Debug helper: validate FBO binding, framebuffer status, viewport/scissor and force clear
void cShadowMap::ValidateAndPrepareForWrite()
{
    // check which FBO is bound
    GLint boundFBO = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFBO);
    printf("[ShadowMap] Bound FBO = %d, expected = %u\n", boundFBO, (unsigned)m_Framebuffer);

    // check framebuffer status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    printf("[ShadowMap] glCheckFramebufferStatus = 0x%X\n", status);

    // check viewport and scissor
    GLint vp[4] = { 0,0,0,0 };
    glGetIntegerv(GL_VIEWPORT, vp);
    printf("[ShadowMap] viewport = %d,%d %d x %d\n", vp[0], vp[1], vp[2], vp[3]);
    GLboolean sc = glIsEnabled(GL_SCISSOR_TEST);
    GLint scissor[4] = { 0,0,0,0 };
    if (sc) glGetIntegerv(GL_SCISSOR_BOX, scissor);
    printf("[ShadowMap] scissor enabled=%d scissor box=%d,%d %d x %d\n", sc ? 1 : 0, scissor[0], scissor[1], scissor[2], scissor[3]);

    // check texture attachment is the depth texture
    GLint attTex = 0;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &attTex);
    printf("[ShadowMap] depth attachment texture = %d (expected %u)\n", attTex, (unsigned)m_DepthTexture);

    // clear depth (ensure we are clearing the correct FBO)
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);

    // flush to be deterministic for debug
    glFinish();

    // report GL error
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        printf("[ShadowMap] GL error after validate/clear: 0x%X\n", err);
}

void cShadowMap::BindForWritingStart()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
    //ValidateAndPrepareForWrite();
    glGetIntegerv(GL_VIEWPORT, m_iViewportData);
    glViewport(0, 0, m_Width, m_Height);

    // save scissor state and set scissor to FBO extents
    glGetBooleanv(GL_SCISSOR_TEST, &m_bOldScissorEnabled);
    if (m_bOldScissorEnabled)
    {
        glGetIntegerv(GL_SCISSOR_BOX, m_iOldScissor);
    }
    // ensure scissor covers FBO so later enabling won't clip unexpectedly
    glEnable(GL_SCISSOR_TEST);
    glScissor(0, 0, m_Width, m_Height);

    // Set cull face to front for shadow map pass
    glCullFace(GL_FRONT);
    glClearDepth(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // keep scissor enabled (we set it above)
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void cShadowMap::BindForWritingEnd()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Restore cull face to back after shadow map pass
    glCullFace(GL_BACK);
    // Restore previously saved viewport so main render uses correct dimensions
    glViewport(m_iViewportData[0], m_iViewportData[1], m_iViewportData[2], m_iViewportData[3]);

    // restore scissor state
    if (m_bOldScissorEnabled)
    {
        glEnable(GL_SCISSOR_TEST);
        glScissor(m_iOldScissor[0], m_iOldScissor[1], m_iOldScissor[2], m_iOldScissor[3]);
    }
    else
    {
#ifndef UWP
        glDisable(GL_SCISSOR_TEST);
#else
        glDisable(GL_SCISSOR_TEST);
#endif
    }
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
