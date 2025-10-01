#pragma once
#include "../AllCoreInclude.h"

class cShadowMap
{
public:
    cShadowMap();
    ~cShadowMap();
    bool Init(int width, int height);
    void BindForWriting();
    void BindForReading(GLenum textureUnit);
    GLuint GetDepthTexture() const { return m_DepthTexture; }
    void Cleanup();
private:
    GLuint m_Framebuffer = 0;
    GLuint m_DepthTexture = 0;
    int m_Width = 0;
    int m_Height = 0;
};
