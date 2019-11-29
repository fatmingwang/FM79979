#pragma once
#include "../../AllImageInclude.h"
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
//end namespace FATMING_CORE
}