#pragma once


class cChangeQRCodePhotoToDecode
{
	UT::sTimeCounter	m_TimeToScan;
	cCameraZoomFunction*m_pCameraZoomFunction;
	Vector4				m_vDetectQRCodeWall;
	bool				DoDecodeByPixelreadQRCode();
	cUIImage*			m_pCaptureImage;
public:
	cChangeQRCodePhotoToDecode(const char*e_strFileName);
	~cChangeQRCodePhotoToDecode();
	void	Update(float e_fElpaseTime);
	void	Render();
	//
	bool		m_bSucess;
	std::string m_strResult;
	void				MouseDown(int e_iPosX, int e_iPosY);
};