#ifdef WIN32
#include "pch.h"
#else
#include "stdafx.h"
#endif
#include "QRCodeDector.h"
//#ifdef USE_OPEN_CV
#include "opencv2/opencv.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "UDPServerCamera.h"
#include "UDPStreamingProtocol.h"
#ifdef WIN32
#include "../../Core/Network/UDPNetwork.h"
#else
#include "Network/UDPNetwork.h"
#endif

using namespace cv;
//for opencv-3.4.1
///home/linaro/work/opencv-3.4.1/include
//-lopencv_core
//-lopencv_highgui
//-lopencv_imgproc
//-lopencv_videoio
//-lopencv_imgcodecs
//opencv 4
//https://www.learnopencv.com/install-opencv-4-on-ubuntu-18-04/
//cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=$cwd/installation/OpenCV-"$cvVersion" -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D WITH_TBB=ON -D WITH_V4L=ON -D OPENCV_PYTHON3_INSTALL_PATH=$cwd/OpenCV-$cvVersion-py3/lib/python3.5/site-packages -D WITH_QT=ON -D WITH_OPENGL=ON -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules -D BUILD_EXAMPLES=ON ..
//no opengl
//cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=$cwd/installation/OpenCV-"$cvVersion" -D INSTALL_C_EXAMPLES=ON -D WITH_TBB=ON -D WITH_V4L=ON -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules -D BUILD_EXAMPLES=ON ..
//0 480p,1 720p,2 1080p
int g_iCameraResolution = 1;

int	FRAME_WIDTH = 1280;
int	FRAME_HEIGHT = 720;
int g_iJPGQuility = 30;

cUDPServerCamera::sMatWithFlag::sMatWithFlag()
{
	pFrame = new Mat;
	bWaitNewData = true;
}

cUDPServerCamera::sMatWithFlag::~sMatWithFlag()
{
	SAFE_DELETE(pFrame);
}

