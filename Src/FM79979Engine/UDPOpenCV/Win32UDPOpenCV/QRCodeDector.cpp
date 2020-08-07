#ifdef WIN32
#include "pch.h"
#else
#include "stdafx.h"
#endif
#include "QRCodeDector.h"
#include "opencv2/opencv.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

sQRCodeDetectorThread::sQRCodeDetectorThread()
{
	m_pQRCodeDecoder = new cv::QRCodeDetector();
	m_bInProcessQRCode = false;
	m_pWaitForProcessDecodeMat = nullptr;
	auto l_Fun = std::bind(&sQRCodeDetectorThread::ProcessQRCodeImage, this, std::placeholders::_1);
	this->ThreadDetach(l_Fun);
}

sQRCodeDetectorThread::~sQRCodeDetectorThread()
{
	SAFE_DELETE(m_pWaitForProcessDecodeMat);
}

void sQRCodeDetectorThread::ProcessQRCodeImage(float e_fElpaseTime)
{
	if (!m_bInProcessQRCode && m_pQRCodeDecoder && m_pWaitForProcessDecodeMat)
	{
		m_FPS.Update();
		m_bInProcessQRCode = true;
		bool l_bMultiQRCodeDetect = true;
		bool l_bGrayTest = false;
		std::string l_strResult;
		std::vector<cv::String> l_strResultVector;
		vector<cv::Point> l_CornersVector;
		cv::Mat bbox, rectifiedImage;
		try
		{
			if (l_bGrayTest)
			{
				cv::Mat imGray;
				cvtColor(*m_pWaitForProcessDecodeMat, imGray, cv::COLOR_BGR2GRAY);
				//l_strResult = m_pQRCodeDecoder->detectAndDecode(imGray, bbox, rectifiedImage);
				//l_strResult = m_pQRCodeDecoder->detectAndDecode(imGray, l_CornersVector);
				if (l_bMultiQRCodeDetect)
					m_pQRCodeDecoder->detectAndDecodeMulti(imGray, l_strResultVector, l_CornersVector);
				else
					l_strResult = m_pQRCodeDecoder->detectAndDecode(imGray, l_CornersVector);
			}
			else
			{
				//l_strResult = m_pQRCodeDecoder->detectAndDecode(*m_pWaitForProcessDecodeMat, bbox, rectifiedImage);

				if (l_bMultiQRCodeDetect)
					m_pQRCodeDecoder->detectAndDecodeMulti(*m_pWaitForProcessDecodeMat, l_strResultVector, l_CornersVector);
				else
					l_strResult = m_pQRCodeDecoder->detectAndDecode(*m_pWaitForProcessDecodeMat, l_CornersVector);
			}
			if (l_strResult.length())
			{
				FMLog::Log("QRCode readed!:", false);
				FMLog::Log(l_strResult.c_str(), false);
			}
			if (l_CornersVector.size())
			{
				for (size_t i = 0; i < l_CornersVector.size(); i += 4)
				{
					size_t qr_idx = i / 4;
					if (l_strResultVector.size() > qr_idx)
					{
						if (!l_strResultVector[qr_idx].empty())
							FMLog::Log(l_strResultVector[qr_idx].c_str(), false);
						//else
							//FMLog::Log("can't decode QR code", false);
					}
					else
					{

					}
				}
			}
		}
		catch (std::exception e)
		{
			FMLog::Log(e.what(), false);
		}
		SAFE_DELETE(m_pWaitForProcessDecodeMat);
		m_FPS.Update();
		//FMLog::Log(UT::ComposeMsgByFormat("QRElpaseTime:%.3f", m_FPS.fElpaseTime).c_str(), false);

		m_bInProcessQRCode = false;
	}
	else
	{
		Sleep(10);
	}
}
