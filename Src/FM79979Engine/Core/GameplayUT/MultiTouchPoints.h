#ifndef _MULTI_TOUCH_POINTS_
#define _MULTI_TOUCH_POINTS_

//#include "Mouse_Touch.h"
//#include "../Camera/MouseCamera.h"
//#ifdef ANDROID
//#include <android/input.h>
//#endif
namespace FATMING_CORE
{

	//here should get 2 points to decide its gesture
	//ex:pan(2 fingers move),zoom,
	//if its pan 2 fingers move direction could be dectec by direction's angle
	//if angle is bigger than 90(or specific one) is should be zoom,but it's possible
	//one finger move andother is stay at same position.
	//if zoom gesture its mean 2 finger's direction's angle is bigger than 90.
	//current it only support 2 touch points.

	//mouse left button for two fingers pan
	//mouse mid button for 2 fingers zoom
	#define	MAX_TOUCH_POINTS		40//although its four but it only works for 2 points
	enum class eGestureEnum
	{
		eGE_CLICK_DOWN = 0,	//click with collision range
		eGE_HORVER,			//stay at same position for a while
		eGE_CLICK_UP,		//click up
		eGE_DOUBLE_CLICK,	//click twice with collision range at a short time//DOUBLE_CLICK_TIME_SPAN
		eGE_SLIDE_LEFT,		//for pan while mouse up
		eGE_SLIDE_UP,		//for pan while mouse up
		eGE_SLIDE_RIGHT,	//for pan while mouse up
		eGE_SLIDE_DOWN,		//for pan while mouse up
		eGE_ZOOM_IN,		//
		eGE_ZOOM_OUT,		//
		eGE_MAX,
	};

	eGestureEnum	GetGesture(const wchar_t*e_strGestureEnum);
	const wchar_t*	ValueToStringW(eGestureEnum e_eGestureEnum);
	struct	sMultiTouchPoints
	{
		bool				bDetectGesture;
		//while first time multitouch is actived
		bool				bDoPointsPosHit;
		bool				bPreviousDoPointsPosHit;
		Vector2				vCenterPos;
		//for points zoom function
		float				fAllFrameDisToCenterPos;//all frame point's average distance to center 
		float				fLastFrameChangeToCenterPos;
		float				fCurrentFrameDisToCenterPos;//for frame's update data
		//for points pan
		float				fMinSlideDistanceForGesture;
		Vector2				vPanDistance;//
		Vector2				vFrameChangePanDistance;//
		//
		Vector2				vSingleTouchMoveDis;
		sMultiTouchPoints(bool e_bDetectGesture = true);
		//for devide detect point index
		//int				iIndex;
		struct	sTouchPointData
		{
			POINT					DoFirstMultiTouchPos;
			POINT					PreviousPos;
			eObjectMouseBehavior	MouseBehavior;
			//sMouseMoveData		MouseMoveData;
			//the point convert by cGameApp::m_svGameResolution
			POINT					Pos;
			//the position from device.
			POINT					ScreenPos;//
			//
			bool					bTouched;
			//The pressure generally ranges from 0 (no pressure at all) to 1 (normal pressure), although values higher than 1 may be generated depending on the calibration of * the input device.
			float					fPressure;
			sTouchPointData(){ MouseBehavior = eOMB_NONE; }
			//sTouchPointData(){ iID = -1; }
		};
		//
		sTouchPointData		TouchPointData[MAX_TOUCH_POINTS];
		std::vector<int>	TouchPointWorkingIndex;
		//
		eGestureEnum	Gesture;
		bool			bTouched;
		bool			bMouseAllUp;
		bool			bPrevoisMouseAllUp;
		//I am assume all points only call mouse event one time
//#ifdef ANDROID
//		void			updateTouchPositions(AInputEvent* event);
//#endif
		void			Touch(bool e_bTouch,int e_iPosX,int e_iPosY,int e_iIndex);
		void			Init();
		void			Update(float e_fElpaseTime);
		void			DebugRender(float e_fRadius);
		bool			IsMultiTouch(){ return TouchPointWorkingIndex.size()>0?true:false; }
		//so far I am not test it yet,so comment below
		//void			Update(float e_fElpaseTime);
		//sMouseState		GetCameraMouseState();
	};
//end namespace FATMING_CORE
}
#endif