cUDPServerCamera::cUDPServerCamera(bool e_bUseSingleThread):cMessageSender(cSimpleUDPClientNetwork::GetInstance())
{
	m_CameraFocusTC.SetTargetTime(5.f);
	if (g_iCameraResolution == 2)
	{
		FRAME_WIDTH = 1920;
		FRAME_HEIGHT = 1080;
	}
	if (g_iCameraResolution == 1)
	{
		FRAME_WIDTH = 1280;
		FRAME_HEIGHT = 720;
	}
	else
	if (g_iCameraResolution == 0)
	{
		FRAME_WIDTH = 640;
		FRAME_HEIGHT = 360;
	}
	m_bSingleThread = e_bUseSingleThread;
	for (int i = 0; i < THREAD_FRAME_COUNT; ++i)
	{
		m_pFrame[i] = new sMatWithFlag;
	}
	if (g_iCameraResolution == 0)
		m_pVideoCapture = new VideoCapture("rkcamsrc io-mode=4 isp-mode=2A tuning-xml-path=/etc/cam_iq/IMX219.xml ! video/x-raw,format=NV12,width=640,height=360 ! videoconvert ! appsink");
	else
	if (g_iCameraResolution == 1)
		m_pVideoCapture = new VideoCapture("rkcamsrc io-mode=4 isp-mode=2A tuning-xml-path=/etc/cam_iq/IMX219.xml ! video/x-raw,format=NV12,width=1280,height=720 ! videoconvert ! appsink");
	if (g_iCameraResolution == 2)
		m_pVideoCapture = new VideoCapture("rkcamsrc io-mode=4 isp-mode=2A tuning-xml-path=/etc/cam_iq/IMX219.xml ! video/x-raw,format=NV12,width=1920,height=1080 ! videoconvert ! appsink");
	if (m_pVideoCapture->isOpened())
	{
		m_pVideoCapture->set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
		m_pVideoCapture->set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
		//https://bbs.21ic.com/blog-62556-683410.html
		m_pVideoCapture->set(cv::CAP_PROP_FOCUS, 150);
		//m_pVideoCapture->set(cv::CAP_PROP_AUTOFOCUS, 1);
		//https://stackoverflow.com/questions/19813276/manually-focus-webcam-from-opencv
	}
	m_CompressionParamsVector.push_back(IMWRITE_JPEG_QUALITY);
	m_CompressionParamsVector.push_back(g_iJPGQuility);
	auto l_pInstance = cSimpleUDPClientNetwork::GetInstance();
	l_pInstance->SetUpdateFunction
	(
		[](cSimpleUDPClientNetwork*e_pSimpleUDPClientNetwork, float e_fElpaseTime)
		{
			bool l_bRequiredImage = false;
			auto l_pReceivedDataVector = e_pSimpleUDPClientNetwork->GetReceivedDataPleaseDeleteAfterUseIt();
			for (auto l_pData : l_pReceivedDataVector)
			{
				sBaseUDPNetworkMessage*l_pBaseNetworkMessage = (sBaseUDPNetworkMessage*)l_pData->pData;
				if (l_pBaseNetworkMessage->iMessage == eUDPML_C2S_REQUIRE_IMAGE)
				{
					if (!l_bRequiredImage)
						e_pSimpleUDPClientNetwork->NetworkMessageShot(l_pBaseNetworkMessage->iMessage, l_pData);
					l_bRequiredImage = true;
				}
				else
				{
					e_pSimpleUDPClientNetwork->NetworkMessageShot(l_pBaseNetworkMessage->iMessage, l_pData);
				}
			}
			DELETE_VECTOR(l_pReceivedDataVector);
		}
	);
	if (m_pVideoCapture->isOpened())
	{
		m_pQRCodeDetectorThread = new sQRCodeDetectorThread();
		if (!e_bUseSingleThread)
		{
			auto l_Fun1 = std::bind(&cUDPServerCamera::CameraReadThread, this, std::placeholders::_1);
			auto l_Fun2 = std::bind(&cUDPServerCamera::UDPSendingDataThread, this, std::placeholders::_1);
			auto l_Fun3 = std::bind(&cUDPServerCamera::CompressDataThread, this, std::placeholders::_1);
			m_CPP11CameraReadThread.ThreadDetach(l_Fun1);
			m_CPP11UDPSendingDataThread.ThreadDetach(l_Fun2);
			m_CPP11CompressDataThread.ThreadDetach(l_Fun3);
		}
		else
		{
			REG_NET_MESSAGE_FUNCTION(eUDPML_C2S_REQUIRE_IMAGE, &cUDPServerCamera::Process_eUDPML_C2S_REQUIRE_IMAGE);
			auto l_Fun3 = std::bind(&cUDPServerCamera::SingleThreadUpdate, this, std::placeholders::_1);
			m_CPP11CameraReadThread.ThreadDetach(l_Fun3);
		}
		l_pInstance->CreateAsServer(UDP_PORT);
	}
}

cUDPServerCamera::~cUDPServerCamera()
{
	m_CPP11CameraReadThread.CloseThreadAndWaitUntilFinish();
	m_CPP11CompressDataThread.CloseThreadAndWaitUntilFinish();
	m_CPP11UDPSendingDataThread.CloseThreadAndWaitUntilFinish();
	SAFE_DELETE(m_pVideoCapture);
	for (int i = 0; i < THREAD_FRAME_COUNT; ++i)
	{
		SAFE_DELETE(m_pFrame[i]);
	}
	cSimpleUDPClientNetwork::DestroyInstance();
}

