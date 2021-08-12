#include "pch.h"
#include "QRCodeDector.h"
//#ifdef USE_OPEN_CV
#include "UDPClient.h"
#include "../../Core/GameplayUT/Render/Texture/jpeg/jpge.h"
#include "../../Core/GameplayUT/Render/Texture/jpeg/jpgd.h"

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "UDPStreamingProtocol.h"
#define JPG_QUALITY	70
bool	g_bCameraUDPTest = true;
cUDPStreamingScreenCaptureServer* g_pScreenCaptureWithCompress = nullptr;
cUDPStreamingDataToTexture* g_pStreamingPixelToTexture = nullptr;

TYPDE_DEFINE_MARCO(cCameraUDPClientNetwork)
cCameraUDPClientNetwork::cCameraUDPClientNetwork()
{
}

cCameraUDPClientNetwork::~cCameraUDPClientNetwork()
{
}

void cCameraUDPClientNetwork::Update(float e_fElpaseTIme)
{
	bool l_bRequiredImage = false;
	auto l_pReceivedDataVector = GetReceivedDataPleaseDeleteAfterUseIt();
	for (auto l_pData : l_pReceivedDataVector)
	{
		sBaseUDPNetworkMessage*l_pBaseNetworkMessage = (sBaseUDPNetworkMessage*)l_pData->pData;
		if (l_pBaseNetworkMessage->iMessage == eUDPML_C2S_REQUIRE_IMAGE)
		{
			if (!l_bRequiredImage)
				NetworkMessageShot(l_pBaseNetworkMessage->iMessage, l_pData);
			l_bRequiredImage = true;
		}
		else
			NetworkMessageShot(l_pBaseNetworkMessage->iMessage, l_pData);
	}
	DELETE_VECTOR(l_pReceivedDataVector);
}

cUDPStreamingDataToTexture::sPixelDataFromPacket::sPixelDataFromPacket
(sNetwork_eUDPML_S2C_IMAGE_PACKET_START*e_pData)
{
	iReceivedDataSize = 0;
	iCompressMethod = e_pData->iCompressMethod;
	iImageWidth = e_pData->iImageWidth;
	iImageHeight = e_pData->iImageHeight;
	iNumPacket = e_pData->i32PacketCount;
	iPacketSize = e_pData->i32EachPacketDataSize;
	iPixelDataSize = e_pData->i32ImageSize;
	pPixelData = new char[iPixelDataSize];
	m_pPacketFilledArray = new bool[iNumPacket];
	memset(m_pPacketFilledArray,0,sizeof(bool)*iNumPacket);
}

cUDPStreamingDataToTexture::sPixelDataFromPacket::~sPixelDataFromPacket()
{
	SAFE_DELETE(pPixelData);
	SAFE_DELETE(m_pPacketFilledArray);
}

int	cUDPStreamingDataToTexture::sPixelDataFromPacket::NumDataNotFilled()
{
	int l_iNum = 0;
	for (int i = 0; i < iNumPacket; ++i)
	{
		if (!m_pPacketFilledArray[i])
		{
			++l_iNum;
		}
	}
	return l_iNum;
}

