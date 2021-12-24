#ifndef	_GAME_UT_DEFINE_H_
#define	_GAME_UT_DEFINE_H_

#include "../Common/CommonDefine.h"

#define	ONE_QUAD_IS_TWO_TRIANGLES					2
#define	A_QUAD_4_TRIANGLES							4
#define	TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT			6           //two triangle could be a quad,one triangle is 3 two triangle is 6
#define	TWO_TRIANGLE_VERTICES_UV_TO_QUAD_UV_COUNT	12    //2 trianle is 6 vertices,each vertex has U and V 6*2=12
#define	TWO_TRIANGLE_STRIP_UV_TO_QUAD_UV_COUNT		8     //2 triangles strip are 4 vertices equal one quad.

#define	DOUBLE_CLICK_TIME_SPAN	0.1f
#define	MOUSE_UP_TIME_OFFSET	0.01f
#define	DOUBLE_CLICK_RESTRICTION_DISTANCE	10
#define	MIN_TWO_FINGERS_MOVE_DISTANCE	50.f

enum    eOrientation
{
    eO_HORIZONTAL = 0,//x
    eO_VERTICAL,//y
	eO_BOTH,//x,y
    eO_NONE,
};

inline	eOrientation	GetOrientation(const wchar_t*e_str)
{
	eOrientation	l_eOrientation = eO_NONE;
	if( !wcscmp(e_str,L"Horizontal") )
		l_eOrientation = eO_HORIZONTAL;
	else
	if( !wcscmp(e_str,L"Vertical") )
		l_eOrientation = eO_VERTICAL;
	return l_eOrientation;
}

inline	const wchar_t*	ValueToString(eOrientation e_eOrientation)
{
	if( e_eOrientation == eO_HORIZONTAL)
		return L"Horizontal";
	else
	if( e_eOrientation == eO_VERTICAL)
		return L"Vertical";
	return L"None";
}

enum    eDirection
{
    eD_LEFT = 0,
	eD_LEFT_UP,
    eD_UP,
	eD_RIGHT_UP,
    eD_RIGHT,
	eD_RIGHT_DOWN,
	eD_DOWN,
	eD_LEFT_DOWN,
	eD_MAX//non direction
};

enum    eSelecteObjectState
{
    eSOS_IDLE = 0,
    eSOS_HORVER,
    eSOS_SELECTED,
    eSOS_MAX = 3,
};

enum    eObjectMouseBehavior
{
    eOMB_NONE = 0,
    eOMB_FIRST_TIME_INTO,
    eOMB_HORVER,
    eOMB_LEAVE,
    eOMB_UP,
    eOMB_DOUBLU_CLICK_UP,
	eOMB_MAX,
	//eOMB_SLIDE_LEFT,		//
	//eOMB_SLIDE_UP,		//
	//eOMB_SLIDE_RIGHT,	//
	//eOMB_SLIDE_DOWN,		//
	//eOMB_ZOOM_IN,		//
	//eOMB_ZOOM_OUT,		//
};

inline	const wchar_t*	ValueToString(eObjectMouseBehavior e_eObjectMouseBehavior)
{
	switch(e_eObjectMouseBehavior)
	{
		case eOMB_NONE:
			return L"None";
		case eOMB_FIRST_TIME_INTO:
			return L"FirstTimeInto";
		case eOMB_HORVER:
			return L"Horver";
		case eOMB_LEAVE:
			return L"Leave";
		case eOMB_UP:
			return L"Up";
		case eOMB_DOUBLU_CLICK_UP:
			return L"DoubleClickUp";
		default:
			return L"unknow";
			break;
	}
	return L"unknow";
}

enum	eDeviceDirection
{
	eDD_PORTRAIT = 0,		//for iphone original orientations,also for original windows orientation
	eDD_UPSIDE_DOWN,		//
	eDD_LANDSCAPE_LEFT,		//home button art left
	eDD_LANDSCAPE_RIGHT,	//
	eDD_MAX
};

#endif