void cUDPServerCamera::CameraReadThread(float e_fElpaseTime)
{
	//30 fps
	Sleep(1000 / 30);
	if (!m_pVideoCapture || !m_pFrame)
	{
		Sleep(100);
		return;
	}
	if (!m_pVideoCapture->isOpened())
	{
		Sleep(100);
		return;
	}
	sMatWithFlag*l_pCurrentMatWithFlag = nullptr;
	{
		MUTEX_PLACE_HOLDER(m_CameraReadMutex,"cUDPServerCamera::CameraReadThread");
		for (int i = 0; i < THREAD_FRAME_COUNT; ++i)
		{
			if (m_pFrame[i]->bWaitNewData)
			{
				l_pCurrentMatWithFlag = m_pFrame[i];
				break;
			}
		};
	}
	if (!l_pCurrentMatWithFlag)
	{//wait client fetch data
		Sleep(10);
		return;
	}
	*m_pVideoCapture >> *l_pCurrentMatWithFlag->pFrame;
	if (l_pCurrentMatWithFlag->pFrame->size().width != 0)
	{
		l_pCurrentMatWithFlag->bWaitNewData = false;
	}
}

void cUDPServerCamera::CompressDataThread(float e_fElpaseTime)
{
	sMatWithFlag*l_pCurrentMatWithFlag = nullptr;
	{
		MUTEX_PLACE_HOLDER(m_CameraReadMutex,"cUDPServerCamera::CompressDataThread");
		for (int i = 0; i < THREAD_FRAME_COUNT; ++i)
		{
			if (!m_pFrame[i]->bWaitNewData)
			{
				if (m_pFrame[i]->EncodedVector.size() == 0)
				{
					l_pCurrentMatWithFlag = m_pFrame[i];
					break;
				}
			}
		};
	}
	if (!l_pCurrentMatWithFlag)
	{//wait client fetch data
		Sleep(10);
		return;
	}
	imencode(".jpg", *l_pCurrentMatWithFlag->pFrame, l_pCurrentMatWithFlag->EncodedVector, m_CompressionParamsVector);
}

void cUDPServerCamera::UDPSendingDataThread(float e_fElpaseTime)
{
	sMatWithFlag*l_pCurrentMatWithFlag = nullptr;
	{
		MUTEX_PLACE_HOLDER(m_CameraReadMutex,"cUDPServerCamera::UDPSendingDataThread 1");
		for (int i = 0; i < THREAD_FRAME_COUNT; ++i)
		{
			if (!m_pFrame[i]->bWaitNewData)
			{
				if (m_pFrame[i]->EncodedVector.size() != 0)
				{
					l_pCurrentMatWithFlag = m_pFrame[i];
					break;
				}
			}
		};
	}
	if (!l_pCurrentMatWithFlag)
	{
		Sleep(10);
		return;
	}
	auto l_pSimpleUDPClientNetwork = cSimpleUDPClientNetwork::GetInstance();
	auto l_pReceivedDataVector = l_pSimpleUDPClientNetwork->GetReceivedDataPleaseDeleteAfterUseIt();
	bool l_bRequiredImage = false;
	for (auto l_pData : l_pReceivedDataVector)
	{
		sBaseUDPNetworkMessage*l_pBaseNetworkMessage = (sBaseUDPNetworkMessage*)l_pData->pData;
		if (l_pBaseNetworkMessage->iMessage == eUDPML_C2S_REQUIRE_IMAGE)
		{
			if (!l_bRequiredImage)
			{
				SendStreamingDataToClient(l_pData, l_pCurrentMatWithFlag->EncodedVector, l_pCurrentMatWithFlag->pFrame->size().width, l_pCurrentMatWithFlag->pFrame->size().height);
				{
					MUTEX_PLACE_HOLDER(m_CameraReadMutex,"cUDPServerCamera::UDPSendingDataThread 2");
					l_pCurrentMatWithFlag->bWaitNewData = true;
					l_pCurrentMatWithFlag->EncodedVector.clear();
				}
			}
			l_bRequiredImage = true;
		}
		else
		{
			l_pSimpleUDPClientNetwork->NetworkMessageShot(l_pBaseNetworkMessage->iMessage, l_pData);
		}
	}
	if(l_pReceivedDataVector.size() == 0)
		Sleep(10);
	DELETE_VECTOR(l_pReceivedDataVector);
}

