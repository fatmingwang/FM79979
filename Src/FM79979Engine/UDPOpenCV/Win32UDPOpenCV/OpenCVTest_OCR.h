#pragma once

#include "OpenCVTestBase.h"
//OSD mode orientation and script detection

//tutorial
//https://www.cnblogs.com/zx-admin/p/14204029.html
//tarnnied model
//https://github.com/tesseract-ocr/tessdata_best
//opencv official document
//https://learnopencv.com/deep-learning-based-text-detection-using-opencv-c-python/
//DNN(very slow)
//https://www.twblogs.net/a/5c80e028bd9eee35fc136515
//tesseract,Linux
//https://medium.com/building-a-simple-text-correction-tool/basic-ocr-with-tesseract-and-opencv-34fae6ab3400
//SetPageSegMode tutorial.
//https://tesseract-ocr.github.io/tessdoc/APIExample.html
//step by step tutorial
//https://www.technical-recipes.com/2021/getting-started-with-tesseract-optical-character-recognition-ocr-library-in-visual-studio/
class cOpenCVTest_OCR:public OpenCVTestBase, public cCPP11Thread
{
	cFreetypeGlyphRender* m_pFreetypeGlyphRender;
	struct sPosAndText
	{
		std::vector<Vector2>	PointsVector;
		std::wstring			strText;
		float					fConfidence;
	};
	std::vector < sPosAndText>	m_WordAndPosVector;
	//
	UT::sTimeAndFPS	m_CameraFPS;
	UT::sTimeAndFPS	m_RecognizerWordFPS;
	std::string		m_strCameraURL;
	std::string		m_strRecognizerWord;
	std::mutex		m_RecognizerFrameCopyMutex;
	std::mutex		m_OpenGLFrameMutex;
	std::mutex		m_RecognizerSetWordMutex;
	std::mutex		m_ConvertedBlobFrameMutex;
	sMatWithFlag*	m_pOpenGLFrame;
	sMatWithFlag*	m_pRecognizerFrame;
	sMatWithFlag*	m_pConvertedBlobFrame;
	cCPP11Thread	m_RecognizerThread;
	//sMatWithFlag*	m_pFaceDetectFrame;
	FATMING_CORE::cBaseImage*		m_pVideoImage;
	FATMING_CORE::cBaseImage*		m_pConvertedBlobImage;
	class cv::VideoCapture*			m_pVideoCapture;
	//std::vector<cv::Rect>			m_FacesRect;
	//vector< vector<cv::Point2f> >	m_FacesPointsVector;
	void							CameraReadThread(float e_fElpaseTime);
	void							RecognizerThread(float e_fElpaseTime);
	//
	void							OpencvDNNParseText(cv::Mat& e_Frame);
	void							TesseractParseTextIterator(cv::Mat&e_Frame);
	void							TesseractParseTextImageComponet(cv::Mat& e_Frame);
public:
	cOpenCVTest_OCR();
	virtual ~cOpenCVTest_OCR();
	virtual void	OpenCamera(const char* e_strCameraURL = nullptr)override;
	virtual void	Update(float e_fElpaseTime)override;
	virtual void	Render()override;
};