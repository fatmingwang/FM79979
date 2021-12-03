#ifndef _M_CEE//manage code dont support cCPP11Thread
#include "assert.h"
#include "../Common/Utility.h"
#include "GameNetWork.h"
#include "../Common/Log/FMLog.h"
#include "../Common/StringToStructure.h"
#ifdef LINUX
#include <alloca.h>
#endif
#include "SDL_net.h"

#ifdef WASM
//emsdk\upstream\emscripten\tests\websocket\tcp_echo_client.cpp
	bool	g_bWASMDoSSL = false;
//#define DO_WEBSOCKET_INIT
	#ifdef DO_WEBSOCKET_INIT
	//fuck!!!!!!!!
	// 	if you have set flag for -s PROXY_POSIX_SOCKETS=1 
	//  you hvae to define DO_WEBSOCKET_INIT or thread stuck,because it expected call emscripten websocket function
	//fuck!!!!!!!!
// -lwebsocket.js -s PROXY_POSIX_SOCKETS=1 
#include <emscripten.h>
#include <emscripten/websocket.h>
#include <emscripten/threading.h>
#include <emscripten/posix_socket.h>
		#define	EMSCRIPTEN_WEBSOCKET_INIT(ADDRESS)	emscripten_init_websocket_to_posix_socket_bridge(ADDRESS)
		#define	EMSCRIPTEN_WEBSOCKET_READY(P1,P2)	emscripten_websocket_get_ready_state(P1,P2)
		bool	g_bDoinit_websocket = true;
	#else
		#define	EMSCRIPTEN_WEBSOCKET_INIT(ADDRESS)1;
		#define	EMSCRIPTEN_WEBSOCKET_READY(P1,P2)1;
		bool	g_bDoinit_websocket = false;
	#endif
#endif

int	WASMPOSIXWakeup(int e_iPort, const char* e_strIP)
{
	int l_iBridgeSocket = 1;
#ifdef WASM
	if (g_bDoinit_websocket)
	{
		//-s WEBSOCKET_SUBPROTOCOL=null 
		//// -lwebsocket.js -s PROXY_POSIX_SOCKETS=1 -s PROXY_TO_PTHREAD=1
		//// -s PROXY_POSIX_SOCKETS=1 
		////  -s PROXY_POSIX_SOCKETS=1 -s PROXY_TO_PTHREAD=1 -s LLD_REPORT_UNDEFINED 
		//https://githubmemory.com/repo/emscripten-core/emscripten/issues/14492
		//"wss://localhost:8080"
		bool l_bDoSSL = g_bWASMDoSSL;
		std::string l_strWebSocketInfo;
		if (l_bDoSSL)
		{
			l_strWebSocketInfo = UT::ComposeMsgByFormat("wss://%s:%d", e_strIP, e_iPort);
		}
		else
		{
			l_strWebSocketInfo = UT::ComposeMsgByFormat("ws://%s:%d", e_strIP, e_iPort);
		}
		printf("call emscripten_init_websocket_to_posix_socket_bridge:WebSocket address is ");
		printf(l_strWebSocketInfo.c_str());
		printf("\n");
		l_iBridgeSocket = EMSCRIPTEN_WEBSOCKET_INIT(l_strWebSocketInfo.c_str());
		printf("BridgeSocket:%d\n", l_iBridgeSocket);
		// Synchronously wait until connection has been established.
		//unsigned short readyState = 0;
		//if (1)
		//{
		//	for (int i = 0; i < 1; ++i)
		//	{
		//		emscripten_websocket_get_ready_state(g_iBridgeSocket, &readyState);
		//		if (readyState != 0)
		//		{
		//			printf("emscripten_websocket_get_ready_state ok\n");
		//			break;
		//		}
		//		emscripten_thread_sleep(100);
		//		printf("emscripten_websocket_get_ready_state try again...\n");
		//	}
		//}
		//else
		//{
		//	do 
		//	{
		//		emscripten_websocket_get_ready_state(g_iBridgeSocket, &readyState);
		//		emscripten_thread_sleep(100);
		//	} 
		//	while (readyState == 0);
		//}
	}
#endif
	return l_iBridgeSocket;
}
namespace FATMING_CORE
{
	sSDLNetTCPSocket::sSDLNetTCPSocket(_TCPsocket* e_pSocket)
	{
		NetworkType = eNT_TCP;
		Socket.pTCPIPSocket = e_pSocket;
	}
	sSDLNetTCPSocket::sSDLNetTCPSocket(IPaddress e_IPaddress)
	{
		Socket.pUDPIPaddress = new IPaddress();
		*Socket.pUDPIPaddress = e_IPaddress;
		NetworkType = eNT_UDP;
	}
	sSDLNetTCPSocket::sSDLNetTCPSocket(size_t e_uiWebSocketAddress)
	{
		Socket.uiWebSocketHandlerAddress = e_uiWebSocketAddress;
		NetworkType = eNT_WEBSOCKET;
	}
	sSDLNetTCPSocket::~sSDLNetTCPSocket()
	{
		if (NetworkType == eNT_TCP)
		{
			if (Socket.pTCPIPSocket)
			{
				SDLNet_TCP_Close(Socket.pTCPIPSocket);
			}
		}
		else
		if (NetworkType == eNT_UDP)
		{
			SAFE_DELETE(Socket.pUDPIPaddress);
		}
	}
	SDLNetSocket GetSDLNetSocket(_TCPsocket* e_pTCPsocket)
	{
		return make_shared<sSDLNetTCPSocket>(e_pTCPsocket);
	}
	SDLNetSocket	GetSDLNetSocket(IPaddress e_IPaddress)
	{
		return make_shared<sSDLNetTCPSocket>(e_IPaddress);
	}
	SDLNetSocket	GetSDLNetSocket(size_t e_uiWebSocketAddress)
	{
		return make_shared<sSDLNetTCPSocket>(e_uiWebSocketAddress);
	}
	void DumpIPInfo(_TCPsocket e_TCPsocket, const char* e_strInfo)
	{
		auto l_pIPaddress = SDLNet_TCP_GetPeerAddress(&e_TCPsocket);
		if (l_pIPaddress)
		{
			auto l_ipaddr = SDL_SwapBE32(l_pIPaddress->host);
			auto l_strInfo = UT::ComposeMsgByFormat("Time:%s---IP %d.%d.%d.%d--Port:%hu\n %s",
				GetSystemTime().c_str(),
				l_ipaddr >> 24,
				(l_ipaddr >> 16) & 0xff,
				(l_ipaddr >> 8) & 0xff,
				l_ipaddr & 0xff,
				l_pIPaddress->port,
				e_strInfo);
			FMLog::Log(l_strInfo.c_str(), false);
		}
	}
	bool	g_bDumpsNetworkReceivedPacketReceiveData = false;
	void	DumpPacketData(_TCPsocket e_pTCPsocket, int e_iPacketSize, char* e_pData, const char* e_strInfo)
	{
		if (g_bDumpsNetworkReceivedPacketReceiveData)
		{
			//e_pTCPsocket->localAddress
			std::string l_strPacketData = "";
			unsigned char* l_pTempPointer = (unsigned char*)&e_iPacketSize;
			//first 4 byte is packet size
			char	l_temp[TEMP_SIZE];
			for (int i = 0; i < PACKET_HEADER_SIZE; ++i)
			{
				sprintf(l_temp, "%03d", l_pTempPointer[i]);
				l_strPacketData += l_temp;
				l_strPacketData += " ";
			}
			l_pTempPointer = (unsigned char*)e_pData;
			for (int i = 0; i < e_iPacketSize; ++i)
			{
				sprintf(l_temp, "%03d", l_pTempPointer[i]);
				l_strPacketData += l_temp;
				if (i>0 && i % 10 == 0)
				{
					l_strPacketData += "\n";
				}
				else
				{
					l_strPacketData += " ";
				}
			}
			DumpIPInfo(e_pTCPsocket, UT::ComposeMsgByFormat("%s %s", e_strInfo,l_strPacketData.c_str()).c_str());
		}
	}


