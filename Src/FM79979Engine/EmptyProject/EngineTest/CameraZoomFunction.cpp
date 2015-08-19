#include "stdafx.h"
#include "CameraZoomFunction.h"
//
//cBaseImage*g_pBG = 0;
//	g_pBG = new cBaseImage();

cCameraZoomFunction::cCameraZoomFunction()
{
	m_pBGImage = new cBaseImage("Area_NorthEurope.png");
	float	l_fWidth = (float)m_pBGImage->GetWidth();
	float	l_fHeight = (float)m_pBGImage->GetHeight();
	Vector2	l_vMax(l_fWidth,l_fHeight);
	m_ZoomableCamera.SetViewData(l_vMax/10,
		Vector4(0,0,l_vMax.x,l_vMax.y),
		Vector4(0,0,l_vMax.x,l_vMax.y),	l_vMax/100);
	m_bSimulate2Touch = false;
}

cCameraZoomFunction::~cCameraZoomFunction()
{
	SAFE_DELETE(m_pBGImage);
}

void	cCameraZoomFunction::Update(float e_fElpaseTime)
{
	m_MultiTouchPoints.Update(e_fElpaseTime);
	if( m_MultiTouchPoints.Gesture == eGestureEnum::eGE_ZOOM_OUT )
	{
		m_ZoomableCamera.Zoom(m_MultiTouchPoints.vCenterPos,1.005f);
	}
	else
	if( m_MultiTouchPoints.Gesture == eGestureEnum::eGE_ZOOM_IN )
	{
		m_ZoomableCamera.Zoom(m_MultiTouchPoints.vCenterPos,0.995f);
	}
	else
	{
		if( m_MultiTouchPoints.TouchPointWorkingIndex.size() == 1 )
			m_ZoomableCamera.Move(m_MultiTouchPoints.vSingleTouchMoveDis);
	}

	if( m_MultiTouchPoints.bMouseAllUp && m_ZoomableCamera.IsDoCenterBack() )
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
		float	l_fMat[16];
		m_pBGImage->Render();
		glhOrthof2(l_fMat,0,(float)m_pBGImage->GetWidth(),(float)m_pBGImage->GetHeight(),0, -10000, 10000);
		FATMING_CORE::SetupShaderViewProjectionMatrix( l_fMat,true );
	}
	this->m_MultiTouchPoints.DebugRender(10);
}

POINT	g_vSecondTouchPoint  = {0,0};

void		cCameraZoomFunction::MouseDown(int e_iPosX,int e_iPosY)
{
	Vector2	l_vGameResolution = cGameApp::m_svGameResolution;
	cGameApp::m_svGameResolution.x = (float)m_pBGImage->GetWidth();
	cGameApp::m_svGameResolution.y = (float)m_pBGImage->GetHeight();
	m_MultiTouchPoints.Touch(true,e_iPosX,e_iPosY,0);
	if( m_bSimulate2Touch )
		//m_MultiTouchPoints.Touch(true,(int)cGameApp::m_svViewPortSize.Width()/2,(int)cGameApp::m_svViewPortSize.Height()/4,1);
			m_MultiTouchPoints.Touch(true,g_vSecondTouchPoint.x,g_vSecondTouchPoint.y,1);
	cGameApp::m_svGameResolution = l_vGameResolution;
}

void		cCameraZoomFunction::MouseMove(int e_iPosX,int e_iPosY)
{
	if(cGameApp::m_sbTouched)
	{
		Vector2	l_vGameResolution = cGameApp::m_svGameResolution;
		cGameApp::m_svGameResolution.x = (float)m_pBGImage->GetWidth();
		cGameApp::m_svGameResolution.y = (float)m_pBGImage->GetHeight();
		m_MultiTouchPoints.Touch(true,e_iPosX,e_iPosY,0);
		if( m_bSimulate2Touch )
			//m_MultiTouchPoints.Touch(true,(int)cGameApp::m_svViewPortSize.Width()/2,(int)cGameApp::m_svViewPortSize.Height()/4,1);
			m_MultiTouchPoints.Touch(true,g_vSecondTouchPoint .x,g_vSecondTouchPoint .y,1);
		cGameApp::m_svGameResolution = l_vGameResolution;
	}
}

void		cCameraZoomFunction::MouseUp(int e_iPosX,int e_iPosY)
{
	Vector2	l_vGameResolution = cGameApp::m_svGameResolution;
	cGameApp::m_svGameResolution.x = (float)m_pBGImage->GetWidth();
	cGameApp::m_svGameResolution.y = (float)m_pBGImage->GetHeight();
	m_MultiTouchPoints.Touch(false,e_iPosX,e_iPosY,0);
	if( m_bSimulate2Touch )
		//m_MultiTouchPoints.Touch(false,(int)cGameApp::m_svViewPortSize.Width()/4,(int)cGameApp::m_svViewPortSize.Height()/4,1);
		m_MultiTouchPoints.Touch(false,g_vSecondTouchPoint.x,g_vSecondTouchPoint.y,1);
	cGameApp::m_svGameResolution = l_vGameResolution;
}

void		cCameraZoomFunction::KeyUp(unsigned char e_ucKey)
{
	if( e_ucKey >= '1' && e_ucKey <= '9' )
	{
		int	l_iWidthPart = (int)cGameApp::m_svViewPortSize.Width()/9;
		int	l_iHeightPart = (int)cGameApp::m_svViewPortSize.Height()/9;
		int	l_iDivide = e_ucKey -'0';
		g_vSecondTouchPoint.x = l_iDivide*l_iWidthPart;
		g_vSecondTouchPoint.y = l_iDivide*l_iHeightPart;
	}
	if( e_ucKey == 17 )
		m_bSimulate2Touch = !m_bSimulate2Touch;
	if( e_ucKey == 13 )
	{
		m_ZoomableCamera.SetMouseUp(false);
		float	l_fWidth = (float)m_pBGImage->GetWidth();
		float	l_fHeight = (float)m_pBGImage->GetHeight();
		Vector2	l_vMax(l_fWidth,l_fHeight);
		m_ZoomableCamera.SetViewData(l_vMax/10,
			Vector4(0,0,l_vMax.x,l_vMax.y),
			Vector4(0,0,l_vMax.x,l_vMax.y),
			l_vMax/100);
	}
}