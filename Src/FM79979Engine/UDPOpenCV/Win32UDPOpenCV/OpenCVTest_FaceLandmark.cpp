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
#include "OpenCVTest_FaceLandmark.h"

#include <opencv2/face.hpp>
using namespace cv;
using namespace cv::face;

// (3) Global variables
Ptr<cv::face::Facemark> g_Facemark;

cOpenCVTest_FaceLandmark::cOpenCVTest_FaceLandmark()
{
	m_bNewFaceDetectData = false;
	std::string l_strOpenCVInfo = "OpenCV version :";	l_strOpenCVInfo += CV_VERSION;
	l_strOpenCVInfo += "\nMajor version : ";			l_strOpenCVInfo += ValueToString(CV_MAJOR_VERSION);
	l_strOpenCVInfo += "\nMinor version : ";			l_strOpenCVInfo += ValueToString(CV_MINOR_VERSION);
	l_strOpenCVInfo += "\nSubminor version : ";			l_strOpenCVInfo +=  ValueToString(CV_SUBMINOR_VERSION);
	FMLog::Log(l_strOpenCVInfo.c_str(),false);
	m_pVideoCapture = nullptr;
	m_pVideoImage = nullptr;
	m_30FPSLimit.SetTargetTime(1 / 60.f);
	m_30FPSLimit.SetLoop(true);
	m_pOpenGLFrame = new sMatWithFlag();
	m_pFaceDetectFrame = new sMatWithFlag();
	g_Facemark = FacemarkLBF::create();
	g_Facemark->loadModel("opencv/lbfmodel.yaml");
	/*create the facemark instance*/
	//FacemarkLBF::Params params;
	////params.model_filename = "helen.model"; // the trained model will be saved using this filename
	//params.model_filename = "opencv/lbfmodel.yaml"; // the trained model will be saved using this filename
	//g_Facemark = FacemarkLBF::create(params);
	//g_Facemark->loadModel(params.model_filename);
	/*create the facemark instance*/
	//m_pCascadeClassifier = new cv::CascadeClassifier("opencv/haarcascade_frontalface_default.xml");
	m_pCascadeClassifier = new cv::CascadeClassifier();
	if (!m_pCascadeClassifier->load("opencv/haarcascade_frontalface_alt2.xml"))
	{
		int a = 0;
	}
	// 
	//class cv::CascadeClassifier* m_FaceCascade;
	//class cv::CascadeClassifier* m_EyesCascade;// 
	//faceDetector("haarcascade_frontalface_alt2.xml");
	//Load Face Detector
	//Ptr<Facemark> facemark = FacemarkLBF::create();
	//// Load landmark detector
	//facemark->loadModel("lbfmodel.yaml");
}

cOpenCVTest_FaceLandmark::~cOpenCVTest_FaceLandmark()
{
	this->CloseThreadAndWaitUntilFinish();
	m_FaceDetectThread.CloseThreadAndWaitUntilFinish();
	SAFE_DELETE(m_pVideoCapture);
}

void faceDetector(const Mat& image,	std::vector<Rect>& faces,CascadeClassifier& face_cascade)
{
	Mat gray;
	// The cascade classifier works best on grayscale images
	if (image.channels() > 1) 
	{
		cvtColor(image, gray, COLOR_BGR2GRAY);
	}
	else 
	{
		gray = image.clone();
	}
	// Histogram equalization generally aids in face detection
	equalizeHist(gray, gray);
	faces.clear();
	// Run the cascade classifier
	face_cascade.detectMultiScale(
		gray,
		faces,
		1.4, // pyramid scale factor
		3,   // lower thershold for neighbors count
			 // here we hint the classifier to only look for one face
		CASCADE_SCALE_IMAGE + CASCADE_FIND_BIGGEST_OBJECT);
	//face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, Size(30, 30));
}

