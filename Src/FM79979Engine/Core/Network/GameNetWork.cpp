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
namespace FATMING_CORE
{
	sSDLNetTCPSocket::sSDLNetTCPSocket(_TCPsocket* e_pSocket) { pSocket = e_pSocket; }
	sSDLNetTCPSocket::~sSDLNetTCPSocket() { if (pSocket)SDLNet_TCP_Close(pSocket); }
	SDLNetSocket GetSDLNetSocket(_TCPsocket* e_pTCPsocket)
	{
		return make_shared<sSDLNetTCPSocket>(e_pTCPsocket);
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
	sNetworkSendPacket::~sNetworkSendPacket() { SAFE_DELETE(pData); }
	sNetworkReceivedPacket::sNetworkReceivedPacket() { pData = nullptr; iSize = 0; }
	sNetworkReceivedPacket::~sNetworkReceivedPacket() { SAFE_DELETE_ARRAY(pData); }
	int	sNetworkReceivedPacket::ReceiveData(SDLNetSocket e_pTCPsocket)
	{
		assert(iSize == 0);
		pReceivedSocket = e_pTCPsocket;
		//first received size
		if (!SDLNet_SocketReady(e_pTCPsocket->pSocket))
			return false;
		//sNetworkSendPacket,first is size,second is data structure
		int l_iHeaderSize = PACKET_HEADER_SIZE;
		int	l_iLength = SDLNet_TCP_Recv(e_pTCPsocket->pSocket, &iSize, l_iHeaderSize);
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
		l_iLength = SDLNet_TCP_Recv(e_pTCPsocket->pSocket, pData, iSize);
		//assert(iSize == l_iLength && "network data size not correct!?");
		if (iSize == l_iLength)
		{
			DumpPacketData(*e_pTCPsocket->pSocket, iSize, pData, "ReceiveData");
			return iSize;
		}
		FMLog::Log(UT::ComposeMsgByFormat("network data size1 not match:Desire%d,RealRecv:%d", iSize, l_iLength).c_str(), false);
		//try again
		int l_iNotEnoughSize = iSize - l_iLength;
		l_iLength = SDLNet_TCP_Recv(e_pTCPsocket->pSocket, &pData[l_iLength], l_iNotEnoughSize);
		if (l_iLength == l_iNotEnoughSize)
		{
			return iSize;
		}
		SAFE_DELETE_ARRAY(pData);
		FMLog::Log(UT::ComposeMsgByFormat("network data size2 not match:Desire%d,RealRecv:%d", iSize, l_iLength).c_str(), false);
		//size not correct!?
		//return l_iLength - iSize;
		//now I dont wannt support streamming so fuck you please reduce packet size.
		return -1;
	}

	//int sNetworkReceivedPacket::ReceiveData(_UDPsocket*e_pSocket, UDPpacket *e_pPackets)
	//{
	//	auto l_iReceivedBytes = SDLNet_UDP_Recv(e_pSocket, e_pPackets);
	//	if (l_iReceivedBytes != -1)
	//	{
	//		UDPAddress = e_pPackets->address;
	//		if (e_pPackets->len < PACKET_HEADER_SIZE)
	//		{
	//			return -1;
	//		}
	//		this->iSize = *(int*)e_pPackets->data;
	//		int l_iTotalSize = iSize + PACKET_HEADER_SIZE;
	//		if (l_iTotalSize != e_pPackets->len)
	//		{
	//			FMLog::Log("UDP network data size not match!??", false);
	//			return -1;
	//		}
	//		pData = new char[iSize];
	//		memcpy(pData, &e_pPackets->data[PACKET_HEADER_SIZE], iSize);
	//		return iSize;
	//	}
	//	return l_iReceivedBytes;
	//}

	int sNetworkReceivedPacket::UDPReceiveDataWithoutHeaderSize(IPaddress e_UDPIPaddress,int e_iDataLen, char * e_pData)
	{
		UDPIPaddress = e_UDPIPaddress;
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

	void	cGameNetwork::SetClientLostConnectionCallback(std::function<void(SDLNetSocket)> e_Function)
	{
		m_ClientLostConnectionCallback = e_Function;
	}

	std::vector<sNetworkReceivedPacket*>		cGameNetwork::GetReceivedDataPleaseDeleteAfterUseIt()
	{
		std::vector<sNetworkReceivedPacket*> l_Vector;
		{
			cPP11MutexHolder l_PP11MutexHolder(m_ReceivedDataMutex);
			//cPP11MutexHolder l_PP11MutexHolder(m_ReceivedDataMutex, L"GetReceivedDataPleaseDeleteAfterUseIt");
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

	bool cGameNetwork::SendData(SDLNetSocket e_pTCPsocket, sNetworkSendPacket * e_pPacket, bool e_bSnedByNetworkThread)
	{
		//cPP11MutexHolder l_PP11MutexHolder(m_SendDataMutex);
		//if e_pTCPsocket is invalid sent will be failed,so don't need to do mutex here(I can't control player lost connection.)
		if (e_pTCPsocket && this->m_pSocket)
		{
			if (e_bSnedByNetworkThread)
			{
				cPP11MutexHolder l_PP11MutexHolder(m_SendDataMutex);
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
		//cPP11MutexHolder hold(m_ClientSocketMutex, L"SendDataToAllClient");
		{
			cPP11MutexHolder hold(m_ClientSocketMutex);
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
		m_IPData.m_strHost = SDLNet_ResolveIP((IPaddress*)&m_IPData.m_IP);
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
			//this will called while this brace is exit SDLNet_TCP_Close(l_pSocket->pSocket);
		}
	}
	void cGameNetwork::RemoveAllClient()
	{
		//cPP11MutexHolder hold(m_ClientSocketMutex, L"RemoveAllClient");
		cPP11MutexHolder hold(m_ClientSocketMutex);
		m_ClientSocketVector.clear();
	}

	bool cGameNetwork::ThreadProcessWaitSendDataVector()
	{
		std::map<SDLNetSocket, std::vector<sNetworkSendPacket*> > l_WaitToSendPacketVector;
		{
			cPP11MutexHolder l_PP11MutexHolder(m_SendDataMutex);
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
				l_bSent = SDLNet_TCP_Send(e_pTCPsocket->pSocket, l_pData, l_iSendSize) == 0 ? false : true;
			}
			else
			{
				char* l_pData = new char[l_iSendSize];
				memcpy(l_pData, &e_pPacket->iSize, l_iHeaderSize);
				memcpy(&l_pData[l_iHeaderSize], e_pPacket->pData, e_pPacket->iSize);
				l_bSent = SDLNet_TCP_Send(e_pTCPsocket->pSocket, l_pData, l_iSendSize) == 0 ? false : true;
				delete[] l_pData;
			}
			DumpPacketData(*e_pTCPsocket->pSocket, e_pPacket->iSize, e_pPacket->pData, "send data");
			return l_bSent;
		}
		return false;
	}
	void cGameNetwork::AddClient(SDLNetSocket e_pTCPsocket)
	{
		//cPP11MutexHolder l_PP11MutexHolder(m_ClientSocketMutex, L"AddClient");
		cPP11MutexHolder l_PP11MutexHolder(m_ClientSocketMutex);
#ifdef DEBUG
		DumpIPInfo(*e_pTCPsocket->pSocket, " new connection");
		int	l_iSize = (int)m_ClientSocketVector.size();
		for (int i = 0; i < l_iSize; ++i)
		{
			if (m_ClientSocketVector[i]->pSocket->channel == e_pTCPsocket->pSocket->channel)
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
		cPP11MutexHolder l_PP11MutexHolder(m_ClientSocketMutex);
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
					cPP11MutexHolder l_PP11MutexHolder(m_SendDataMutex);
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
		//cPP11MutexHolder hold(m_ClientSocketMutex, L"CreateSocksetToListenData");
		cPP11MutexHolder hold(m_ClientSocketMutex);
		if (m_pSocket->pSocket->iServerFlag)
			l_iNumClient = (int)m_ClientSocketVector.size();
		//+1 fo server
		m_pAllSocketToListenClientMessage = SDLNet_AllocSocketSet(l_iNumClient + 1);
		if (!m_pAllSocketToListenClientMessage)
		{
			//FMLog::LogWithFlag("SDLNet_AllocSocketSet: %s", SDL_GetError());
			//exit(1); /*most of the time this is a major error, but do what you want. */
			return false;
		}

		SDLNet_TCP_AddSocket(m_pAllSocketToListenClientMessage, m_pSocket->pSocket);
		for (int i = 0; i < l_iNumClient; i++)
			SDLNet_TCP_AddSocket(m_pAllSocketToListenClientMessage, m_ClientSocketVector[i]->pSocket);
		return true;
	}

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
			assert(m_pSocket && !m_pSocket->pSocket->iServerFlag && "cGameNetwork::ClientListenDataThread");
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
				return;
			if (m_pSocket)
			{
				bool	l_bFraomTemplateData = false;
				sNetworkReceivedPacket*l_pPacket = new sNetworkReceivedPacket();
				int l_iReceivedSize = l_pPacket->ReceiveData(m_pSocket);
				if (l_iReceivedSize <= 0)
				{//wrong data ignore it
					FMLog::LogWithFlag("l_pPacket->ReceiveData failed", CORE_LOG_FLAG);
					bool	l_bConnectionFailed = false;
					delete l_pPacket;
					goto FAILED;
				}
				{
					//FMLog::LogWithFlag("recevied message", CORE_LOG_FLAG);
					//cPP11MutexHolder l_PP11MutexHolder(m_ReceivedDataMutex, L"recevied message");
					cPP11MutexHolder l_PP11MutexHolder(m_ReceivedDataMutex);
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
		assert(m_pSocket->pSocket->iServerFlag&&"server update thread require a server flag");
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
			if (m_pSocket&&m_pSocket->pSocket->iServerFlag && SDLNet_SocketReady(m_pSocket->pSocket))
			{
				if (m_pSocket->pSocket->iServerFlag)
				{
					TCPsocket l_pNewClient = SDLNet_TCP_Accept(m_pSocket->pSocket);
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
				if (m_pSocket->pSocket->iServerFlag)
				{
					//cPP11MutexHolder l_PP11MutexHolder(m_ClientSocketMutex,L"m_pSocket->iServerFlag");
					cPP11MutexHolder l_PP11MutexHolder(m_ClientSocketMutex);
					int	l_iNumClients = (int)m_ClientSocketVector.size();
					for (int i = 0; i < l_iNumClients; i++)
					{
						_TCPsocket*l_pClient = m_ClientSocketVector[i]->pSocket;
						if (SDLNet_SocketReady(l_pClient))
						{
							bool	l_bFraomTemplateData = false;
							sNetworkReceivedPacket*l_pPacket = new sNetworkReceivedPacket();
							int l_iResult = l_pPacket->ReceiveData(m_ClientSocketVector[i]);
							if(l_iResult > 0)
							{
								//FMLog::LogWithFlag("recevied message", CORE_LOG_FLAG);
								//cPP11MutexHolder l_PP11MutexHolder(m_ReceivedDataMutex,L"recevied message");
								cPP11MutexHolder l_PP11MutexHolder(m_ReceivedDataMutex);
								m_ReceivedDataVector.push_back(l_pPacket);
								if(0)
								{
									static UT::sTimeAndFPS l_siPacketReceivedFPS;
									l_siPacketReceivedFPS.Update();
									if (l_siPacketReceivedFPS.fElpaseTime > 1.f)
										FMLog::Log(UT::ComposeMsgByFormat("PacketReceived:%.3f", l_siPacketReceivedFPS.fElpaseTime).c_str(), false);
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
				{
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