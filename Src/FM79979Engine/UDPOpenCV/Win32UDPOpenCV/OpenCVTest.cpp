#ifdef WIN32
#include "pch.h"
#else
#include "stdafx.h"
#endif
//#ifdef USE_OPEN_CV
#include "opencv2/opencv.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "OpenCVTest.h"


cOpenCVTest::sMatWithFlag::sMatWithFlag()
{
	pFrame = new cv::Mat;
	bNewData = false;
}
cOpenCVTest::sMatWithFlag::~sMatWithFlag()
{
	SAFE_DELETE(pFrame);
}

cOpenCVTest::cOpenCVTest()
{
	m_pVideoCapture = nullptr;
	m_pVideoImage = nullptr;
	m_pFrame = new sMatWithFlag();
}

cOpenCVTest::~cOpenCVTest()
{
	SAFE_DELETE(m_pVideoCapture);
}

void cOpenCVTest::CameraReadThread(float e_fElpaseTime)
{
	//30 fps
	//Sleep(1000 / 30);
	MUTEX_PLACE_HOLDER(m_CameraReadMutex, "cOpenCVTest::CameraReadThread");
	if (!m_pVideoCapture)
	{
		//Sleep(100);
		return;
	}
	if (!m_pVideoCapture->isOpened())
	{
		if (m_strCameraURL.length())
		{
			m_pVideoCapture->open(m_strCameraURL.c_str());
			Sleep(1000);
		}
		else
		{
			m_pVideoCapture->open(0);
		}
		return;
	}
	{
		MUTEX_PLACE_HOLDER(m_FrameMutex, "cOpenCVTest::CameraReadThread");
		*m_pVideoCapture >> *m_pFrame->pFrame;
		m_pFrame->bNewData = true;
	}
}

void cOpenCVTest::OpenCamera(const char* e_strCameraURL)
{
	m_strCameraURL = e_strCameraURL;
	SAFE_DELETE(m_pVideoCapture);
	m_pVideoCapture = new cv::VideoCapture("rkcamsrc io-mode=4 isp-mode=2A tuning-xml-path=/etc/cam_iq/IMX219.xml ! video/x-raw,format=NV12,width=640,height=360 ! videoconvert ! appsink");
}

void cOpenCVTest::Update(float e_fElpaseTime)
{
	CameraReadThread(e_fElpaseTime);
	if(m_pFrame)
	{
		MUTEX_PLACE_HOLDER(m_FrameMutex, "cOpenCVTest::CameraReadThread");
#ifdef WIN32
		GLenum inputColourFormat = GL_BGR;
#else
		GLenum inputColourFormat = GL_RGB;
#endif
		if (!m_pVideoImage)
		{
			m_pVideoImage = new cBaseImage(L"OpenCVToOpenGLTexture");
		}
		if(m_pFrame->bNewData)
		{
			MUTEX_PLACE_HOLDER(m_FrameMutex, "cOpenCVTest::CameraReadThread");
			m_pVideoImage->SetupTexture(3, m_pFrame->pFrame->rows, m_pFrame->pFrame->cols, inputColourFormat, GL_UNSIGNED_BYTE, false, (GLvoid*)m_pFrame->pFrame->data);
			m_pFrame->bNewData = false;
		}
	}
}

void cOpenCVTest::Render()
{
	if (m_pVideoImage)
	{
		m_pVideoImage->Render();
	}
}