bool cUDPServerCamera::Process_eUDPML_C2S_REQUIRE_IMAGE(sNetworkReceivedPacket * e_pData)
{
	sMatWithFlag*l_pCurrentMatWithFlag = m_pFrame[0];
	SendStreamingDataToClient(e_pData,l_pCurrentMatWithFlag->EncodedVector, l_pCurrentMatWithFlag->pFrame->size().width, l_pCurrentMatWithFlag->pFrame->size().height);
	return true;
}

bool cUDPServerCamera::SendStreamingDataToClient(sNetworkReceivedPacket * e_pData,vector<unsigned char>& e_EncodedVector, int e_iImageWidth, int e_iImageHeight)
{
	if (e_EncodedVector.size())
	{
		auto l_iWidth = e_iImageWidth;
		auto l_iHeight = e_iImageHeight;
		int l_iChannel = 3;//RGBA
		int l_iLength = (int)e_EncodedVector.size();
		int l_iNumPacket = (int)(e_EncodedVector.size() / PIXELS_PACK_SIZE);
		if (l_iLength % PIXELS_PACK_SIZE)
			++l_iNumPacket;
		sNetwork_eUDPML_S2C_IMAGE_PACKET_START l_FirstPacket;
		l_FirstPacket.i32EachPacketDataSize = PIXELS_PACK_SIZE;
		l_FirstPacket.i32ImageSize = l_iLength;
		l_FirstPacket.i32PacketCount = l_iNumPacket;
		l_FirstPacket.iCompressMethod = eCM_JPG;
		l_FirstPacket.iImageWidth = l_iWidth;
		l_FirstPacket.iImageHeight = l_iHeight;
		l_FirstPacket.iNumChannel = l_iChannel;
		cSimpleUDPClientNetwork::GetInstance()->SendDataToClient<sNetwork_eUDPML_S2C_IMAGE_PACKET_START>(*e_pData->pReceivedSocket->Socket.pUDPIPaddress, &l_FirstPacket);
		sNetwork_eUDPML_S2C_IMAGE_PACKET l_SequencyPacket;
		int l_iSentDataLen = 0;
		int l_iDataIndex = 0;
		for (int i = 0; i < l_iNumPacket; ++i)
		{
			int l_iCurrentPacketSendingDataLen = PIXELS_PACK_SIZE;
			l_iSentDataLen += PIXELS_PACK_SIZE;
			if (l_iSentDataLen > l_iLength)
			{
				l_iCurrentPacketSendingDataLen = l_iLength - (l_iSentDataLen - PIXELS_PACK_SIZE);
			}
			l_SequencyPacket.iPixelDataLen = l_iCurrentPacketSendingDataLen;
			memcpy(l_SequencyPacket.PixelData, &e_EncodedVector[l_iDataIndex], l_iCurrentPacketSendingDataLen);
			l_SequencyPacket.iPacketIndex = i;
			l_iDataIndex += PIXELS_PACK_SIZE;
			cSimpleUDPClientNetwork::GetInstance()->SendDataToClient<sNetwork_eUDPML_S2C_IMAGE_PACKET>(*e_pData->pReceivedSocket->Socket.pUDPIPaddress, &l_SequencyPacket);
		}
		e_EncodedVector.clear();
	}
	return false;
}

