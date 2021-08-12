#pragma once
//opencv_world440d.lib
//

namespace cv
{
	class Mat;
	class QRCodeDetector;
}
struct sNetwork_eUDPML_S2C_IMAGE_PACKET_START;
//is here need a thread for quick send and receive data?
class cCameraUDPClientNetwork:public cUDPNetwork,public cSingltonTemplate<cCameraUDPClientNetwork>
	,public cMessageSenderManager
{
	cCameraUDPClientNetwork();
	virtual ~cCameraUDPClientNetwork();
public:
	DEFINE_TYPE_INFO();
	SINGLETON_BASIC_FUNCTION(cCameraUDPClientNetwork);
	virtual void	Update(float e_fElpaseTIme)override;
};

//process  streaming data to texture.
class cUDPStreamingDataToTexture: public cMessageSender,public NamedTypedObject
{
	UT::sTimeAndFPS	m_UDPFPS;
	bool	Process_eUDPML_S2C_IMAGE_PACKET_START(sNetworkReceivedPacket*e_pData);
	bool	Process_eUDPML_S2C_IMAGE_PACKET(sNetworkReceivedPacket*e_pData);
	//
	bool	AllocMemory(sNetwork_eUDPML_S2C_IMAGE_PACKET_START*e_pData);
	bool	AddData(int e_iPacketIndex, char*e_pPixelData, int e_iDataLen);
	//
	FATMING_CORE::cBaseImage*	m_pVideoImage;
	struct sPixelDataFromPacket
	{
		int		iCompressMethod;//default is jpg
		int		iReceivedDataSize;
		int		iNumPacket;
		char*	pPixelData;
		int		iPacketSize;
		int		iPixelDataSize;
		int		iImageWidth;
		int		iImageHeight;
		int		iNumChanne;;
		bool*	m_pPacketFilledArray;
		sPixelDataFromPacket(sNetwork_eUDPML_S2C_IMAGE_PACKET_START*e_pData);
		~sPixelDataFromPacket();
		int		NumDataNotFilled();
	};
	sPixelDataFromPacket*m_pPixelDataFromPacket;
	void	PacketDataFinishToGenerateTexture();
	//
	UT::sTimeCounter	m_RequireImageTC;
	//
	struct sQRCodeDetectorThread*	m_pQRCodeDetectorThread;
	std::vector<char>	m_ForOpenCVVectorData;
public:
	//please use cCameraUDPClientNetwork
	cUDPStreamingDataToTexture(cMessageSenderManager*e_pParent);
	virtual ~cUDPStreamingDataToTexture();
	void	Render(int e_iPosX,int e_iPosY);
	void	Update(float e_fElpaseTime);
};
//for test
//capture app screen and send to client
class cUDPStreamingScreenCaptureServer : public cMessageSender, public cScreenCapture
{
	unsigned char*m_pTempData;
	int		m_iMaximumTampFileLength;
	bool	Process_eUDPML_C2S_REQUIRE_IMAGE(sNetworkReceivedPacket*e_pData);
public:
	//please use cCameraUDPClientNetwork
	cUDPStreamingScreenCaptureServer(cMessageSenderManager*e_pParent);
	virtual ~cUDPStreamingScreenCaptureServer();
};

void	UDPTestServerInit();
void	UDPTestServerUpdate();
void	UDPTestServerCaptureStart();
void	UDPTestServerCaptureEnd();
void	UDPTestServerRender();
void	UDPTestServerDestroy();


void	UDPTestClientInit();
void	UDPTestClientUpdate();
void	UDPTestClientRender();
void	UDPTestClientDestroy();