#pragma once

#include <mutex>
#ifndef WIN32
	#include "AllCommonInclude.h"
#endif
namespace cv
{
	class VideoCapture;
	class Mat;
	class QRCodeDetector;
}

struct sQRCodeDetectorThread :public cCPP11Thread
{
	UT::sTimeAndFPS		m_FPS;
	std::mutex			m_Mutex;
	cv::QRCodeDetector* m_pQRCodeDecoder;
	bool				m_bInProcessQRCode;
	cv::Mat* m_pWaitForProcessDecodeMat;
	void				ProcessQRCodeImage(float e_fElpaseTime);
	sQRCodeDetectorThread();
	~sQRCodeDetectorThread();
};