	TYPDE_DEFINE_MARCO(cGameNetwork)
#ifdef WIN32
#include "sensapi.h"
#pragma comment(lib, "Sensapi.lib")
	bool	IsNetworkAlive()
	{
		DWORD dw;
		bool l_bResult = IsNetworkAlive(&dw);
		return l_bResult;
	}
#else
	bool	IsNetworkAlive()
	{
		return true;
	}
#endif
	sNetworkSendPacket::sNetworkSendPacket() { pData = nullptr; iSize = 0; }
	sNetworkSendPacket::sNetworkSendPacket(sNetworkSendPacket * e_pNetworkSendPacket)
	{
		this->iSize = e_pNetworkSendPacket->iSize;
		this->pData = new char[iSize];
		memcpy(pData,e_pNetworkSendPacket->pData, iSize);
	}
	sNetworkSendPacket::sNetworkSendPacket(char* e_pData, int e_iDataLength)
	{
		this->iSize = e_iDataLength;
		this->pData = new char[iSize];
		memcpy(pData, e_pData, iSize);
	}
	sNetworkSendPacket::~sNetworkSendPacket() { SAFE_DELETE_ARRAY(pData); }
	sNetworkReceivedPacket::sNetworkReceivedPacket() { pData = nullptr; iSize = 0; }
	sNetworkReceivedPacket::~sNetworkReceivedPacket() { SAFE_DELETE_ARRAY(pData); }
	int	sNetworkReceivedPacket::ReceiveTCPData(SDLNetSocket e_pTCPsocket)
	{
		assert(iSize == 0);
		pReceivedSocket = e_pTCPsocket;
		//first received size
		if (!SDLNet_SocketReady(e_pTCPsocket->Socket.pTCPIPSocket))
			return false;
		//sNetworkSendPacket,first is size,second is data structure
		int l_iHeaderSize = PACKET_HEADER_SIZE;
		int	l_iLength = SDLNet_TCP_Recv(e_pTCPsocket->Socket.pTCPIPSocket, &iSize, l_iHeaderSize);
		//l_iLength = SDL_SwapBE32(l_iLength);
		if (l_iLength != l_iHeaderSize)
		{
#ifdef DEBUG
			if(l_iLength == 0)
				FMLog::Log("Connection lost", false);
			else
				FMLog::Log("network fetch header size failed(not enough 4 bytes)!?ignore this packet", false);
#endif
			return -1;
		}
		//default TCP/IP packet size restriction is 64k
		if (iSize >= 64 * 1024 || iSize < 1)
		{
#ifdef DEBUG
			FMLog::Log("network data size not correct!?ignore this packet", false);
#endif
			return -1;
		}
		if (iSize == 0)
		{
#ifdef DEBUG
			FMLog::Log("network data size 0!??", false);
#endif
		}
		pData = new char[iSize];
		l_iLength = SDLNet_TCP_Recv(e_pTCPsocket->Socket.pTCPIPSocket, pData, iSize);
		//assert(iSize == l_iLength && "network data size not correct!?");
		if (iSize == l_iLength)
		{
			DumpPacketData(*e_pTCPsocket->Socket.pTCPIPSocket, iSize, pData, "ReceiveTCPData");
			return iSize;
		}
		FMLog::Log(UT::ComposeMsgByFormat("network data size1 not match:Desire%d,RealRecv:%d", iSize, l_iLength).c_str(), true);
		//try again
		int l_iNotEnoughSize = iSize - l_iLength;
		l_iLength = SDLNet_TCP_Recv(e_pTCPsocket->Socket.pTCPIPSocket, &pData[l_iLength], l_iNotEnoughSize);
		if (l_iLength == l_iNotEnoughSize)
		{
			return iSize;
		}
		SAFE_DELETE_ARRAY(pData);
		FMLog::Log(UT::ComposeMsgByFormat("network data size2 not match:Desire%d,RealRecv:%d", iSize, l_iLength).c_str(), true);
		//size not correct!?
		//return l_iLength - iSize;
		//now I dont wannt support streamming so fuck you please reduce packet size.
		return -1;
	}

