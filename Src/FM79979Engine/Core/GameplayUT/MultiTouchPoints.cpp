#include "../stdafx.h"
#include "MultiTouchPoints.h"
#include "../Image/SimplePrimitive.h"
#include "../GameplayUT/GameApp.h"
namespace FATMING_CORE
{
	eGestureEnum	GetGesture(const wchar_t*e_strGestureEnum)
	{	
		if( !wcscmp(e_strGestureEnum,L"ClickDown") )
			return eGestureEnum::eGE_CLICK_DOWN;
		if( !wcscmp(e_strGestureEnum,L"Horver") )
			return eGestureEnum::eGE_HORVER;
		if( !wcscmp(e_strGestureEnum,L"ClickUp") )
			return eGestureEnum::eGE_CLICK_UP;
		if( !wcscmp(e_strGestureEnum,L"DoubleClick") )
			return eGestureEnum::eGE_DOUBLE_CLICK;
		if( !wcscmp(e_strGestureEnum,L"SlideLeft") )
			return eGestureEnum::eGE_SLIDE_LEFT;
		if( !wcscmp(e_strGestureEnum,L"SlideUp") )
			return eGestureEnum::eGE_SLIDE_UP;
		if( !wcscmp(e_strGestureEnum,L"SlideRight") )
			return eGestureEnum::eGE_SLIDE_RIGHT;
		if( !wcscmp(e_strGestureEnum,L"SlideDown") )
			return eGestureEnum::eGE_SLIDE_DOWN;
		if( !wcscmp(e_strGestureEnum,L"ZoomIn") )
			return eGestureEnum::eGE_ZOOM_IN;
		if( !wcscmp(e_strGestureEnum,L"ZoomOut") )
			return eGestureEnum::eGE_ZOOM_OUT;
		assert(0&&"GestureEnum data wrong!!");
		return eGestureEnum::eGE_MAX;
	}

	const wchar_t*			ValueToStringW(eGestureEnum e_eGestureEnum)
	{
		if( e_eGestureEnum == eGestureEnum::eGE_CLICK_DOWN )
			return L"ClickDown";
		if( e_eGestureEnum == eGestureEnum::eGE_HORVER )
			return L"Horver";
		if( e_eGestureEnum == eGestureEnum::eGE_CLICK_UP )
			return L"ClickUp";
		if( e_eGestureEnum == eGestureEnum::eGE_DOUBLE_CLICK )
			return L"DoubleClick";
		if( e_eGestureEnum == eGestureEnum::eGE_SLIDE_LEFT )
			return L"SlideLeft";
		if( e_eGestureEnum == eGestureEnum::eGE_SLIDE_UP )
			return L"SlideUp";
		if( e_eGestureEnum == eGestureEnum::eGE_SLIDE_RIGHT )
			return L"SlideRight";
		if( e_eGestureEnum == eGestureEnum::eGE_SLIDE_DOWN )
			return L"SlideDown";
		if( e_eGestureEnum == eGestureEnum::eGE_ZOOM_IN )
			return L"ZoomIn";
		if( e_eGestureEnum == eGestureEnum::eGE_ZOOM_OUT )
			return L"ZoomOut";
		return L"unknow";
	}

	sMultiTouchPoints::sMultiTouchPoints(bool e_bDetectGesture)
	{
		fMinSlideDistanceForGesture = 150.f;
		bTouched = false;
		vPanDistance = Vector2::Zero;
		bDetectGesture = e_bDetectGesture;
		bDoPointsPosHit = false;
		vFrameChangePanDistance = vPanDistance = vCenterPos = Vector2::Zero;
		fCurrentFrameDisToCenterPos = 0.f;
		fAllFrameDisToCenterPos = 0.f;
		bMouseAllUp = false;
		Gesture = eGestureEnum::eGE_MAX;
		vSingleTouchMoveDis = Vector2::Zero;
		this->Init();
	}

