#pragma once
#include "OpenCVTestBase.h"
//tutorial
//https://learnopencv.com/facemark-facial-landmark-detection-using-opencv/
class cOpenCVTest_FaceLandmark:public OpenCVTestBase,public cCPP11Thread
{
	cCPP11Thread		m_FaceDetectThread;
	UT::sTimeAndFPS		m_CameraFPS;
	UT::sTimeAndFPS		m_FaceLandMarkFPS;
	UT::sTimeCounter	m_30FPSLimit;
	std::string			m_strCameraURL;
	std::mutex			m_FaceDectedMutex;
	std::mutex			m_OpenGLFrameMutex;
	std::mutex			m_FaceRectMutex;
	sMatWithFlag*					m_pOpenGLFrame;
	sMatWithFlag*					m_pFaceDetectFrame;
	FATMING_CORE::cBaseImage*		m_pVideoImage;
	class cv::VideoCapture*			m_pVideoCapture;
	//class cv::CascadeClassifier*	m_pCascadeClassifier;
	//https://www.pyimagesearch.com/2018/02/26/face-detection-with-opencv-and-deep-learning/
	//class cv::CascadeClassifier*	m_FaceCascade;
	//class cv::CascadeClassifier*	m_EyesCascade;
	class cv::CascadeClassifier*	m_pCascadeClassifier;
	std::vector<cv::Rect>			m_FacesRect;
	vector< vector<cv::Point2f> >	m_FacesPointsVector;
	bool							m_bNewFaceDetectData;
	void	CameraReadThread(float e_fElpaseTime);
	void	FaceDetectThread(float e_fElpaseTime);
public:
	cOpenCVTest_FaceLandmark();
	virtual ~cOpenCVTest_FaceLandmark();
	virtual void	OpenCamera(const char* e_strCameraURL = nullptr)override;
	virtual void	Update(float e_fElpaseTime)override;
	virtual void	Render()override;
};