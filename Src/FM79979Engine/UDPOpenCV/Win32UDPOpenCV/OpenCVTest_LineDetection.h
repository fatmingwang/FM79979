#pragma once
#include "OpenCVTestBase.h"
//https://stackoverflow.com/questions/45322630/how-to-detect-lines-in-opencv
//http://www.magicandlove.com/blog/2015/07/21/opencv-and-processing-19/

//https://github.com/opencv/opencv_contrib/blob/master/modules/ximgproc/samples/fld_lines.cpp
#include "opencv2/opencv.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ximgproc/fast_line_detector.hpp>
class cOpenCVTest_LineDetection :public OpenCVTestBase, public cCPP11Thread
{
	cv::Ptr<cv::ximgproc::FastLineDetector>m_LineDetection;
	//
	std::vector<Vector2>			m_DetectLineVector;
	//
	std::string						m_strCameraURL;
	UT::sTimeAndFPS					m_CameraFPS;
	UT::sTimeAndFPS					m_RecognizerLineFPS;
	std::mutex						m_OpenGLFrameMutex;
	std::mutex						m_LineVectorMutex;
	std::mutex						m_LineFrameMutex;
	sMatWithFlag*					m_pOpenGLFrame;
	sMatWithFlag*					m_pLineDetectFrame;
	sMatWithFlag*					m_pGrayFrame;
	cCPP11Thread					m_LineDetectThread;
	FATMING_CORE::cBaseImage*		m_pVideoImage;
	FATMING_CORE::cBaseImage*		m_pGrayImage;
	class cv::VideoCapture*			m_pVideoCapture;
	void							CameraReadThread(float e_fElpaseTime);
	void							RecognizerThread(float e_fElpaseTime);
public:
	cOpenCVTest_LineDetection();
	virtual ~cOpenCVTest_LineDetection();
	virtual void	OpenCamera(const char* e_strCameraURL = nullptr)override;
	virtual void	Update(float e_fElpaseTime)override;
	virtual void	Render()override;
};

//cv::Mat input = cv::imread("C:/StackOverflow/Input/parking.png");
//cv::Mat gray;
//cv::cvtColor(input, gray, CV_BGR2GRAY);
//cv::Ptr<cv::LineSegmentDetector> det;
//det = cv::createLineSegmentDetector();
//
//
//
//cv::Mat lines;
//det->detect(gray, lines);
//
//det->drawSegments(input, lines);


//MatOfFloat4 lines = new MatOfFloat4();
//line.detect(grey, lines);
//
//if (lines.rows() > 0 && lines.cols() > 0) {
//	float[] f = lines.toArray();
//	int cnt = f.length / 4;
//
//	for (int i = 0; i < cnt; i++) {
//		int x1 = round(f[i * 4]);
//		int y1 = round(f[i * 4 + 1]);
//		int x2 = round(f[i * 4 + 2]);
//		int y2 = round(f[i * 4 + 3]);
//		int mx = constrain((x1 + x2) / 2, 0, img.width - 1);
//		int my = constrain((y1 + y2) / 2, 0, img.height - 1);
//		color col = img.pixels[my * img.width + mx];
//		stroke(col);
//		strokeWeight(random(5));
//		line(x1, y1, x2, y2);
//	}
//}