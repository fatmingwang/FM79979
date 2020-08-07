#pragma once

//#ifdef USE_OPEN_CV

//-lopencv_core
//-lopencv_imgproc
//-lopencv_videoio
//-lopencv_imgcodecs
//-lopencv_objdetect

#define	THREAD_FRAME_COUNT	1

//eUDPML_C2S_REQUIRE_IMAGE = 200000,
//eUDPML_S2C_IMAGE_PACKET_START,
//eUDPML_S2C_IMAGE_PACKET,

namespace cv
{
	class Mat;
	class VideoCapture;
	class QRCodeDetector;
}
//https://stackoverflow.com/questions/46001399/installing-opencv-in-tinker-board
class cUDPServerCamera :public  cMessageSender
{
	UT::sTimeCounter	m_CameraFocusTC;
	bool	Process_eUDPML_C2S_REQUIRE_IMAGE(sNetworkReceivedPacket*e_pData);
	bool	SendStreamingDataToClient(sNetworkReceivedPacket * e_pData,vector < unsigned char >&e_EncodedVector,int e_iImageWidth,int e_iImageHeight);
	//
	vector < int >				m_CompressionParamsVector;
	//
	class cv::VideoCapture*		m_pVideoCapture;
	struct sMatWithFlag
	{
		class cv::Mat*				pFrame;
		bool						bWaitNewData;
		vector < unsigned char >	EncodedVector;
		sMatWithFlag();
		~sMatWithFlag();
	};
	sMatWithFlag*	m_pFrame[THREAD_FRAME_COUNT];
	//
	bool			m_bSingleThread;
	cCPP11Thread	m_CPP11CameraReadThread;
	cCPP11Thread	m_CPP11CompressDataThread;
	cCPP11Thread	m_CPP11UDPSendingDataThread;
	std::mutex		m_CameraReadMutex;
	std::mutex		m_UDPSendingDataMutex;
	//
	struct sQRCodeDetectorThread*	m_pQRCodeDetectorThread;
	//
	void	CameraReadThread(float e_fElpaseTime);
	void	CompressDataThread(float e_fElpaseTime);
	void	UDPSendingDataThread(float e_fElpaseTime);
	//
	void	SingleThreadUpdate(float e_fElpaseTime);
public:
	cUDPServerCamera(bool e_bUseSingleThread = true);
	~cUDPServerCamera();
};
//#endif