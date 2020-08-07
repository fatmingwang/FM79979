#pragma once

#define		UDP_PORT	22667

#define		MESSAGE_ID_ASSIGN(TYPE,ID)TYPE():sBaseUDPNetworkMessage(){memset(this,0,sizeof(TYPE));iMessage = ID;iSize = (int)sizeof(TYPE);}};
#define		LAZY_MESSAGE_HEADER_STAR(ID)						struct sNetwork_##ID:public sBaseUDPNetworkMessage{
#define		LAZY_MESSAGE_HEADER_END(ID)							MESSAGE_ID_ASSIGN(sNetwork_##ID,ID)

struct sBaseUDPNetworkMessage
{
	int		iSize;
	int		iMessage;
	//sBaseUDPNetworkMessage(int e_iID,int e_iSize) :iMessage(e_iID) { iSize= e_iSize;	}
	sBaseUDPNetworkMessage() { iSize = 0; iMessage = 0; }
};

enum eUDP_MessageList
{
	eUDPML_C2S_REQUIRE_IMAGE = 200000,
	eUDPML_S2C_IMAGE_PACKET_START,
	eUDPML_S2C_IMAGE_PACKET,
	eUDPML_MAX
};

enum eCompressMethod
{
	eCM_JPG = 0,
	eCM_PNG,
	eCM_MAX,
};

//tcpip 1 packet default maximum size is 65k

LAZY_MESSAGE_HEADER_STAR(eUDPML_C2S_REQUIRE_IMAGE)
LAZY_MESSAGE_HEADER_END(eUDPML_C2S_REQUIRE_IMAGE)


LAZY_MESSAGE_HEADER_STAR(eUDPML_S2C_IMAGE_PACKET_START)
	int		iCompressMethod;
	int32	i32ImageSize;
	int32	i32PacketCount;
	int32	i32EachPacketDataSize;
	int		iImageWidth;
	int		iImageHeight;
	int		iNumChannel;
LAZY_MESSAGE_HEADER_END(eUDPML_S2C_IMAGE_PACKET_START)


#define PIXELS_PACK_SIZE 4096 //udp pack size; note that OSX limits < 8100 bytes

LAZY_MESSAGE_HEADER_STAR(eUDPML_S2C_IMAGE_PACKET)
	int		iPacketIndex;
	char	PixelData[PIXELS_PACK_SIZE];
	int		iPixelDataLen;
LAZY_MESSAGE_HEADER_END(eUDPML_S2C_IMAGE_PACKET)