void cUDPStreamingDataToTexture::PacketDataFinishToGenerateTexture()
{
	if (m_pPixelDataFromPacket)
	{
		if (m_pPixelDataFromPacket->iReceivedDataSize >= m_pPixelDataFromPacket->iPixelDataSize)
		{
			if (m_pPixelDataFromPacket->pPixelData && m_pVideoImage)
			{
				int l_iWidth = 0;
				int l_iHeight = 0;
				int l_iQuility = 0;
				bool l_bDoOpenCV = true;
				unsigned char*l_pNewPixelData = nullptr;
				cv::Mat l_Decode;
				if (l_bDoOpenCV)
				{
					if (m_ForOpenCVVectorData.size() < m_pPixelDataFromPacket->iPixelDataSize)
					{
						m_ForOpenCVVectorData.resize(m_pPixelDataFromPacket->iPixelDataSize);
					}
					memcpy(&m_ForOpenCVVectorData[0], m_pPixelDataFromPacket->pPixelData, m_pPixelDataFromPacket->iPixelDataSize);
					l_Decode = cv::imdecode(m_ForOpenCVVectorData, cv::IMREAD_COLOR);
					l_pNewPixelData = l_Decode.data;					
				}
				else
				{
					l_pNewPixelData = jpgd::decompress_jpeg_image_from_memory((const unsigned char*)m_pPixelDataFromPacket->pPixelData,
						m_pPixelDataFromPacket->iPixelDataSize,&l_iWidth,&l_iHeight,&l_iQuility, 3);
					std::vector<char> l_VectorData(m_pPixelDataFromPacket->pPixelData, m_pPixelDataFromPacket->pPixelData + m_pPixelDataFromPacket->iPixelDataSize);
					l_Decode = cv::imdecode(l_VectorData, cv::IMREAD_COLOR);

				}
				if (m_pQRCodeDetectorThread && !this->m_pQRCodeDetectorThread->m_bInProcessQRCode)
				{
					cv::Mat*l_pDecode = new cv::Mat;
					l_Decode.copyTo(*l_pDecode);
					m_pQRCodeDetectorThread->m_pWaitForProcessDecodeMat = l_pDecode;
				}
				auto l_iPixelFormat = GL_RGB;
				bool l_bRunInUWP = true;
#if defined(WIN32)
				if(l_bDoOpenCV && !l_bRunInUWP)
					l_iPixelFormat = GL_BGR;
#endif
				m_pVideoImage->SetupTexture(3, m_pPixelDataFromPacket->iImageWidth, m_pPixelDataFromPacket->iImageHeight, l_iPixelFormat, GL_UNSIGNED_BYTE, false, l_pNewPixelData,false);
				float l_fUV[] = {0,1,1,0};
				m_pVideoImage->SetUV(l_fUV);
				m_pVideoImage->SetWidth(1600);
				m_pVideoImage->SetHeight(900);
				//m_pVideoImage->SetColor(Vector4(1.5, 1.5, 1.3, 1));
				if (!l_bDoOpenCV)
					SAFE_DELETE(l_pNewPixelData);
			}
		}
	}
}

cUDPStreamingDataToTexture::cUDPStreamingDataToTexture(cMessageSenderManager*e_pParent):cMessageSender(e_pParent)
{
	m_pQRCodeDetectorThread = new sQRCodeDetectorThread();
	m_pPixelDataFromPacket = nullptr;
	m_pVideoImage = nullptr;
	m_pVideoImage = new FATMING_CORE::cBaseImage((const char*)"");
	m_pVideoImage->SetName(L"VideoImage");
	//60 fps
	m_RequireImageTC.SetTargetTime(1/60.f);
	//m_RequireImageTC.SetTargetTime(0.1f);
	REG_NET_MESSAGE_FUNCTION(eUDPML_S2C_IMAGE_PACKET, &cUDPStreamingDataToTexture::Process_eUDPML_S2C_IMAGE_PACKET);
	REG_NET_MESSAGE_FUNCTION(eUDPML_S2C_IMAGE_PACKET_START, &cUDPStreamingDataToTexture::Process_eUDPML_S2C_IMAGE_PACKET_START);
}

cUDPStreamingDataToTexture::~cUDPStreamingDataToTexture()
{
	SAFE_DELETE(m_pQRCodeDetectorThread);
	SAFE_DELETE(m_pVideoImage);
	SAFE_DELETE(m_pPixelDataFromPacket);
}

bool cUDPStreamingDataToTexture::Process_eUDPML_S2C_IMAGE_PACKET_START(sNetworkReceivedPacket * e_pData)
{
	sNetwork_eUDPML_S2C_IMAGE_PACKET_START*l_pData = (sNetwork_eUDPML_S2C_IMAGE_PACKET_START*)e_pData->pData;
	AllocMemory(l_pData);
	return true;
}

bool cUDPStreamingDataToTexture::Process_eUDPML_S2C_IMAGE_PACKET(sNetworkReceivedPacket * e_pData)
{
	sNetwork_eUDPML_S2C_IMAGE_PACKET*l_pData = (sNetwork_eUDPML_S2C_IMAGE_PACKET*)e_pData->pData;
	AddData(l_pData->iPacketIndex, l_pData->PixelData, l_pData->iPixelDataLen);
	return true;
}

