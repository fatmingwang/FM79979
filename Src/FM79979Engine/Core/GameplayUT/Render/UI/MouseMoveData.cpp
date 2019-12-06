#include "MouseMoveData.h"
#include "../../GameUTDefine.h"
#include "../../../Common/CommonApp.h"
namespace FATMING_CORE
{
	float   sMouseMoveData::DownUpDistance(Vector2 *e_pvDirection)
	{
		Vector2 l_vDirection((float)(UpPos.x - DownPos.x), (float)(UpPos.y - DownPos.y));
		if (e_pvDirection)
			*e_pvDirection = l_vDirection;
		return l_vDirection.Length();
	}

	bool	sMouseMoveData::IsDownUpDistanceSamePoint(Vector2 *e_pvDirection)
	{
		float l_fDistance = DownUpDistance(e_pvDirection);
		if (l_fDistance < DOUBLE_CLICK_RESTRICTION_DISTANCE)
		{
			return true;
		}
		return false;
	}

	float   sMouseMoveData::DownMoveDistance(Vector2 *e_pvDirection)
	{
		Vector2 l_vDirection((float)(MovePos.x - DownPos.x), (float)(MovePos.y - DownPos.y));
		if (e_pvDirection)
			*e_pvDirection = l_vDirection;
		return l_vDirection.Length();
	}
	Vector2   sMouseMoveData::DownUp()
	{
		return Vector2((float)(DownPos.x - UpPos.x), (float)(DownPos.y - UpPos.y));
	}

	Vector2   sMouseMoveData::DownMove()
	{
		return Vector2((float)(DownPos.x - MovePos.x), (float)(DownPos.y - MovePos.y));
	}
	void	sMouseMoveData::Init()
	{
		fDownToUpElpaseTime = 0.f;
		MouseDownTime = 0;
	}

	float   sMouseMoveData::GetDownElpaseTime()
	{
		if (MouseDownTime == 0)
			return 0.f;
		return (cCommonApp::m_sTimeAndFPS.uiCurrentTime - MouseDownTime) / 1000.f;
	}
	void	sMouseMoveData::MouseDown(int e_iPosX, int e_iPosY)
	{
		FrameMovePos.x = FrameMovePos.y = 0;
		DownPos.x = e_iPosX;
		DownPos.y = e_iPosY;
		MovePos = DownPos;
		UpPos = DownPos;
		MouseDownTime = cCommonApp::m_sTimeAndFPS.uiCurrentTime;
		fDownToUpElpaseTime = 0.f;
	}
	void	sMouseMoveData::MouseMove(int e_iPosX, int e_iPosY)
	{
		FrameMovePos.x = e_iPosX - MovePos.x;
		FrameMovePos.y = e_iPosY - MovePos.y;
		MovePos.x = e_iPosX;
		MovePos.y = e_iPosY;
	}

	bool	sMouseMoveData::MouseUp(int e_iPosX, int e_iPosY)
	{
		bool	l_bDoubleclick = false;
		UpPos.x = e_iPosX;
		UpPos.y = e_iPosY;
		fDownToUpElpaseTime = (cCommonApp::m_sTimeAndFPS.uiCurrentTime - MouseDownTime) / 1000.f;
		//is first mouse up ?check distance and time.
		if (IsDownUpDistanceSamePoint())
		{
			if (uiLastMouseUpClickElpaseTime == 0)//mouse up
			{
				uiLastMouseUpClickElpaseTime = cCommonApp::m_sTimeAndFPS.uiCurrentTime;
			}//for double click check
			else
			{
				float	l_fLastMouseUptime = (cCommonApp::m_sTimeAndFPS.uiCurrentTime - uiLastMouseUpClickElpaseTime) / 1000.f;
				if (l_fLastMouseUptime <= DOUBLE_CLICK_TIME_SPAN)
				{
					l_bDoubleclick = true;
				}
				uiLastMouseUpClickElpaseTime = 0;
			}
		}
		return l_bDoubleclick;
	}
}