void cOpenCVTest_FaceLandmark::CameraReadThread(float e_fElpaseTime)
{
	//UT::sTimeAndFPS	m_CameraFPS;
	m_CameraFPS.Update();
	//30 fps
	//Sleep(1000 / 30);
	//Sleep(10);
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
			auto l_b = m_pVideoCapture->open(0);
			Sleep(100);
		}
		return;
	}
	Sleep(1);
	cv::Mat l_Frame;
	*m_pVideoCapture >> l_Frame;
	{
		MUTEX_PLACE_HOLDER(m_OpenGLFrameMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
		l_Frame.copyTo(*m_pOpenGLFrame->pFrame);
		m_pOpenGLFrame->bNewData = true;
	}
	{
		MUTEX_PLACE_HOLDER(m_FaceDectedMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
		l_Frame.copyTo(*m_pFaceDetectFrame->pFrame);
		m_pFaceDetectFrame->bNewData = true;
	}
}

void	cOpenCVTest_FaceLandmark::FaceDetectThread(float e_fElpaseTime)
{
	m_30FPSLimit.Update(e_fElpaseTime);
	if (m_30FPSLimit.bTragetTimrReached)
	{
		//m_30FPSLimit.Start();
	}
	else
	{
		//Sleep(1);
		//return;
	}
	m_FaceLandMarkFPS.Update();
	cv::Mat l_Frame;
	{
		MUTEX_PLACE_HOLDER(m_FaceDectedMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
		if (m_pFaceDetectFrame->bNewData)
		{
			m_pFaceDetectFrame->pFrame->copyTo(l_Frame);
			m_pFaceDetectFrame->bNewData = false;
		}
		else
		{
			return;
		}
		
	}
	// ... obtain an image in img
	std::vector<cv::Rect>			l_FacesRect;
	faceDetector(l_Frame, l_FacesRect, *m_pCascadeClassifier);
	{
		vector< vector<Point2f> > shapes;
		if (g_Facemark->fit(l_Frame, l_FacesRect, shapes))
		{
			MUTEX_PLACE_HOLDER(m_FaceRectMutex, "m_FaceRectMutex");
			m_FacesRect = l_FacesRect;
			m_FacesPointsVector = shapes;
			m_bNewFaceDetectData = true;
		}
	}
}
//https://www.itread01.com/content/1543374489.html
void cOpenCVTest_FaceLandmark::OpenCamera(const char* e_strCameraURL)
{
	if (e_strCameraURL)
	{
		m_strCameraURL = e_strCameraURL;
	}
	SAFE_DELETE(m_pVideoCapture);
	m_pVideoCapture = new cv::VideoCapture("rkcamsrc io-mode=4 isp-mode=2A tuning-xml-path=/etc/cam_iq/IMX219.xml ! video/x-raw,format=NV12,width=640,height=360 ! videoconvert ! appsink");
	f_ThreadWorkingFunction l_f_ThreadWorkingFunction = std::bind(&cOpenCVTest_FaceLandmark::CameraReadThread, this, std::placeholders::_1);
	this->ThreadDetach(l_f_ThreadWorkingFunction, "cGameNetwork::CreateAsServer");
	f_ThreadWorkingFunction l_f_ThreadWorkingFunction2 = std::bind(&cOpenCVTest_FaceLandmark::FaceDetectThread, this, std::placeholders::_1);
	m_FaceDetectThread.ThreadDetach(l_f_ThreadWorkingFunction2, "cGameNetwork::CreateAsServer");
	//m_pVideoCapture = new cv::VideoCapture();
	//m_pVideoCapture->open(e_strCameraURL);
}
//opencv-4.4.0-vc14_vc15\opencv\build\testdata\cv\face\face_landmark_model.dat
void cOpenCVTest_FaceLandmark::Update(float e_fElpaseTime)
{
	if(m_pOpenGLFrame)
	{
#ifdef WIN32
		GLenum inputColourFormat = GL_BGR;
#else
		GLenum inputColourFormat = GL_RGB;
#endif
		if (!m_pVideoImage)
		{
			//m_pVideoImage = new cBaseImage(L"OpenCVToOpenGLTexture");
			m_pVideoImage = new FATMING_CORE::cBaseImage((const char*)"");
			m_pVideoImage->SetName(L"OpenCVToOpenGLTexture");
		}
		{
			MUTEX_PLACE_HOLDER(m_OpenGLFrameMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
			if (m_pOpenGLFrame->bNewData)
			{
				m_pVideoImage->SetupTexture(3, m_pOpenGLFrame->pFrame->cols, m_pOpenGLFrame->pFrame->rows, inputColourFormat, GL_UNSIGNED_BYTE, false, (GLvoid*)m_pOpenGLFrame->pFrame->data,false);
				m_pOpenGLFrame->bNewData = false;
			}
		}
	}
}

void cOpenCVTest_FaceLandmark::Render()
{
	if (m_pVideoImage)
	{
		float l_fScale = 1.f;
		auto l_pTex = m_pVideoImage->GetTexture();
		if (l_pTex)
		{
			if (l_pTex->GetWidth() <= 640)
			{
				l_fScale = 2.f;
				m_pVideoImage->SetWidth(l_pTex->GetWidth() * l_fScale);
				m_pVideoImage->SetHeight(l_pTex->GetHeight() * l_fScale);
			}
		}
		m_pVideoImage->Render();
		bool l_bNewFace = m_bNewFaceDetectData;
		vector< vector<Point2f> >		l_Shapes;
		std::vector<cv::Rect>			l_FacesRect;
		{
			MUTEX_PLACE_HOLDER(m_FaceRectMutex, "m_FaceRectMutex");
			l_FacesRect = m_FacesRect;
			l_Shapes = m_FacesPointsVector;
			m_bNewFaceDetectData = false;
		}
		if (!l_bNewFace)
		{
			cGameApp::RenderFont(0, 500, l_bNewFace ? L"New" : L"Old");
		}
		for (auto l_Rect : l_FacesRect)
		{
			l_Rect.x *= l_fScale; l_Rect.y *= l_fScale;
			l_Rect.width *= l_fScale; l_Rect.height *= l_fScale;
			GLRender::RenderRectangle(Vector2(l_Rect.x, l_Rect.y), (float)l_Rect.width, (float)l_Rect.height, Vector4::Red);
		}
		//https://github.com/YuvalNirkin/find_face_landmarks/blob/master/sequence_face_landmarks/utilities.cpp
		for (auto l_PointsVector : l_Shapes)
		//if(l_Shapes.size())
		{
			//auto l_PointsVector = l_Shapes[0];
			if (l_PointsVector.size() == 68)
			{
				//chink,0~16
				//left eyebow 17~21
				//right eyebow 22~26
				//left eye 36~41
				//right eye 42~47
				//nose 27~35
				//outter lip 48~59
				//inner  lip  60~67
				struct sDrawLine
				{
					int iStart;
					int iEnd;
					Vector4 vColor;
				};
				const int l_iNum = 8;
				sDrawLine l_DrawLine[l_iNum] =
				{
					{0,16,Vector4(1,0,0,1)},
					{17,21,Vector4(0,1,0,1)},
					{22,26,Vector4(0,0,1,1)},
					{36,41,Vector4(1,1,0,1)},
					{42,47,Vector4(0,1,1,1)},
					{27,35,Vector4(1,0,1,1)},
					{48,59,Vector4(0,0,0,1)},
					{60,67,Vector4(0.5,0.5,0.5,1)},
				};
				for (int i = 0; i < l_iNum; ++i)
				{
					sDrawLine l_DrawLine2 = l_DrawLine[i];
					std::vector<Vector2>l_vVector;
					for (int j = l_DrawLine2.iStart; j <= l_DrawLine2.iEnd;++j)
					{
						auto l_Point = l_PointsVector[j];
						l_vVector.push_back(Vector2(l_Point.x* l_fScale, l_Point.y* l_fScale));
					}
					GLRender::RenderLine(&l_vVector, l_DrawLine2.vColor);
				}
			}
			else
			{
				for (auto l_Point : l_PointsVector)
				{
					GLRender::RenderSphere(Vector2(l_Point.x* l_fScale, l_Point.y* l_fScale), 1);
				}
			}
		}
		std::wstring l_str = L"CameraFPS:";
		l_str += ValueToStringW(m_CameraFPS.GetFPS());
		l_str += L"\n";
		l_str += L"FaceDetecFPS:";
		l_str += ValueToStringW(m_FaceLandMarkFPS.GetFPS());
		l_str += L"\n";
		cGameApp::RenderFont(700, 0, l_str.c_str());
		
	}
}