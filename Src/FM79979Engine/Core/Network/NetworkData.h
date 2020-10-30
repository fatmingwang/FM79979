#pragma once

namespace FATMING_CORE
{
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
		sNetworkSendPacket(sNetworkSendPacket*e_pNetworkSendPacket);
		~sNetworkSendPacket();
	};
	struct sNetworkReceivedPacket
	{
		int			iSize;
		char*		pData;
		_TCPsocket*	pReceivedSocket;
		IPaddress   UDPIPaddress;
		sNetworkReceivedPacket();
		~sNetworkReceivedPacket();
		//return rest data wait for receiving,-1 or less or equal than 0 connection has problem(lost connection).
		int	ReceiveData(_TCPsocket*e_pTCPsocket);
		//int	ReceiveData(_UDPsocket*e_pSocket, UDPpacket *e_pPackets);
		int	UDPReceiveDataWithoutHeaderSize(IPaddress e_UDPIPaddress,int e_iDataLen,char*e_pData);
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