	int sNetworkReceivedPacket::UDPReceiveDataWithoutHeaderSize(SDLNetSocket e_pTCPsocket,int e_iDataLen, char * e_pData)
	{
		pReceivedSocket = e_pTCPsocket;
		if (e_iDataLen <= 0)
			return -1;
		this->iSize = e_iDataLen;
		pData = new char[iSize];
		memcpy(pData, e_pData, iSize);
		return iSize;
	}

	int sNetworkReceivedPacket::WebSocketReceiveDataWithoutHeaderSize(SDLNetSocket e_pTCPsocket,int e_iDataLen, const char* e_pData)
	{
		pReceivedSocket = e_pTCPsocket;
		if (e_iDataLen <= 0)
			return -1;
		this->iSize = e_iDataLen;
		pData = new char[iSize];
		memcpy(pData, e_pData, iSize);
		return iSize;
	}
	void	cGameNetwork::SetConnectionLostCallbackFunction(std::function<void()> e_Function)
	{
		m_ConnectionLostCallbackFunction = e_Function;
	}

	void cGameNetwork::SetOpenSocketOkayCallback(std::function<void(SDLNetSocket)> e_Function)
	{
		m_SocketOpenOkayCallback = e_Function;
	}

	void	cGameNetwork::SetClientLostConnectionCallback(std::function<void(SDLNetSocket)> e_Function)
	{
		m_ClientLostConnectionCallback = e_Function;
	}

	std::vector<sNetworkReceivedPacket*>		cGameNetwork::GetReceivedDataPleaseDeleteAfterUseIt()
	{
		std::vector<sNetworkReceivedPacket*> l_Vector;
		{
			MUTEX_PLACE_HOLDER(m_ReceivedDataMutex,"cGameNetwork::GetReceivedDataPleaseDeleteAfterUseIt");
			if (m_ReceivedDataVector.size())
			{
				l_Vector = m_ReceivedDataVector;
				m_ReceivedDataVector.clear();
			}
		}
		return l_Vector;
	}
	cGameNetwork::cGameNetwork() :cCPP11Thread(this)
	{
		SDLNet_Init();
#ifdef WASM
		m_iBridgeSocket = -1;
#endif
		m_pReconnectFunction = nullptr;
		m_pAllSocketToListenClientMessage = nullptr;
		m_pSocket = nullptr;
		m_eNetWorkStatus = eNWS_NONE;
		m_ConnectionLostCallbackFunction = nullptr;
		m_bDoDisconnect = false;
	}
	cGameNetwork::~cGameNetwork()
	{
		this->Destroy();
		SDLNet_Quit();
	}
	bool cGameNetwork::GetIP()
	{
		char l_strName[MAX_PATH];
		if (gethostname(l_strName, (int)sizeof(l_strName)) == SOCKET_ERROR)
		{
			return false;
		}
		hostent *l_pPhe = gethostbyname(l_strName);
		if (l_pPhe == 0)
		{
			return false;
		}
		//
		//for (int i = 0; l_pPhe->h_addr_list[i] != 0; ++i) 
		//{
		//	struct in_addr addr;
		//	memcpy(&addr, l_pPhe->h_addr_list[i], sizeof(struct in_addr));
		//	//server machine might has more than one network adapter,just get first one.
		//	if( i == 0 )
		//		memcpy(m_MyIP,&addr.S_un.S_un_b,sizeof(char)*4);
		//}

		m_IPData.m_strIP = inet_ntoa(*(struct in_addr *)*l_pPhe->h_addr_list);
		sprintf(l_strName, "%s", m_IPData.m_strIP.c_str());


		char*	l_str = strtok(l_strName, ".");
		int	l_iIndex = 0;
		while (l_str)
		{
			m_IPData.m_MyIP[l_iIndex] = atoi(l_str);
			l_str = strtok(0, ".");
			++l_iIndex;
		}
		//printf("IP:%s",l_str);
		return true;
	}
	void cGameNetwork::Init()
	{
		Destroy();
	}
	void cGameNetwork::Update(float e_fElpaseTime)
	{
		//if (!m_pSocket->iServerFlag)
		//{

		//}
		if (this->m_pReconnectFunction)
		{
			m_pReconnectFunction->DoReconnectWhileLostConnection(e_fElpaseTime);
		}
	}
	void cGameNetwork::Destroy()
	{
		SAFE_DELETE(m_pReconnectFunction);
		this->CloseThreadAndWaitUntilFinish();
		CloseSocket();
	}
	void cGameNetwork::SetDoDisconnect(bool e_bDisConnect, bool e_bDeleteReConnect)
	{
		m_bDoDisconnect = e_bDisConnect;
		if (e_bDeleteReConnect)
		{
			SAFE_DELETE(this->m_pReconnectFunction);
		}
	}

