#ifndef ORAW_SELECT_RECTANGLE_BY_MOUSE_H
#define	ORAW_SELECT_RECTANGLE_BY_MOUSE_H
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
#endif