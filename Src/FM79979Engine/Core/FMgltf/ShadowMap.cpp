#include "ShadowMap.h"

cShadowMap::cShadowMap() {}
cShadowMap::~cShadowMap() { Cleanup(); }

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
    float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    bool status = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return status;
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
    m_DepthTexture = 0;
    m_Framebuffer = 0;
}