	bool	cGameNetwork::SendDataToClient(SDLNetSocket e_pTCPsocket, char* e_pData, int e_iDataLength, bool e_bSnedByNetworkThread)
	{
		//if e_pTCPsocket is invalid sent will be failed,so don't need to do mutex here(I can't control player lost connection.)
		if (e_pTCPsocket && this->m_pSocket)
		{
			if (e_bSnedByNetworkThread)
			{
				MUTEX_PLACE_HOLDER(m_SendDataMutex, "cGameNetwork::SendData");
				std::vector<sNetworkSendPacket*>* l_pNetworkSendPacketVector = nullptr;
				if (m_WaitToSendPacketVector.find(e_pTCPsocket) == m_WaitToSendPacketVector.end())
				{
					m_WaitToSendPacketVector.insert(std::make_pair(e_pTCPsocket, std::vector<sNetworkSendPacket*>()));
				}
				l_pNetworkSendPacketVector = &m_WaitToSendPacketVector[e_pTCPsocket];
				l_pNetworkSendPacketVector->push_back(new sNetworkSendPacket(e_pData,e_iDataLength));
			}
			else
			{
				sNetworkSendPacket l_NetworkSendPacket;
				l_NetworkSendPacket.iSize = e_iDataLength;
				l_NetworkSendPacket.pData = e_pData;
				InternalSendData(e_pTCPsocket, &l_NetworkSendPacket);
				l_NetworkSendPacket.pData = nullptr;
			}
			return true;
		}
		return false;
	}

	bool cGameNetwork::SendDataToAllClient(char* e_pData, int e_iDataLength, bool e_bSnedByNetworkThread)
	{
		if (e_pData == nullptr || e_iDataLength<=0)
			return false;
		bool l_bSendDataResult = false;
		{
			MUTEX_PLACE_HOLDER(m_ClientSocketMutex, "cGameNetwork::SendDataToAllClient");
			int	l_iSize = (int)m_ClientSocketVector.size();
			for (int i = 0; i < l_iSize; ++i)
			{
				int* l_pDebugData2 = (int*)e_pData;
				if (m_ClientSocketVector[i])
				{
					bool	l_bSent = false;
					if (e_bSnedByNetworkThread)
					{
						l_bSent = SendDataToClient(m_ClientSocketVector[i], e_pData,e_iDataLength, e_bSnedByNetworkThread);
					}
					else
					{
						sNetworkSendPacket l_NetworkSendPacket;
						l_NetworkSendPacket.iSize = e_iDataLength;
						l_NetworkSendPacket.pData = e_pData;
						InternalSendData(m_ClientSocketVector[i], &l_NetworkSendPacket);
						l_NetworkSendPacket.pData = nullptr;
					}
					if (!l_bSent)
					{
						l_bSendDataResult = false;
					}
				}
			}
		}
		return l_bSendDataResult;
	}


	bool cGameNetwork::SendData(SDLNetSocket e_pTCPsocket, sNetworkSendPacket * e_pPacket, bool e_bSnedByNetworkThread)
	{
		//if e_pTCPsocket is invalid sent will be failed,so don't need to do mutex here(I can't control player lost connection.)
		if (e_pTCPsocket && this->m_pSocket)
		{
			if (e_bSnedByNetworkThread)
			{
				MUTEX_PLACE_HOLDER(m_SendDataMutex, "cGameNetwork::SendData");
				std::vector<sNetworkSendPacket*>* l_pNetworkSendPacketVector = nullptr;
				if (m_WaitToSendPacketVector.find(e_pTCPsocket) == m_WaitToSendPacketVector.end())
				{
					m_WaitToSendPacketVector.insert(std::make_pair(e_pTCPsocket, std::vector<sNetworkSendPacket*>()));
				}
				l_pNetworkSendPacketVector = &m_WaitToSendPacketVector[e_pTCPsocket];
				l_pNetworkSendPacketVector->push_back(new sNetworkSendPacket(e_pPacket));
			}
			else
			{
				InternalSendData(e_pTCPsocket,e_pPacket);
			}
			return true;
		}
		return false;
	}
	bool cGameNetwork::SendDataToServer(sNetworkSendPacket * e_pPacket, bool e_bSnedByNetworkThread)
	{
		if (m_pSocket)
		{
			return SendData(this->m_pSocket, e_pPacket, e_bSnedByNetworkThread);
		}
		return false;
	}
	bool cGameNetwork::SendDataToAllClient(sNetworkSendPacket * e_pPacket, bool e_bSnedByNetworkThread)
	{
		if (!e_pPacket)
			return false;
		bool l_bSendDataResult = false;
		{
			MUTEX_PLACE_HOLDER(m_ClientSocketMutex, "cGameNetwork::SendDataToAllClient");
			int	l_iSize = (int)m_ClientSocketVector.size();
			for (int i = 0; i < l_iSize; ++i)
			{
				int* l_pDebugData2 = (int*)e_pPacket->pData;
				if (m_ClientSocketVector[i])
				{
					bool	l_bSent = false;
					if (e_bSnedByNetworkThread)
					{
						l_bSent = SendData(m_ClientSocketVector[i], e_pPacket,e_bSnedByNetworkThread);
					}
					else
					{
						this->InternalSendData(m_ClientSocketVector[i], e_pPacket);
					}
					if (!l_bSent)
					{
						l_bSendDataResult = false;
					}
				}
			}
		}
		return l_bSendDataResult;
	}

