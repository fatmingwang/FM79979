#pragma once
#include "../CommonRender/RenderObject.h"
namespace FATMING_CORE
{
	//render has multi buttons situation is hard to handle.
	//
	//situation for scroll
	//child 
	//

	class cScissorRenderObject :public cRenderObject
	{
		GLboolean		m_bSissorTest;
		int				m_iOriginalSissor[4];
		Vector4			m_vScissorRect;
		Vector4			m_vWorldPosScissorRect;
		//
		GET_SET_DEC(bool, m_bRenderScissor, IsRenderScissor, SetRenderScissor);
		GET_SET_DEC(Vector4, m_vScissorRectColor, GetScissorRectColor, SetScissorRectColor);
		virtual	void    UpdateCachedWorldTransformIfNeeded()override;
	public:
		cScissorRenderObject();
		virtual ~cScissorRenderObject();
		virtual	void	Render()override;
		virtual	void	EndRender()override;
		Vector4			GetScissorRect();
		Vector4			GetWorldPosScissorRect();
		void			SetScissorRect(Vector4 e_vScissorRect);
		virtual bool	Collide(int e_iPosX, int e_iPosY)override;
		void			SetScissorWithTransform();
	};

	class cFrameBufferNode : public cRenderObject
	{
		std::unique_ptr<class cFrameBuffer> m_pFrameBuffer;
		std::unique_ptr<cTexture> m_pFrameBufferTexture;
		bool m_bIsRenderingToBuffer = false;
		unsigned int m_uiWidth = 0;
		unsigned int m_uiHeight = 0;
	public:
		cFrameBufferNode(unsigned int width, unsigned int height, bool depth = true, GLenum imageType = GL_RGBA, GLenum dataType = GL_UNSIGNED_BYTE);
		virtual ~cFrameBufferNode();
		virtual void Render() override;
		virtual void EndRender() override;
		cTexture* GetFrameBufferTexture() const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
	};
//end namespace FATMING_CORE
}