	void	sMultiTouchPoints::Touch(bool e_bTouch,int e_iPosX,int e_iPosY,int e_iIndex)
	{
		if( e_iIndex < 0 || e_iIndex >= MAX_TOUCH_POINTS )
			return;
		bTouched = true;
		bool	l_bContain = false;
		for( size_t i=0;i<TouchPointWorkingIndex.size();++i )
		{
			if(TouchPointWorkingIndex[i] == e_iIndex)
				l_bContain = true;
		}
		TouchPointData[e_iIndex].PreviousPos = TouchPointData[e_iIndex].Pos;
		POINT	l_Viewport = {(int)cGameApp::m_svViewPortSize.Width(),(int)cGameApp::m_svViewPortSize.Height()};
		//if u need a different viewport and resolution please set cGameApp::m_svViewport and cGameApp::m_sGameResoution as need.
		TouchPointData[e_iIndex].Pos = ConvertCoordinate(e_iPosX,e_iPosY,l_Viewport);
		TouchPointData[e_iIndex].ScreenPos.x = e_iPosX;
		TouchPointData[e_iIndex].ScreenPos.y = e_iPosY;
		TouchPointData[e_iIndex].bTouched = e_bTouch;
		if( !l_bContain )
		{
			TouchPointData[e_iIndex].PreviousPos.x = TouchPointData[e_iIndex].Pos.x;
			TouchPointData[e_iIndex].PreviousPos.y = TouchPointData[e_iIndex].Pos.y;
			TouchPointWorkingIndex.push_back(e_iIndex);
			if( TouchPointWorkingIndex.size() > 1 && bDetectGesture == true && bDoPointsPosHit == false )
			{
				bDoPointsPosHit = true;
			}
		}		
		if( e_bTouch )
		{
			if( TouchPointData[e_iIndex].MouseBehavior == eOMB_NONE )
			{
				TouchPointData[e_iIndex].MouseBehavior = eOMB_FIRST_TIME_INTO;

			}
			else
			{
				TouchPointData[e_iIndex].MouseBehavior = eOMB_HORVER;
			}
		}
		else
		{//once mouseup is called set all to unuseless
			for (int i = 0; i < MAX_TOUCH_POINTS; i++)
			{
				TouchPointData[i].MouseBehavior = eOMB_NONE;
			}
			this->bMouseAllUp = true;
			//TouchPointData[e_iIndex].MouseBehavior = eOMB_UP;
		}
	}

	void	sMultiTouchPoints::Init()
	{
		TouchPointWorkingIndex.clear();
		for( int i=0;i<MAX_TOUCH_POINTS;++i )
			TouchPointData[i].MouseBehavior = eOMB_NONE;
	}

