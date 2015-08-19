#ifndef _CAMERA_ZOOM_FUNCTION_H_
#define _CAMERA_ZOOM_FUNCTION_H_

class cCameraZoomFunction
{
	cZoomableCamera		m_ZoomableCamera;
	sMultiTouchPoints	m_MultiTouchPoints;
	cBaseImage*			m_pBGImage;
	bool				m_bSimulate2Touch;
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

#endif // !_CAMERA_ZOOM_FUNCTION_H_