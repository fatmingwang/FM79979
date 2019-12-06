#pragma once
#include "../../Common/CommonDefine.h"
namespace FATMING_CORE
{
/**
	draw mouse select rectangle.

	@ingroup FATMING_CORE
*/
	class cDrawSelectRectangleByMouse
	{
		GET_SET_DEC(POINT,m_StartPos,GetStartPos,SetStartPos);
		GET_SET_DEC(POINT,m_EndPos,GetEndPos,SetEndPos);
		GET_SET_DEC(bool,m_bMouseClick,IsClick,SetClick);
	public:
		cDrawSelectRectangleByMouse();
		virtual ~cDrawSelectRectangleByMouse();
		void					Update(bool e_bMouseClick,POINT e_CurrentMousePoint);
		void					Render();
	};
}