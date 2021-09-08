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
	std::string l_strOpenCVInfo = "OpenCV version :";	l_strOpenCVInfo += CV_VERSION;
	l_strOpenCVInfo += "\nMajor version : ";			l_strOpenCVInfo += ValueToString(CV_MAJOR_VERSION);
	l_strOpenCVInfo += "\nMinor version : ";			l_strOpenCVInfo += ValueToString(CV_MINOR_VERSION);
	l_strOpenCVInfo += "\nSubminor version : ";			l_strOpenCVInfo +=  ValueToString(CV_SUBMINOR_VERSION);
	FMLog::Log(l_strOpenCVInfo.c_str(),false);
	m_pVideoCapture = nullptr;
	m_pVideoImage = nullptr;
	m_pFrame = new sMatWithFlag();
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
	//face_cascade.detectMultiScale(
	//	gray,
	//	faces,
	//	1.4, // pyramid scale factor
	//	3,   // lower thershold for neighbors count
	//		 // here we hint the classifier to only look for one face
	//	CASCADE_SCALE_IMAGE + CASCADE_FIND_BIGGEST_OBJECT);
	face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, Size(30, 30));
}

void cOpenCVTest_FaceLandmark::CameraReadThread(float e_fElpaseTime)
{
	//30 fps
	//Sleep(1000 / 30);
	MUTEX_PLACE_HOLDER(m_CameraReadMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");	
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
	{
		{
			MUTEX_PLACE_HOLDER(m_FrameMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
			*m_pVideoCapture >> *m_pFrame->pFrame;
			// ... obtain an image in img
			std::vector<cv::Rect>			l_FacesRect;
			faceDetector(*m_pFrame->pFrame, l_FacesRect, *m_pCascadeClassifier);
			{
				vector< vector<Point2f> > shapes;
				if (g_Facemark->fit(*m_pFrame->pFrame, l_FacesRect, shapes))
				{
					//for (size_t i = 0; i < l_FacesRect.size(); i++)
					//{
					//	cv::rectangle(img, faces[i], Scalar(255, 0, 0));
					//}
					for (unsigned long i = 0; i < l_FacesRect.size(); i++)
					{
						for (unsigned long k = 0; k < shapes[i].size(); k++)
						{
							//cv::circle(img, shapes[i][k], 5, cv::Scalar(0, 0, 255), FILLED);
						}
					}
				}
				{
					MUTEX_PLACE_HOLDER(m_FaceRectMutex, "m_FaceRectMutex");
					m_FacesRect = l_FacesRect;
					m_FacesPointsVector = shapes;
				}
			}
			m_pFrame->bNewData = true;
		}
		// Check if any faces were detected or not
		//if (m_pVideoCapture->read(*m_pFrame->pFrame))
		//{
		//	m_pFrame->bNewData = true;
		//}
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
	//m_pVideoCapture = new cv::VideoCapture();
	//m_pVideoCapture->open(e_strCameraURL);
}
//opencv-4.4.0-vc14_vc15\opencv\build\testdata\cv\face\face_landmark_model.dat
void cOpenCVTest_FaceLandmark::Update(float e_fElpaseTime)
{
	CameraReadThread(e_fElpaseTime);
	if(m_pFrame)
	{
		//MUTEX_PLACE_HOLDER(m_FrameMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
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
			MUTEX_PLACE_HOLDER(m_FrameMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
			if (m_pFrame->bNewData)
			{
				m_pVideoImage->SetupTexture(3, m_pFrame->pFrame->cols, m_pFrame->pFrame->rows, inputColourFormat, GL_UNSIGNED_BYTE, false, (GLvoid*)m_pFrame->pFrame->data,false);
				m_pFrame->bNewData = false;
			}
		}
	}
}

void cOpenCVTest_FaceLandmark::Render()
{
	if (m_pVideoImage)
	{
		m_pVideoImage->Render();

		vector< vector<Point2f> >		l_Shapes;
		std::vector<cv::Rect>			l_FacesRect;
		{
			MUTEX_PLACE_HOLDER(m_FaceRectMutex, "m_FaceRectMutex");
			l_FacesRect = m_FacesRect;
			l_Shapes = m_FacesPointsVector;
		}
		for (auto l_Rect : l_FacesRect)
		{
			GLRender::RenderRectangle(Vector2(l_Rect.x, l_Rect.y), (float)l_Rect.width, (float)l_Rect.height, Vector4::Red);
		}

		for (auto l_PointsVector : l_Shapes)
		{
			for (auto l_Point : l_PointsVector)
			{
				GLRender::RenderSphere(Vector2(l_Point.x, l_Point.y), 1);
			}
		}
	}
}