bool	cUDPStreamingDataToTexture::AllocMemory(sNetwork_eUDPML_S2C_IMAGE_PACKET_START*e_pData)
{
	SAFE_DELETE(m_pPixelDataFromPacket);
	m_pPixelDataFromPacket = new sPixelDataFromPacket(e_pData);
	m_UDPFPS.Update();
	return true;
}

bool	cUDPStreamingDataToTexture::AddData(int e_iPacketIndex, char * e_pPixelData, int e_iDataLen)
{
	m_RequireImageTC.Start();
	if (m_pPixelDataFromPacket)
	{
		if (e_iPacketIndex > m_pPixelDataFromPacket->iNumPacket)
		{
			FMLog::Log("PacketIndex out of range", false);
			return false;
		}
		else
		{
			if (!m_pPixelDataFromPacket->m_pPacketFilledArray[e_iPacketIndex])
			{
				m_pPixelDataFromPacket->m_pPacketFilledArray[e_iPacketIndex] = true;
				int l_iTargetIndex = m_pPixelDataFromPacket->iPacketSize*e_iPacketIndex;
				int l_iEndPos = l_iTargetIndex + e_iDataLen;
				if (m_pPixelDataFromPacket->iPixelDataSize >= l_iEndPos)
				{
					m_pPixelDataFromPacket->iReceivedDataSize += e_iDataLen;
					memcpy(&m_pPixelDataFromPacket->pPixelData[l_iTargetIndex], e_pPixelData, e_iDataLen);
					if (m_pPixelDataFromPacket->iNumPacket == e_iPacketIndex + 1)
					{
						if(m_pPixelDataFromPacket->NumDataNotFilled() == 0)
							PacketDataFinishToGenerateTexture();
					}
				}
				else
				{
					FMLog::Log("l_iEndPos out of position",false);
				}
			}
			else
			{
				FMLog::Log("packed filled", false);
			}
		}
	}
	else
	{
		FMLog::Log("m_pPixelDataFromPacket not alloc", false);
	}
	return false;
}

void cUDPStreamingDataToTexture::Render(int e_iPosX, int e_iPosY)
{
	if (m_pVideoImage)
	{
		m_pVideoImage->Render(e_iPosX,e_iPosY);
		cGameApp::RenderFont(20, 20, ValueToStringW(m_UDPFPS.GetFPS()));
	}
}

void cUDPStreamingDataToTexture::Update(float e_fElpaseTime)
{
	m_RequireImageTC.Update(e_fElpaseTime);
	if (m_RequireImageTC.bTragetTimrReached)
	{
		m_RequireImageTC.Start();
		auto l_pUDP = cCameraUDPClientNetwork::GetInstance();
		auto l_Statis = l_pUDP->GetNetWorkStatus();
		if (l_Statis == eNetWorkStatus::eNWS_CONNECTED)
		{
			sNetwork_eUDPML_C2S_REQUIRE_IMAGE l_Data;
			cCameraUDPClientNetwork::GetInstance()->SendDataToServer<sNetwork_eUDPML_C2S_REQUIRE_IMAGE>(&l_Data);
		}
	}
}

cUDPStreamingScreenCaptureServer::cUDPStreamingScreenCaptureServer(cMessageSenderManager*e_pParent):cMessageSender(e_pParent)
, cScreenCapture(1280,720)
{
	m_pTempData = nullptr;
	m_iMaximumTampFileLength = 0;
	REG_NET_MESSAGE_FUNCTION(eUDPML_C2S_REQUIRE_IMAGE,&cUDPStreamingScreenCaptureServer::Process_eUDPML_C2S_REQUIRE_IMAGE);
}

cUDPStreamingScreenCaptureServer::~cUDPStreamingScreenCaptureServer()
{
	SAFE_DELETE_ARRAY(m_pTempData);
}