	bool cGameNetwork::CreateAsServer(int e_iPort,bool e_bCreateReconnectFunction, float e_fReconnectionTimeGap)
	{
		m_IPData.m_iPort = e_iPort;
		if(1)
		{
			f_ThreadWorkingFunction l_f_ThreadWorkingFunction = std::bind(&cGameNetwork::ServerListenDataThread, this, std::placeholders::_1);
			this->ThreadDetach(l_f_ThreadWorkingFunction,"cGameNetwork::CreateAsServer");
			if (!this->m_pReconnectFunction && e_bCreateReconnectFunction)
			{
				m_pReconnectFunction = new sReconnectFunction(this,true, e_fReconnectionTimeGap);
			}
			return true;
		}
		return false;
	}
	bool cGameNetwork::CreateAsClient(int e_iPort, const char * e_strIP, bool e_bCreateReconnectFunction, float e_fReconnectionTimeGap)
	{
		m_IPData.m_iPort = e_iPort;
		m_IPData.m_strServerIP = e_strIP;
#ifdef WASM
		//if (m_iBridgeSocket == -1)
		{
			this->m_iBridgeSocket = WASMPOSIXWakeup(m_IPData.m_iPort, m_IPData.m_strServerIP.c_str());
		}
#endif
		if(1)
		{
			f_ThreadWorkingFunction l_f_ThreadWorkingFunction = std::bind(&cGameNetwork::ClientListenDataThread, this, std::placeholders::_1);
			this->ThreadDetach(l_f_ThreadWorkingFunction,"cGameNetwork::CreateAsClient");
			if (!this->m_pReconnectFunction && e_bCreateReconnectFunction)
			{
				m_pReconnectFunction = new sReconnectFunction(this, false, e_fReconnectionTimeGap);
			}
			return true;
		}
		return false;
	}
	bool cGameNetwork::OpenSocket(int e_iPort, const char * e_strIP)
	{
		if (this->m_pSocket)
		{
			if (m_eNetWorkStatus == eNWS_CONNECTED)
			{
				return false;
			}
			this->Destroy();
		}
		m_eNetWorkStatus = eNWS_TRY_TO_CONNECT;
		m_IPData.m_iPort = e_iPort;
		if (e_strIP)
		{
			m_IPData.m_strHost = e_strIP;
		}
#ifdef WASM
		if (g_bDoinit_websocket && m_iBridgeSocket != -1)
		{
			unsigned short readyState = 0;
			Sleep(2000);
			EMSCRIPTEN_WEBSOCKET_READY(m_iBridgeSocket, &readyState);
			if (readyState == 0)
			{
				FMLog::Log("WASM websocket not ready yet.", false);
				return false;
			}
		}
#endif
		FMLog::LogWithFlag(UT::ComposeMsgByFormat("fetch IP :Port: %d", m_IPData.m_iPort), CORE_LOG_FLAG);
		/* Resolve the argument into an IPaddress type */
		if (SDLNet_ResolveHost((IPaddress*)&m_IPData.m_IP, e_strIP, m_IPData.m_iPort) == -1)
		{
			FMLog::LogWithFlag("fetch IP failed", CORE_LOG_FLAG);
			this->m_eNetWorkStatus = eNWS_NO_INTERNET;
			return false;
		}
		UINT l_uiAddr = SDL_SwapBE32(m_IPData.m_IP.host);
		FMLog::LogWithFlag(UT::ComposeMsgByFormat("IP Address : %d.%d.%d.%d",
			l_uiAddr >> 24,(l_uiAddr >> 16) & 0xff,(l_uiAddr >> 8) & 0xff,l_uiAddr & 0xff), CORE_LOG_FLAG);
		auto l_pSDLTCPSocket = SDLNet_TCP_Open((IPaddress*)&m_IPData.m_IP);
		//if SDLNet_TCP_Open called server's SDLNet_TCP_Accept will be triggered
		if (!l_pSDLTCPSocket)
		{
			FMLog::LogWithFlag("socket open failed", CORE_LOG_FLAG);
			this->m_eNetWorkStatus = eNWS_CONNECTION_NOT_EXISTS;
			return false;
		}
		m_pSocket = GetSDLNetSocket(l_pSDLTCPSocket);
		FMLog::LogWithFlag("socket open", CORE_LOG_FLAG);
		if (m_SocketOpenOkayCallback)
		{
			m_SocketOpenOkayCallback(m_pSocket);
		}
		return true;
	}
	void cGameNetwork::CloseSocket()
	{
		FMLog::LogWithFlag("cGameNetwork::CloseSocket", CORE_LOG_FLAG);
		if (m_pSocket)
		{
			auto l_pSocket = m_pSocket;
			m_pSocket = nullptr;
			RemoveAllClient();
			DELETE_VECTOR(m_ReceivedDataVector);
			m_ClientSocketVector.clear();;
			if (m_pAllSocketToListenClientMessage)
			{
				SDLNet_FreeSocketSet(m_pAllSocketToListenClientMessage);
				m_pAllSocketToListenClientMessage = 0;
			}
			//this will called while this brace is exit SDLNet_TCP_Close(l_pSocket->Socket.pTCPIPSocket);
		}
	}
	void cGameNetwork::RemoveAllClient()
	{
		MUTEX_PLACE_HOLDER(m_ClientSocketMutex, "cGameNetwork::RemoveAllClient");
		m_ClientSocketVector.clear();
	}