void cUDPServerCamera::SingleThreadUpdate(float e_fElpaseTime)
{
	//fps 30
	Sleep(1000/30);
	//m_CameraFocusTC.Update(e_fElpaseTime);
	//if (m_CameraFocusTC.bTragetTimrReached)
	//{
	//	m_CameraFocusTC.Start();
	//	static int l_iFocus = 5;
	//	l_iFocus += 20;
	//	if (l_iFocus >= 255)
	//		l_iFocus = 5;
	//	m_pVideoCapture->set(cv::CAP_PROP_FOCUS, l_iFocus);
	//}
	if (!m_pVideoCapture || !m_pFrame )
		return;
	//https://stackoverflow.com/questions/29099839/opencv-stream-from-a-camera-connected-to-a-remote-machine
	//Where address is something like 
	//rtsp://127.0.0.1:554 or 
	//rtsp://username:password@127.0.0.1:554 if it is password protected.
	//std::string address = "rtsp://<username:password>@<ip_address>:<port>";
	//cv::VideoCapture cap;
	//if (!cap.open(address))
	//{
	//	std::cout << "Error opening video stream: " << address << std::endl;
	//	return -1;
	//}
	if (!m_pVideoCapture->isOpened()) 
	{
		m_pVideoCapture->open(0);
		return;
	}
	sMatWithFlag*l_pCurrentMatWithFlag = m_pFrame[0];
	if (l_pCurrentMatWithFlag->EncodedVector.size() == 0)
	{
		bool l_bDoScissor = false;
		if (l_bDoScissor)
		{
			Mat l_CameraImage;
			*m_pVideoCapture >> l_CameraImage;
			cv::Rect l_TargetRect;
			if (g_iCameraResolution == 0)
			{
				//640,360
				l_TargetRect = Rect(160, 90, 320, 180);
			}
			else
			if (g_iCameraResolution == 1)
			{
				//1280,720
				l_TargetRect = Rect(320,180,640,360);
			}
			else
			if (g_iCameraResolution == 2)
			{
				//1920,1080
				l_TargetRect = Rect(480, 270, 960, 540);
			}
			l_CameraImage(l_TargetRect).copyTo(*l_pCurrentMatWithFlag->pFrame);
		}
		else
		{
			*m_pVideoCapture >> *l_pCurrentMatWithFlag->pFrame;
		}
		if (l_pCurrentMatWithFlag->pFrame->size().width == 0)
			return;
		bool l_bConvertToBGR = true;
		if (l_bConvertToBGR)
		{
			cv::Mat l_RGB;
			cv::cvtColor(*l_pCurrentMatWithFlag->pFrame, l_RGB, cv::COLOR_RGB2BGR);
			imencode(".jpg", l_RGB, l_pCurrentMatWithFlag->EncodedVector, m_CompressionParamsVector);
		}
		else
		{
			imencode(".jpg", *l_pCurrentMatWithFlag->pFrame, l_pCurrentMatWithFlag->EncodedVector, m_CompressionParamsVector);
		}
		if (m_pQRCodeDetectorThread && !this->m_pQRCodeDetectorThread->m_bInProcessQRCode)
		{
			cv::Mat*l_pDecode = new cv::Mat;
			l_pCurrentMatWithFlag->pFrame->copyTo(*l_pDecode);
			m_pQRCodeDetectorThread->m_pWaitForProcessDecodeMat = l_pDecode;
		}
	}
	auto l_pSimpleUDPClientNetwork = cSimpleUDPClientNetwork::GetInstance();
	auto l_pReceivedDataVector = l_pSimpleUDPClientNetwork->GetReceivedDataPleaseDeleteAfterUseIt();
	bool l_bRequiredImage = false;
	for (auto l_pData : l_pReceivedDataVector)
	{
		sBaseUDPNetworkMessage*l_pBaseNetworkMessage = (sBaseUDPNetworkMessage*)l_pData->pData;
		if (l_pBaseNetworkMessage->iMessage == eUDPML_C2S_REQUIRE_IMAGE)
		{
			if (!l_bRequiredImage && l_pCurrentMatWithFlag)
			{
				SendStreamingDataToClient(l_pData, l_pCurrentMatWithFlag->EncodedVector, l_pCurrentMatWithFlag->pFrame->size().width, l_pCurrentMatWithFlag->pFrame->size().height);
				l_pCurrentMatWithFlag->EncodedVector.clear();
			}
			l_bRequiredImage = true;
		}
		else
		{
			l_pSimpleUDPClientNetwork->NetworkMessageShot(l_pBaseNetworkMessage->iMessage, l_pData);
		}
	}
	if (l_pReceivedDataVector.size() == 0)
		DELETE_VECTOR(l_pReceivedDataVector);
}

//#endif