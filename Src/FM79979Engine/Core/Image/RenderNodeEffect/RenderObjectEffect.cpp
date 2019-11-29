#include "../../stdafx.h"
#include "RenderObjectEffect.h"

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
		GLRender::RenderFilledRectangle(Vector2(m_vWorldPosScissorRect.x, m_vWorldPosScissorRect.y), m_vWorldPosScissorRect.Width(), m_vWorldPosScissorRect.Height(), m_vScissorRectColor,0.f);
	}
	glGetBooleanv(GL_SCISSOR_TEST, &m_bSissorTest);
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