	bool cGameNetwork::ThreadProcessWaitSendDataVector()
	{
		std::map<SDLNetSocket, std::vector<sNetworkSendPacket*> > l_WaitToSendPacketVector;
		{
			MUTEX_PLACE_HOLDER(m_SendDataMutex, "cGameNetwork::ThreadProcessWaitSendDataVector");
			if (m_WaitToSendPacketVector.size() == 0)
				return true;
			l_WaitToSendPacketVector = m_WaitToSendPacketVector;
			m_WaitToSendPacketVector.clear();
		}
		for (auto l_It = l_WaitToSendPacketVector.begin(); l_It != l_WaitToSendPacketVector.end(); ++l_It)
		{
			for (auto l_DataIT = l_It->second.begin(); l_DataIT != l_It->second.end(); ++l_DataIT)
			{
				if (!InternalSendData(l_It->first, *l_DataIT))
				{//dump log.

				}
			}
			DELETE_VECTOR(l_It->second);
		}
		return false;
	}

	bool cGameNetwork::InternalSendData(SDLNetSocket e_pTCPsocket, sNetworkSendPacket * e_pPacket)
	{
		//if e_pTCPsocket is invalid sent will be failed,so don't need to do mutex here(I can't control player lost connection.)
		if (e_pTCPsocket)
		{
			int l_iHeaderSize = PACKET_HEADER_SIZE;
			int	l_iSendSize = (int)(l_iHeaderSize + e_pPacket->iSize);
			bool	l_bSent = false;
			bool l_bUsealloca = true;//for better memory management
			if (l_bUsealloca)
			{
				char* l_pData = (char*)alloca(l_iSendSize);
				memcpy(l_pData, &e_pPacket->iSize, l_iHeaderSize);
				memcpy(&l_pData[l_iHeaderSize], e_pPacket->pData, e_pPacket->iSize);
				l_bSent = SDLNet_TCP_Send(e_pTCPsocket->Socket.pTCPIPSocket, l_pData, l_iSendSize) == 0 ? false : true;
			}
			else
			{
				char* l_pData = new char[l_iSendSize];
				memcpy(l_pData, &e_pPacket->iSize, l_iHeaderSize);
				memcpy(&l_pData[l_iHeaderSize], e_pPacket->pData, e_pPacket->iSize);
				l_bSent = SDLNet_TCP_Send(e_pTCPsocket->Socket.pTCPIPSocket, l_pData, l_iSendSize) == 0 ? false : true;
				delete[] l_pData;
			}
			DumpPacketData(*e_pTCPsocket->Socket.pTCPIPSocket, e_pPacket->iSize, e_pPacket->pData, "send data");
			return l_bSent;
		}
		return false;
	}
	void cGameNetwork::AddClient(SDLNetSocket e_pTCPsocket)
	{
		MUTEX_PLACE_HOLDER(m_ClientSocketMutex, "cGameNetwork::AddClient");
#ifdef DEBUG
		DumpIPInfo(*e_pTCPsocket->Socket.pTCPIPSocket, " new connection");
		int	l_iSize = (int)m_ClientSocketVector.size();
		for (int i = 0; i < l_iSize; ++i)
		{
			if (m_ClientSocketVector[i]->Socket.pTCPIPSocket->channel == e_pTCPsocket->Socket.pTCPIPSocket->channel)
			{
				UT::ErrorMsg(L"Client was exist", L"add new client error");
				return;
			}
		}
#endif
		m_ClientSocketVector.push_back(e_pTCPsocket);
	}	

	bool	cGameNetwork::RemoveClient(SDLNetSocket e_pTCPsocket)
	{
		MUTEX_PLACE_HOLDER(m_ClientSocketMutex, "cGameNetwork::RemoveClient");
		return RemoveClientWhileClientLostConnection(e_pTCPsocket);
	}

	bool	cGameNetwork::RemoveClientWhileClientLostConnection(SDLNetSocket e_pTCPsocket)
	{
		if (!e_pTCPsocket)
			return false;
		/* close the old socket, even if it's dead... */
		int	l_iSize = (int)m_ClientSocketVector.size();
		for (int i = 0; i < l_iSize; ++i)
		{
			if (m_ClientSocketVector[i] == e_pTCPsocket)
			{
				{
					MUTEX_PLACE_HOLDER(m_SendDataMutex, "cGameNetwork::RemoveClientWhileClientLostConnection");
					auto l_IT = m_WaitToSendPacketVector.find(e_pTCPsocket);
					if (l_IT != m_WaitToSendPacketVector.end())
					{
						DELETE_VECTOR(l_IT->second);
						m_WaitToSendPacketVector.erase(l_IT);
					}
				}
				if (m_ClientLostConnectionCallback)
					m_ClientLostConnectionCallback(e_pTCPsocket);
				//SDLNet_TCP_Close will be called if no reference?
				m_ClientSocketVector.erase(m_ClientSocketVector.begin() + i);
				return true;
			}
		}
		return false;
	}

	bool cGameNetwork::CreateSocksetToListenData()
	{
		//FMLog::LogWithFlag("create socket");
		if (m_pAllSocketToListenClientMessage)
			SDLNet_FreeSocketSet(m_pAllSocketToListenClientMessage);
		int	l_iNumClient = 0;
		//is it server?
		MUTEX_PLACE_HOLDER(m_ClientSocketMutex, "cGameNetwork::CreateSocksetToListenData");
		if (m_pSocket->Socket.pTCPIPSocket->iServerFlag)
			l_iNumClient = (int)m_ClientSocketVector.size();
		//+1 fo server
		m_pAllSocketToListenClientMessage = SDLNet_AllocSocketSet(l_iNumClient + 1);
		if (!m_pAllSocketToListenClientMessage)
		{
			//FMLog::LogWithFlag("SDLNet_AllocSocketSet: %s", SDL_GetError());
			//exit(1); /*most of the time this is a major error, but do what you want. */
			return false;
		}

		SDLNet_TCP_AddSocket(m_pAllSocketToListenClientMessage, m_pSocket->Socket.pTCPIPSocket);
		for (int i = 0; i < l_iNumClient; i++)
			SDLNet_TCP_AddSocket(m_pAllSocketToListenClientMessage, m_ClientSocketVector[i]->Socket.pTCPIPSocket);
		return true;
	}

