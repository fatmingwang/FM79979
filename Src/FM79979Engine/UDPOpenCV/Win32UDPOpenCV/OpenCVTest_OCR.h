#pragma once

#include "OpenCVTestBase.h"
//tutorial
//https://www.cnblogs.com/zx-admin/p/14204029.html
//tarnnied model
//https://github.com/tesseract-ocr/tessdata_best


//new one?
//https://www.twblogs.net/a/5c80e028bd9eee35fc136515

class cOpenCVTest_OCR:public OpenCVTestBase
{
	std::string		m_strCameraURL;
	std::mutex		m_CameraReadMutex;
	std::mutex		m_FrameMutex;
	std::mutex		m_FaceRectMutex;
	sMatWithFlag* m_pFrame;
	FATMING_CORE::cBaseImage*		m_pVideoImage;
	class cv::VideoCapture*			m_pVideoCapture;
	class cv::CascadeClassifier*	m_pCascadeClassifier;
	std::vector<cv::Rect>			m_FacesRect;
	vector< vector<cv::Point2f> >	m_FacesPointsVector;
	void	CameraReadThread(float e_fElpaseTime);
public:
	cOpenCVTest_OCR();
	virtual ~cOpenCVTest_OCR();
	virtual void	OpenCamera(const char* e_strCameraURL = nullptr)override;
	virtual void	Update(float e_fElpaseTime)override;
	virtual void	Render()override;
};