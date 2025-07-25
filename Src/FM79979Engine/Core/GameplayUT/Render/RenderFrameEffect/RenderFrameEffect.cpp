#include "RenderFrameEffect.h"
#include "../OpenGLObject/FrameBuffer.h"
#include "../Texture/Texture.h"
#include "../Texture/TextureManager.h"

namespace FATMING_CORE
{

	cScissorRenderObject::cScissorRenderObject()
	{
		m_vScissorRectColor = Vector4(0.5f, 0.5f, 0.5f, 1);
		m_bSissorTest = false;;
		memset(m_iOriginalSissor, 0, sizeof(m_iOriginalSissor));
		m_vScissorRect = m_vWorldPosScissorRect = Vector4::Zero;
		m_bRenderScissor = false;
	}

	cScissorRenderObject::~cScissorRenderObject()
	{
	}

	void FATMING_CORE::cScissorRenderObject::UpdateCachedWorldTransformIfNeeded()
	{
		bool l_bDirty = m_CachedWorldTransform._11 == FRAME_DIRTY_WORLD_CACHE ? true : false;
		Frame::UpdateCachedWorldTransformIfNeeded();
		if (l_bDirty)
		{
			SetScissorWithTransform();
		}
	}

	void cScissorRenderObject::SetScissorWithTransform()
	{
		auto l_vPos = this->GetWorldPosition();
		m_vWorldPosScissorRect.x = l_vPos.x + m_vScissorRect.x;
		m_vWorldPosScissorRect.y = l_vPos.y + m_vScissorRect.y;
		m_vWorldPosScissorRect.z = m_vWorldPosScissorRect.x + m_vScissorRect.Width();
		m_vWorldPosScissorRect.w = m_vWorldPosScissorRect.y + m_vScissorRect.Height();
	}

	void cScissorRenderObject::Render()
	{
		if (m_bRenderScissor)
		{
			GLRender::RenderFilledRectangle(Vector2(m_vWorldPosScissorRect.x, m_vWorldPosScissorRect.y), m_vWorldPosScissorRect.Width(), m_vWorldPosScissorRect.Height(), m_vScissorRectColor, 0.f);
		}
		MyglGetBooleanv(GL_SCISSOR_TEST, &m_bSissorTest);
		if (!m_bSissorTest)
			MyGLEnable(GL_SCISSOR_TEST);
		else
			MyGLGetIntegerv(GL_SCISSOR_BOX, m_iOriginalSissor);
		Vector4	l_vViewRect = ViewRectToOpenGLScissor(m_vWorldPosScissorRect);
		glScissor((int)l_vViewRect.x, (int)l_vViewRect.y, (int)l_vViewRect.z, (int)l_vViewRect.w);
	}

	void cScissorRenderObject::EndRender()
	{
		if (!m_bSissorTest)
			MyGLDisable(GL_SCISSOR_TEST);
		else
			glScissor(m_iOriginalSissor[0], m_iOriginalSissor[1], m_iOriginalSissor[2], m_iOriginalSissor[3]);
	}

	Vector4 cScissorRenderObject::GetScissorRect()
	{
		return m_vScissorRect;
	}

	Vector4 cScissorRenderObject::GetWorldPosScissorRect()
	{
		return m_vWorldPosScissorRect;
	}

	void cScissorRenderObject::SetScissorRect(Vector4 e_vScissorRect)
	{
		m_vScissorRect = e_vScissorRect;
		SetScissorWithTransform();
	}

	bool cScissorRenderObject::Collide(int e_iPosX, int e_iPosY)
	{
		return m_vWorldPosScissorRect.CollidePoint(e_iPosX, e_iPosY);
	}
}
cFrameBufferNode::cFrameBufferNode(unsigned int width, unsigned int height, bool depth, GLenum imageType, GLenum dataType)
    : m_uiWidth(width), m_uiHeight(height)
{
    m_pFrameBuffer = std::make_unique<cFrameBuffer>(width, height, depth, imageType, dataType);
}
cFrameBufferNode::~cFrameBufferNode() {}
void cFrameBufferNode::Render()
{
    // Start rendering to framebuffer
    m_pFrameBuffer->StartDraw(true);
    m_bIsRenderingToBuffer = true;
}
void cFrameBufferNode::EndRender()
{
    m_pFrameBuffer->EndDraw();
    m_bIsRenderingToBuffer = false;
    // Update texture reference
    GLuint texID = m_pFrameBuffer->GetTextureID();
    auto sharedTex = cTextureManager::GetObjectByPixels(nullptr, m_uiWidth, m_uiHeight, L"FrameBufferNodeTexture", texID);
    if (!m_pFrameBufferTexture || m_pFrameBufferTexture->m_uiImageIndex != texID)
    {
        m_pFrameBufferTexture.reset();
        m_pFrameBufferTexture = std::unique_ptr<cTexture>(sharedTex.get());
        m_pFrameBufferTexture->m_uiImageIndex = texID;
    }
}
cTexture* cFrameBufferNode::GetFrameBufferTexture() const
{
    return m_pFrameBufferTexture.get();
}
unsigned int cFrameBufferNode::GetWidth() const
{
    return m_uiWidth;
}
unsigned int cFrameBufferNode::GetHeight() const
{
    return m_uiHeight;
}