	//typedef std::function<void()> f_PingFailedLostConnection;

	//l_sfElpaseTime += e_ElpaseTime;
	//if (l_sfElpaseTime >= l_sfTryToPingServerTC)
	//{
	//	UT::sTimeAndFPS l_TimeAndFPS;
	//	l_TimeAndFPS.Update();
	//	auto l_strTargetIP = UT::ComposeMsgByFormat("ping -c1 -s1 %s  > /dev/null 2>&1", m_IPData.m_strServerIP.c_str());
	//	int x = system(l_strTargetIP.c_str());
	//	l_TimeAndFPS.Update();
	//	float l_fTime = l_TimeAndFPS.fElpaseTime;
	//	if (x == 0)
	//	{//cout << "success" << endl;
	//	}
	//	else
	//	{
	//		goto FAILED;
	//	}
	//	l_sfElpaseTime = 0.f;
	//}

	//void PingIP(const char*e_strIP, f_PingFailedLostConnection e_f_PingFailedLostConnection,bool e_bKeepThread)
	//{
	//	UT::sTimeCounter l_TC;
	//	l_TC.SetTargetTime(5.f);
	//	UT::sTimeAndFPS l_TimeAndFPS;
	//	l_TimeAndFPS.Update();
	//	while (e_bKeepThread)
	//	{
	//		Sleep(100);
	//		l_TimeAndFPS.Update();
	//		l_TC.Update(l_TimeAndFPS.fElpaseTime);
	//		if (l_TC.bTragetTimrReached)
	//		{
	//			auto l_strTargetIP = UT::ComposeMsgByFormat("ping -c1 -s1 %s  > /dev/null 2>&1", e_strIP);
	//			int x = system(l_strTargetIP.c_str());
	//			l_TimeAndFPS.Update();
	//			float l_fTime = l_TimeAndFPS.fElpaseTime;
	//			if (x == 0)
	//			{//cout << "success" << endl;

	//			}
	//			else
	//			{
	//				e_f_PingFailedLostConnection();
	//			}
	//			l_TC.Start();
	//		}
	//	}
	//}

	void	cGameNetwork::ClientListenDataThread(float e_ElpaseTime)
	{
		eNetWorkStatus l_eNetWorkStatus = m_eNetWorkStatus;
		if (!m_pSocket)
		{
			if (!OpenSocket(m_IPData.m_iPort, m_IPData.m_strServerIP.c_str()))
			{
				goto FAILED;
			}
		}
		m_eNetWorkStatus = eNWS_CONNECTED;
		if (m_pSocket)
		{
			assert(m_pSocket && !m_pSocket->Socket.pTCPIPSocket->iServerFlag && "cGameNetwork::ClientListenDataThread");
			if (!IsNetworkAlive())
			{
				FMLog::LogWithFlag("Network not aviable", CORE_LOG_FLAG);
				goto FAILED;
			}
			ThreadProcessWaitSendDataVector();
			int l_iNumready = 0;
			if (!CreateSocksetToListenData())
			{
				FMLog::LogWithFlag("CreateSocksetToListenData failed", CORE_LOG_FLAG);
				goto FAILED;
			}
			//FMLog::LogWithFlag("listen");
			l_iNumready = SDLNet_CheckSockets(m_pAllSocketToListenClientMessage, (UINT)10);
			if (l_iNumready == -1)
			{
				//FMLog::LogWithFlag("SDLNet_CheckSockets: %s",SDL_GetError());
				FMLog::LogWithFlag("SDLNet_CheckSockets failed", CORE_LOG_FLAG);
				goto FAILED;
			}
			if (l_iNumready == 0)
			{
				return;
			}
			if (m_pSocket)
			{
				bool	l_bFraomTemplateData = false;
				sNetworkReceivedPacket*l_pPacket = new sNetworkReceivedPacket();
				int l_iReceivedSize = l_pPacket->ReceiveTCPData(m_pSocket);
				if (l_iReceivedSize <= 0)
				{//wrong data ignore it
					FMLog::LogWithFlag("l_pPacket->ReceiveTCPData failed", CORE_LOG_FLAG);
					bool	l_bConnectionFailed = false;
					delete l_pPacket;
					goto FAILED;
				}
				{
					//FMLog::LogWithFlag("recevied message", CORE_LOG_FLAG);
					MUTEX_PLACE_HOLDER(m_ReceivedDataMutex, "cGameNetwork::ClientListenDataThread");
					this->m_ReceivedDataVector.push_back(l_pPacket);
				}
			}
		}
		if(!m_bDoDisconnect)
			return;
	FAILED:
		FMLog::LogWithFlag("cGameNetwork::ClientListenDataThread::disconnect", CORE_LOG_FLAG);
		m_bDoDisconnect = false;
		CloseSocket();
		m_eNetWorkStatus = eNWS_LOST_CONNECTION;
		if (l_eNetWorkStatus == eNetWorkStatus::eNWS_CONNECTED)
		{
			if (m_ConnectionLostCallbackFunction)
				m_ConnectionLostCallbackFunction();
		}
		this->m_bLeaveThread = true;
	}

