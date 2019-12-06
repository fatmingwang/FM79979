#pragma once
#include "../Render/CommonRender/BaseImage.h"
#include "../MultiTouch/MultiTouchPoints.h"
#include "OrthogonalCamera.h"

namespace FATMING_CORE
{
	class cCameraZoomFunction
	{
		cZoomableCamera		m_ZoomableCamera;
		sMultiTouchPoints	m_MultiTouchPoints;
		cBaseImage*			m_pBGImage;
		bool				m_bSimulate2Touch;
		Vector2				m_vCenterPos;
		Vector2				m_vGameMousePos;
		//avoid zoom too fast
		float				m_fSkipGestureTime;
	public:
		cCameraZoomFunction(const char*e_strTestBGImageName = nullptr);
		~cCameraZoomFunction();
		void		Update(float e_fElpaseTime);
		void		Render();
		void		MouseDown(int e_iPosX,int e_iPosY);
		void		MouseUp(int e_iPosX,int e_iPosY);
		void		MouseMove(int e_iPosX,int e_iPosY);
		void		KeyUp(unsigned char e_ucKey);
	};
}