bool cUDPStreamingScreenCaptureServer::Process_eUDPML_C2S_REQUIRE_IMAGE(sNetworkReceivedPacket * e_pData)
{
	if (m_pPixelBuffer && m_pFrameBuffer)
	{
		auto l_iWidth = this->m_pFrameBuffer->GetWidth();
		auto l_iHeight = this->m_pFrameBuffer->GetHeight();
		int l_iChannel = 4;//RGBA
		int	l_iLength = l_iWidth* l_iHeight*l_iChannel;
		if (m_iMaximumTampFileLength < l_iLength)
		{
			SAFE_DELETE(m_pTempData);
			m_iMaximumTampFileLength = l_iLength;
			m_pTempData = new unsigned char[l_iLength];
		}
		sTimeAndFPS l_CpompressFPS;
		auto l_pBuffer = m_pTempData;
		jpge::params	l_Par;
		l_Par.m_quality = JPG_QUALITY;
		l_CpompressFPS.Update();
		if (jpge::compress_image_to_jpeg_file_in_memory(l_pBuffer, l_iLength, l_iWidth, l_iHeight, l_iChannel, (const jpge::uint8*)m_pPixelBuffer, l_Par))
		{
			l_CpompressFPS.Update();
			l_CpompressFPS.fElpaseTime;
			sTimeAndFPS l_FPS;
			int l_iNumPacket = l_iLength/PIXELS_PACK_SIZE;
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
			l_FPS.Update();
			cCameraUDPClientNetwork::GetInstance()->SendDataToClient<sNetwork_eUDPML_S2C_IMAGE_PACKET_START>(*e_pData->pReceivedSocket->Socket.pUDPIPaddress, &l_FirstPacket);
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
				memcpy(l_SequencyPacket.PixelData, &m_pTempData[l_iDataIndex], l_iCurrentPacketSendingDataLen);
				l_SequencyPacket.iPacketIndex = i;
				l_iDataIndex += PIXELS_PACK_SIZE;
				cCameraUDPClientNetwork::GetInstance()->SendDataToClient<sNetwork_eUDPML_S2C_IMAGE_PACKET>(*e_pData->pReceivedSocket->Socket.pUDPIPaddress, &l_SequencyPacket);
			}
			l_FPS.Update();
			//measure elpse time.
			
			float l_fTime = l_FPS.fElpaseTime;
			int a = 0;
		}
	}
	return false;
}

void	UDPTestServerInit()
{
	if (g_bCameraUDPTest)
	{
		auto l_pCameraUDPClientNetwork = cCameraUDPClientNetwork::GetInstance();
		g_pScreenCaptureWithCompress = new cUDPStreamingScreenCaptureServer(l_pCameraUDPClientNetwork);
		l_pCameraUDPClientNetwork->CreateAsServer(UDP_PORT);
	}
}
void	UDPTestServerUpdate()
{
	if(g_bCameraUDPTest)
		cCameraUDPClientNetwork::GetInstance()->Update(0.016f);
	if (g_pScreenCaptureWithCompress)
		g_pScreenCaptureWithCompress->Update(0.016f);
}
void	UDPTestServerCaptureStart()
{
	if (g_pScreenCaptureWithCompress)
		g_pScreenCaptureWithCompress->StartDraw();
}
void	UDPTestServerCaptureEnd()
{
	if (g_pScreenCaptureWithCompress)
		g_pScreenCaptureWithCompress->EndDraw();
}
void	UDPTestServerRender()
{
	if (g_pScreenCaptureWithCompress)
	{		
		g_pScreenCaptureWithCompress->Render(Vector3(0, 0, 0),
			(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(),
			(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
	}
}
void	UDPTestServerDestroy()
{
	SAFE_DELETE(g_pScreenCaptureWithCompress);
	cCameraUDPClientNetwork::DestroyInstance();
}

void	UDPTestClientInit()
{
	if (g_bCameraUDPTest)
	{
		auto l_pCameraUDPClientNetwork = cCameraUDPClientNetwork::GetInstance();
		g_pStreamingPixelToTexture = new cUDPStreamingDataToTexture(l_pCameraUDPClientNetwork);
		l_pCameraUDPClientNetwork->CreateAsClient("192.168.1.163", UDP_PORT);
	}
}
void	UDPTestClientUpdate()
{
	if (g_bCameraUDPTest)
		cCameraUDPClientNetwork::GetInstance()->Update(0.016f);
	if (g_pStreamingPixelToTexture)
		g_pStreamingPixelToTexture->Update(0.016f);
}
void	UDPTestClientRender()
{
	if (g_pStreamingPixelToTexture)
		g_pStreamingPixelToTexture->Render(100,100);
}
void	UDPTestClientDestroy()
{
	SAFE_DELETE(g_pStreamingPixelToTexture);
	cCameraUDPClientNetwork::DestroyInstance();
}
//#endif