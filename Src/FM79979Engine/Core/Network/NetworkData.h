#pragma once
#include "SDL_net.h"
namespace FATMING_CORE
{
	struct sSDLNetTCPSocket
	{
		enum eNetworkType
		{
			eNT_TCP = 0,
			eNT_UDP,
			eNT_WEBSOCKET,
			eNT_MAX
		};
		eNetworkType	NetworkType = eNT_MAX;
		union WhichSocket
		{
			_TCPsocket*		pTCPIPSocket;
			IPaddress*		pUDPIPaddress;
			size_t			uiWebSocketHandlerAddress;//websocketpp address.
		};
		WhichSocket Socket;
		sSDLNetTCPSocket(_TCPsocket* e_pSocket);
		sSDLNetTCPSocket(IPaddress e_IPaddress);
		sSDLNetTCPSocket(size_t e_uiWebSocketAddress);
		~sSDLNetTCPSocket();
	};

	typedef  std::shared_ptr<sSDLNetTCPSocket>	SDLNetSocket;
	SDLNetSocket	GetSDLNetSocket(_TCPsocket*);
	SDLNetSocket	GetSDLNetSocket(IPaddress);
	SDLNetSocket	GetSDLNetSocket(size_t);


	//struct sConnectionTypeAndSocket
	//{
	//	enum eNetworkType
	//	{
	//		eNT_TCP = 0,
	//		eNT_UDP,
	//		eNT_WEBSOCKET,
	//		eNT_MAX
	//	};
	//	eNetworkType	NetworkType;
	//	SDLNetSocket	pReceivedSocket;
	//	IPaddress		UDPIPaddress;
	//	char*			pWebSocketHandler;//websocketpp.
	//};
	//sNetworkSendPacket and sNetworkReceivedPacket has a header for description packet size
#define	PACKET_HEADER_SIZE (int)sizeof(int)
	enum eNetWorkStatus
	{
		eNWS_NONE = 0,
		eNWS_TRY_TO_CONNECT,
		eNWS_CONNECTED,
		eNWS_CONNECTION_NOT_EXISTS, 
		eNWS_LOST_CONNECTION,
		eNWS_NO_INTERNET,
	};
	struct	sNetworkSendPacket
	{
		int		iSize;
		char*	pData;//first int must be unsigned int for messageID.
		sNetworkSendPacket();
		sNetworkSendPacket(char* e_pData, int e_iDataLength);
		sNetworkSendPacket(sNetworkSendPacket*e_pNetworkSendPacket);
		~sNetworkSendPacket();
	};
	struct sNetworkReceivedPacket
	{
		int				iSize;
		char*			pData;
		SDLNetSocket	pReceivedSocket;
		sNetworkReceivedPacket();
		~sNetworkReceivedPacket();
		//return rest data wait for receiving,-1 or less or equal than 0 connection has problem(lost connection).
		int	ReceiveTCPData(SDLNetSocket e_pTCPsocket,bool e_bUseExtractHeader);
		//int	ReceiveData(_UDPsocket*e_pSocket, UDPpacket *e_pPackets);
		int	UDPReceiveDataWithoutHeaderSize(SDLNetSocket e_pTCPsocket,int e_iDataLen,char*e_pData);
		int	WebSocketReceiveDataWithoutHeaderSize(SDLNetSocket e_pTCPsocket,int e_iDataLen,const char* e_pData);
	};

	struct sIPData
	{
		int									m_iPort;
		IPaddress							m_IP;
		unsigned char						m_MyIP[4];
		std::string							m_strIP;
		std::string							m_strServerIP;
		std::string							m_strHost;
	};
	extern bool	IsNetworkAlive();
}