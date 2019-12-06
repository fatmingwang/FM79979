#pragma once
#include "../../../Common/CommonDefine.h"
#include "../../../AllMathInclude.h"
namespace FATMING_CORE
{
	//===========================================
	//the reason why we do want a global sMouseMoveData
	//because the button event just spend a bit calculate works,
	//and we might need to work at multi touch so just keep every mouse has a sMouseMoveData
	//so we could get all touch's data
	//===========================================
	struct  sMouseMoveData
	{
		//the pos for mouse down
		POINT   DownPos;
		//the pos for mouse up
		POINT   UpPos;
		//the pos for mouse move
		POINT   MovePos;
		//the position for how mouse move in each mouse move call.
		POINT	FrameMovePos;
		UINT	MouseDownTime;
		float   fDownToUpElpaseTime;
		//for mouse double click time
		UINT	uiLastMouseUpClickElpaseTime;

		sMouseMoveData() { MouseDownTime = 0; fDownToUpElpaseTime = 0.f; FrameMovePos.x = FrameMovePos.y = DownPos.x = DownPos.y = UpPos.x = UpPos.y = MovePos.x = MovePos.y = 0; uiLastMouseUpClickElpaseTime = 0; }
		~sMouseMoveData() {}
		void		Init();
		float		GetDownElpaseTime();
		float		DownUpDistance(Vector2 *e_pvDirection = 0);
		float		DownMoveDistance(Vector2 *e_pvDirection = 0);
		Vector2		DownUp();
		Vector2		DownMove();
		//
		void		MouseDown(int e_iPosX, int e_iPosY);
		void		MouseMove(int e_iPosX, int e_iPosY);
		//if true double click is true or not
		bool		MouseUp(int e_iPosX, int e_iPosY);
		bool		IsDownUpDistanceSamePoint(Vector2 *e_pvDirection = 0);
	};
//end namespace FATMING_CORE
}