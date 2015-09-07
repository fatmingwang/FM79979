#ifndef _CAMERA_ZOOM_FUNCTION_H_
#define _CAMERA_ZOOM_FUNCTION_H_

#include "MultiTouchPoints.h"
#include "../Camera/OrthogonalCamera.h"

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
	public:
		cCameraZoomFunction();
		~cCameraZoomFunction();
		void		Update(float e_fElpaseTime);
		void		Render();
		void		MouseDown(int e_iPosX,int e_iPosY);
		void		MouseUp(int e_iPosX,int e_iPosY);
		void		MouseMove(int e_iPosX,int e_iPosY);
		void		KeyUp(unsigned char e_ucKey);
	};
}
#endif // !_CAMERA_ZOOM_FUNCTION_H_