	void cGameNetwork::ServerListenDataThread(float e_ElpaseTime)
	{
		eNetWorkStatus l_eNetWorkStatus = m_eNetWorkStatus;
		if (!m_pSocket)
		{
			if(!OpenSocket(m_IPData.m_iPort,nullptr))
			{
				goto FAILED;
			}
		}
		m_eNetWorkStatus = eNWS_CONNECTED;
		assert(m_pSocket->Socket.pTCPIPSocket->iServerFlag&&"server update thread require a server flag");
		if(m_pSocket)
		{
			if (!IsNetworkAlive())
			{
				goto FAILED;
			}
			ThreadProcessWaitSendDataVector();
			int l_iNumready = 0;
			if (!CreateSocksetToListenData())
			{
				goto FAILED;
			}
			//FMLog::LogWithFlag("listen");
			l_iNumready = SDLNet_CheckSockets(m_pAllSocketToListenClientMessage, (UINT)10);
			if (l_iNumready == -1)
			{
				//FMLog::LogWithFlag("SDLNet_CheckSockets: %s",SDL_GetError());
				goto FAILED;
			}
			if (l_iNumready == 0)
				return;
			//new client?
			if (m_pSocket&&m_pSocket->Socket.pTCPIPSocket->iServerFlag && SDLNet_SocketReady(m_pSocket->Socket.pTCPIPSocket))
			{
				if (m_pSocket->Socket.pTCPIPSocket->iServerFlag)
				{
					TCPsocket l_pNewClient = SDLNet_TCP_Accept(m_pSocket->Socket.pTCPIPSocket);
					if (l_pNewClient)
					{
						AddClient(GetSDLNetSocket(l_pNewClient));
						FMLog::LogWithFlag("new client", CORE_LOG_FLAG);
						//continue;
					}
				}
			}
			if (m_pSocket)
			{
				if (m_pSocket->Socket.pTCPIPSocket->iServerFlag)
				{
					MUTEX_PLACE_HOLDER(m_ClientSocketMutex, "cGameNetwork::ServerListenDataThread1");
					int	l_iNumClients = (int)m_ClientSocketVector.size();
					for (int i = 0; i < l_iNumClients; i++)
					{
						_TCPsocket*l_pClient = m_ClientSocketVector[i]->Socket.pTCPIPSocket;
						if (SDLNet_SocketReady(l_pClient))
						{
							bool	l_bFraomTemplateData = false;
							sNetworkReceivedPacket*l_pPacket = new sNetworkReceivedPacket();
							int l_iResult = l_pPacket->ReceiveTCPData(m_ClientSocketVector[i]);
							if(l_iResult > 0)
							{
								//FMLog::LogWithFlag("recevied message", CORE_LOG_FLAG);
								MUTEX_PLACE_HOLDER(m_ReceivedDataMutex, "cGameNetwork::ServerListenDataThread2");
								m_ReceivedDataVector.push_back(l_pPacket);
								if(0)
								{
									static UT::sTimeAndFPS l_siPacketReceivedFPS;
									l_siPacketReceivedFPS.Update();
									if (l_siPacketReceivedFPS.fElpaseTime > 1.f)
									{
										FMLog::Log(UT::ComposeMsgByFormat("PacketReceived:%.3f", l_siPacketReceivedFPS.fElpaseTime).c_str(), false);
									}
								}
							
							}
							else
							{//wrong data ignore it
								delete l_pPacket;
								FMLog::LogWithFlag("client connection failed", CORE_LOG_FLAG);
								if (!RemoveClientWhileClientLostConnection(m_ClientSocketVector[i]))
								{
									//assert(0&&"no this client");
								}
								else
								{
									--l_iNumClients;
									--i;
								}
							}
						}
					}
				}
			}
		}
		if (!m_bDoDisconnect)
			return;
	FAILED:
		FMLog::LogWithFlag("cGameNetwork::ServerListenDataThread::disconnect", CORE_LOG_FLAG);
		m_bDoDisconnect = false;
		CloseSocket();
		m_eNetWorkStatus = eNWS_LOST_CONNECTION;
		if (l_eNetWorkStatus == eNetWorkStatus::eNWS_CONNECTED)
		{
			if (m_ConnectionLostCallbackFunction)
				m_ConnectionLostCallbackFunction();
		}
		this->m_bLeaveThread = true;
	}

	void cGameNetwork::sReconnectFunction::DoReconnectWhileLostConnection(float e_fElpaseTime)
	{
		if (m_pGameNetwork)
		{
			this->m_ReConnectTime.Update(e_fElpaseTime);
			if (m_pGameNetwork->m_eNetWorkStatus != eNWS_CONNECTED &&
				m_pGameNetwork->m_eNetWorkStatus != eNWS_TRY_TO_CONNECT && 
				//make sure thread left.
				!m_pGameNetwork->IsThreadWorking())
			{
				if (m_ReConnectTime.bTragetTimrReached)
				{//make sure its client

#ifdef WASM
					if (m_pGameNetwork->m_IPData.m_strServerIP.length())
					{
						m_pGameNetwork->m_iBridgeSocket = WASMPOSIXWakeup(m_pGameNetwork->m_IPData.m_iPort, m_pGameNetwork->m_IPData.m_strServerIP.c_str());
					}
#endif
					if (m_bServeFlag)
					{
						m_pGameNetwork->CreateAsServer(m_pGameNetwork->m_IPData.m_iPort, false);
					}
					else
					{
						m_pGameNetwork->CreateAsClient(m_pGameNetwork->m_IPData.m_iPort, m_pGameNetwork->m_IPData.m_strServerIP.c_str(), false);
					}
					m_ReConnectTime.Start();
				}
			}
		}
	}
}

//_M_CEE
#endif