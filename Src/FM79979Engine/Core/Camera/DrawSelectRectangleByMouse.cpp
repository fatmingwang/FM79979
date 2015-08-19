#include "../stdafx.h"
#include "DrawSelectRectangleByMouse.h"
#include "../GLSL/Shader.h"
#include "../Image/SimplePrimitive.h"
namespace FATMING_CORE
{
	cDrawSelectRectangleByMouse::cDrawSelectRectangleByMouse()
	{
		m_StartPos.x = m_StartPos.y = m_EndPos.x = m_EndPos.y = 0;
		m_bMouseClick = false;
	}

	cDrawSelectRectangleByMouse::~cDrawSelectRectangleByMouse()
	{
		
	}

	void	cDrawSelectRectangleByMouse::Update(bool e_bMouseClick,POINT e_CurrentMousePoint)
	{
		if( !m_bMouseClick )
		{
			if(e_bMouseClick)
			{//first time mouse click
				this->m_StartPos = this->m_EndPos = e_CurrentMousePoint;
				m_bMouseClick = true;
				return;
			}
		}
		this->m_bMouseClick = e_bMouseClick;
		//end point
		if( m_bMouseClick )
		{
			this->m_EndPos = e_CurrentMousePoint;
		}
	}

	void	cDrawSelectRectangleByMouse::Render()
	{
		if( !this->m_bMouseClick )
			return;
		//		1
		//  S---------------
		//	 |			   |
		//	2|			   |3
		//	 |_____________|E
		//			4
		glLineWidth(1.f);
		float	l_fAllVertices[] = { (float)m_StartPos.x,(float)m_StartPos.y,
									 (float)m_EndPos.x,(float)m_StartPos.y,
									 (float)m_StartPos.x,(float)m_StartPos.y,
									 (float)m_StartPos.x,(float)m_EndPos.y,
									 (float)m_EndPos.x,(float)m_EndPos.y,
									 (float)m_EndPos.x,(float)m_StartPos.y,
									 (float)m_EndPos.x,(float)m_EndPos.y,
									 (float)m_StartPos.x,(float)m_EndPos.y
									 };
		myGlVertexPointer(2,&l_fAllVertices);
		SetupShaderColor(Vector4::One);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, 8);
	}
}