	void	sMultiTouchPoints::Update(float e_fElpaseTime)
	{
		bPreviousDoPointsPosHit = bDoPointsPosHit;
		Gesture = eGestureEnum::eGE_MAX;
		vSingleTouchMoveDis = Vector2::Zero;
		bPrevoisMouseAllUp = bMouseAllUp;
		bMouseAllUp = false;
		if( bTouched == false )
			return;
		bool	l_bMouseUp = false;
		size_t	l_iSize = TouchPointWorkingIndex.size();
		if( l_iSize < 2 )
		{
			if( TouchPointWorkingIndex.size() > 0 )
			{
				int	l_iIndex = TouchPointWorkingIndex[0];
				sTouchPointData*l_pTouchPointData = &TouchPointData[l_iIndex];
				vSingleTouchMoveDis = Vector2((float)(l_pTouchPointData->Pos.x-l_pTouchPointData->PreviousPos.x),(float)(l_pTouchPointData->Pos.y-l_pTouchPointData->PreviousPos.y));
				l_pTouchPointData->PreviousPos = l_pTouchPointData->Pos;
				if(l_pTouchPointData->MouseBehavior == eOMB_UP)
				{
					l_pTouchPointData->MouseBehavior = eOMB_NONE;
					TouchPointWorkingIndex.erase(TouchPointWorkingIndex.begin());
				}
			}
			bDoPointsPosHit = false;
		}
		else
		{
			bool	l_bDoPointsPosHit = bDoPointsPosHit;
			if( bDoPointsPosHit )
			{
				bDoPointsPosHit = false;
				vCenterPos = Vector2::Zero;
				for( size_t i=0;i<l_iSize;++i )
				{
					int	l_iIndex = TouchPointWorkingIndex[i];
					vCenterPos.x += TouchPointData[l_iIndex].Pos.x;
					vCenterPos.y += TouchPointData[l_iIndex].Pos.y;
					TouchPointData[l_iIndex].DoFirstMultiTouchPos = TouchPointData[l_iIndex].Pos;
				}
				vCenterPos /= (float)l_iSize;
				fLastFrameChangeToCenterPos = fAllFrameDisToCenterPos = fCurrentFrameDisToCenterPos = 0.f;
				vFrameChangePanDistance = vPanDistance = Vector2::Zero;
			}
			else
			{
				for( size_t i=0;i<l_iSize;++i )
				{
					int	l_iIndex = TouchPointWorkingIndex[i];
					if(TouchPointData[l_iIndex].MouseBehavior == eOMB_UP)
					{
						int	l_iIndex = TouchPointWorkingIndex[i];
						l_bMouseUp = true;
						TouchPointData[l_iIndex].MouseBehavior = eOMB_NONE;
						TouchPointWorkingIndex.erase(TouchPointWorkingIndex.begin()+i);
						--i;
						--l_iSize;
					}
				}
				if( l_iSize < 2 )
				{
					bMouseAllUp = true;
				}
			}
			if( bDetectGesture )
			{
				fLastFrameChangeToCenterPos = fCurrentFrameDisToCenterPos;
				if( bTouched )
				{
					int a=0;
				}
				float	l_fCurrentFrameDisToCenterPos = 0.f;
				float	l_fAllFrameDisToCenterPos = 0.f;
				Vector2	l_vStepDis = Vector2::Zero;
				Vector2	l_vTotalDis = Vector2::Zero;
				bool	l_bSameDirection = true;
				for(size_t i = 0; i < l_iSize; i++)
				{
					int	l_iIndex = TouchPointWorkingIndex[i];
					sTouchPointData*l_pTouchPointData = &TouchPointData[l_iIndex];
					Vector2	l_vStartPos(vCenterPos.x-l_pTouchPointData->DoFirstMultiTouchPos.x,vCenterPos.y-l_pTouchPointData->DoFirstMultiTouchPos.y);
					Vector2	l_vNewPos(vCenterPos.x-l_pTouchPointData->Pos.x,vCenterPos.y-l_pTouchPointData->Pos.y);
					Vector2	l_vPreviousPos(vCenterPos.x-l_pTouchPointData->PreviousPos.x,vCenterPos.y-l_pTouchPointData->PreviousPos.y);
					l_pTouchPointData->PreviousPos = l_pTouchPointData->Pos;
					Vector2	l_v1 = l_vNewPos-l_vPreviousPos;
					Vector2	l_v2 = l_vNewPos-l_vStartPos;
					l_vStepDis += l_v1;
					l_vTotalDis += l_v2;
					l_fCurrentFrameDisToCenterPos += l_vNewPos.Length();
					l_fAllFrameDisToCenterPos += l_vNewPos.Length();//l_vTotalDis.Length();
				}
				vPanDistance = l_vTotalDis/(float)l_iSize;
				vFrameChangePanDistance = l_vStepDis/(float)l_iSize;
				fAllFrameDisToCenterPos = l_fAllFrameDisToCenterPos/l_iSize;
				fCurrentFrameDisToCenterPos = l_fCurrentFrameDisToCenterPos/l_iSize;
				if( l_bMouseUp )
				{//do pan
					if( l_vTotalDis.x*l_iSize >= fMinSlideDistanceForGesture || 
						l_vTotalDis.y*l_iSize >= fMinSlideDistanceForGesture)
					{
						if(l_vTotalDis.x > l_vTotalDis.y)
						{//horizontal
							if( l_vTotalDis.x > 0 )
								Gesture = eGestureEnum::eGE_SLIDE_RIGHT;
							else
								Gesture = eGestureEnum::eGE_SLIDE_LEFT;
						}
						else
						{//vertical
							if( l_vTotalDis.y > 0 )
								Gesture = eGestureEnum::eGE_SLIDE_DOWN;
							else
								Gesture = eGestureEnum::eGE_SLIDE_UP;
						}
					}
				}
				else
				{//do zoom
					if( l_bDoPointsPosHit == false )
					{
						float	l_fMoveDis = fCurrentFrameDisToCenterPos-fLastFrameChangeToCenterPos;
						if( l_fMoveDis != 0.f )
						{
							std::wstring	l_str = UT::ComposeMsgByFormat(L"\nNew:%.2f,Last%.2f,Dif%.2f\n",fCurrentFrameDisToCenterPos,fLastFrameChangeToCenterPos,l_fMoveDis);
							FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
							static int	l_siTest = 0;
							l_siTest++;
							if( l_fMoveDis < 0.f )
							{
								Gesture = eGestureEnum::eGE_ZOOM_OUT;
								FMLog::LogWithFlag(ValueToStringW(l_siTest).c_str(), CORE_LOG_FLAG);
								FMLog::LogWithFlag(L"zoom out", CORE_LOG_FLAG);
							}
							else
							{
								Gesture = eGestureEnum::eGE_ZOOM_IN;
								FMLog::LogWithFlag(ValueToStringW(l_siTest).c_str(), CORE_LOG_FLAG);
								FMLog::LogWithFlag(L"zoom in", CORE_LOG_FLAG);
							}
						}
					}
					//eGE_ZOOM_IN
					//eGE_ZOOM_OUT
				}
			}
		}
		bTouched = false;
	}

	void	sMultiTouchPoints::DebugRender(float e_fRadius)
	{
		for( size_t i=0;i<TouchPointWorkingIndex.size();++i )
		{
			int	l_iIndex = TouchPointWorkingIndex[i];
			Vector2 l_Pos((float)TouchPointData[l_iIndex].Pos.x,(float)TouchPointData[l_iIndex].Pos.y);
			GLRender::RenderSphere(Vector2(l_Pos),e_fRadius);
			l_Pos = Vector2((float)TouchPointData[l_iIndex].DoFirstMultiTouchPos.x,(float)TouchPointData[l_iIndex].DoFirstMultiTouchPos.y);
			GLRender::RenderSphere(Vector2(l_Pos),e_fRadius,Vector4(1,1,0,1));
		}
		GLRender::RenderPoint(this->vCenterPos,15,Vector4::Red);
		WCHAR*l_strStatus = L"Event";
		if( fLastFrameChangeToCenterPos > fCurrentFrameDisToCenterPos )
			l_strStatus = L"Out";
		else
		if( fLastFrameChangeToCenterPos < fCurrentFrameDisToCenterPos )
			l_strStatus = L"In";
		std::wstring	l_str = UT::ComposeMsgByFormat(L"Origi:%.2f\nNew%.2f\n%s",fCurrentFrameDisToCenterPos,fLastFrameChangeToCenterPos,l_strStatus);
		cGameApp::RenderFont(100,100,l_str);
	}

//end namespace
}