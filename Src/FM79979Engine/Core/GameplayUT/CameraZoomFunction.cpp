#include "../stdafx.h"
#include "../Camera/OrthogonalCamera.h"
#include "CameraZoomFunction.h"
#include "GameApp.h"
namespace FATMING_CORE
{
	cCameraZoomFunction::cCameraZoomFunction(const char*e_strTestBGImageName)
	{
		m_fSkipGestureTime = 0.001f;
		m_pBGImage = nullptr;
		//for test
		if( e_strTestBGImageName != nullptr )
		{
			m_pBGImage = new cBaseImage(e_strTestBGImageName);
			float	l_fWidth = (float)m_pBGImage->GetWidth();
			float	l_fHeight = (float)m_pBGImage->GetHeight();
			Vector2	l_vMax(l_fWidth,l_fHeight);
			m_ZoomableCamera.SetViewData(l_vMax/10,
				Vector4(0,0,l_vMax.x,l_vMax.y),
				Vector4(0,0,l_vMax.x,l_vMax.y));
		}
		else
		{
			Vector2	l_vMax(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
			m_ZoomableCamera.SetViewData(l_vMax/10,
				Vector4(0,0,l_vMax.x,l_vMax.y),
				Vector4(0,0,l_vMax.x,l_vMax.y));		
		}
		m_bSimulate2Touch = false;
	}

	cCameraZoomFunction::~cCameraZoomFunction()
	{
		SAFE_DELETE(m_pBGImage);
	}

	void	cCameraZoomFunction::Update(float e_fElpaseTime)
	{
		bool	l_bDoPointsPosHit = m_MultiTouchPoints.bPreviousDoPointsPosHit;
#ifdef WIN32
		m_MultiTouchPoints.Update(e_fElpaseTime);
#else
		m_MultiTouchPoints = *cGameApp::m_spMultiTouchPoints;
#endif
		if(l_bDoPointsPosHit)
		{
			Vector2	l_vCenterPos = m_MultiTouchPoints.vCenterPos;
			m_vCenterPos = cOrthogonalCamera::ConvertMousePositionToWorldPosition(l_vCenterPos,cGameApp::m_svViewPortSize.Size(),m_ZoomableCamera.GetViewRect());
		}
		m_fSkipGestureTime -= e_fElpaseTime;
		if( m_fSkipGestureTime <= 0.f )
		{
			m_fSkipGestureTime = 0.001f;
			if( m_MultiTouchPoints.Gesture == eGestureEnum::eGE_ZOOM_OUT )
			{
				m_ZoomableCamera.Zoom(m_vCenterPos,1.005f);
			}
			else
			if( m_MultiTouchPoints.Gesture == eGestureEnum::eGE_ZOOM_IN )
			{
				m_ZoomableCamera.Zoom(m_vCenterPos,0.995f);
			}
			else
			{
				if( m_MultiTouchPoints.TouchPointWorkingIndex.size() == 1 )
					m_ZoomableCamera.Move(m_MultiTouchPoints.vSingleTouchMoveDis);
			}
		}

		if( m_MultiTouchPoints.bPrevoisMouseAllUp && m_ZoomableCamera.IsDoCenterBack() )
		{
			m_ZoomableCamera.SetMouseUp(true);
		}
		
		m_ZoomableCamera.Update(e_fElpaseTime);
	}

	void		cCameraZoomFunction::Render()
	{
		this->m_ZoomableCamera.GetProjection();
		if( m_pBGImage )
		{
			m_pBGImage->Render();
		}
		glEnable2D(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height());
		this->m_MultiTouchPoints.DebugRender(10);
		cGameApp::RenderFont(0.f,300.f,UT::ComposeMsgByFormat(L"%.2f,%.2f",m_vGameMousePos.x,m_vGameMousePos.y));
		cOrthogonalCamera l_OrthogonalCamera(Vector2(1024,1280));
		l_OrthogonalCamera.SetViewRect(m_ZoomableCamera.GetViewRect());
		l_OrthogonalCamera.Render();
		l_OrthogonalCamera.DrawGrid();
	}

	POINT	g_vSecondTouchPoint  = {0,0};

	void		cCameraZoomFunction::MouseDown(int e_iPosX,int e_iPosY)
	{
#ifdef WIN32
		Vector2	l_vGameResolution = cGameApp::m_svGameResolution;
		if( m_pBGImage )
		{
			cGameApp::m_svGameResolution.x = (float)m_pBGImage->GetWidth();
			cGameApp::m_svGameResolution.y = (float)m_pBGImage->GetHeight();
		}
		m_MultiTouchPoints.Touch(true,e_iPosX,e_iPosY,0);
		if( m_bSimulate2Touch )
				m_MultiTouchPoints.Touch(true,g_vSecondTouchPoint.x,g_vSecondTouchPoint.y,1);
		cGameApp::m_svGameResolution = l_vGameResolution;
		m_vGameMousePos = cOrthogonalCamera::ConvertMousePositionToWorldPosition(Vector2(e_iPosX,e_iPosY),cGameApp::m_svViewPortSize.Size(),m_ZoomableCamera.GetViewRect());
#endif
	}

	void		cCameraZoomFunction::MouseMove(int e_iPosX,int e_iPosY)
	{
#ifdef WIN32
		if(cGameApp::m_sbTouched)
		{
			Vector2	l_vGameResolution = cGameApp::m_svGameResolution;
			if( m_pBGImage )
			{
				cGameApp::m_svGameResolution.x = (float)m_pBGImage->GetWidth();
				cGameApp::m_svGameResolution.y = (float)m_pBGImage->GetHeight();
			}
			m_MultiTouchPoints.Touch(true,e_iPosX,e_iPosY,0);
			if( m_bSimulate2Touch )
				m_MultiTouchPoints.Touch(true,g_vSecondTouchPoint .x,g_vSecondTouchPoint .y,1);
			cGameApp::m_svGameResolution = l_vGameResolution;
		}
		m_vGameMousePos = cOrthogonalCamera::ConvertMousePositionToWorldPosition(Vector2(e_iPosX,e_iPosY),cGameApp::m_svViewPortSize.Size(),m_ZoomableCamera.GetViewRect());
#endif
	}

	void		cCameraZoomFunction::MouseUp(int e_iPosX,int e_iPosY)
	{
#ifdef WIN32
		Vector2	l_vGameResolution = cGameApp::m_svGameResolution;
		if( m_pBGImage )
		{
			cGameApp::m_svGameResolution.x = (float)m_pBGImage->GetWidth();
			cGameApp::m_svGameResolution.y = (float)m_pBGImage->GetHeight();
		}
		m_MultiTouchPoints.Touch(false,e_iPosX,e_iPosY,0);
		if( m_bSimulate2Touch )
			m_MultiTouchPoints.Touch(false,g_vSecondTouchPoint.x,g_vSecondTouchPoint.y,1);
		cGameApp::m_svGameResolution = l_vGameResolution;
		m_vGameMousePos = cOrthogonalCamera::ConvertMousePositionToWorldPosition(Vector2(e_iPosX,e_iPosY),cGameApp::m_svViewPortSize.Size(),m_ZoomableCamera.GetViewRect());
#endif
	}

	void		cCameraZoomFunction::KeyUp(unsigned char e_ucKey)
	{
		if( e_ucKey >= '0' && e_ucKey <= '9' )
		{
			Vector2	l_vSize = cGameApp::m_svViewPortSize.Size();
			float	l_fWidthPart = l_vSize.x/9;
			float	l_fHeightPart = l_vSize.y/9;
			int	l_iDivide = e_ucKey -'0';
			Vector2	l_vOffsetPos;
			l_vOffsetPos.x = (float)l_iDivide*l_fWidthPart;
			l_vOffsetPos.y = (float)l_iDivide*l_fHeightPart;
			g_vSecondTouchPoint.x = (int)l_vOffsetPos.x;
			g_vSecondTouchPoint.y = (int)l_vOffsetPos.y;
		}
		if( e_ucKey == 17 )
			m_bSimulate2Touch = !m_bSimulate2Touch;
		if( e_ucKey == 13 )
		{
			m_ZoomableCamera.SetMouseUp(false);
			float	l_fWidth = cGameApp::m_svGameResolution.x;
			float	l_fHeight = cGameApp::m_svGameResolution.y;
			if( m_pBGImage )
			{
				float	l_fWidth = (float)m_pBGImage->GetWidth();
				float	l_fHeight = (float)m_pBGImage->GetHeight();
			}
			Vector2	l_vMax(l_fWidth,l_fHeight);
			m_ZoomableCamera.SetViewData(l_vMax/10,
				Vector4(0,0,l_vMax.x,l_vMax.y),
				Vector4(0,0,l_vMax.x,l_vMax.y));
		}
	}
}