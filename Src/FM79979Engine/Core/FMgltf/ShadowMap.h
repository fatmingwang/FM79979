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
    // Shadow map shader program
    void InitShadowMapProgram();
    GLuint GetShadowMapProgram() const { return m_ShadowMapProgram; }
    void RenderFrameBufferAs2DImage(Vector2 e_vPos, Vector2 e_vSize);
private:
    GLuint m_Framebuffer = 0;
    GLuint m_DepthTexture = 0;
    int m_Width = 0;
    int m_Height = 0;
    GLuint m_ShadowMapProgram = 0;
};


extern cShadowMap* g_pShadowMap;