#ifndef _M_CEE//manage code dont support cCPP11Thread
#include "assert.h"
#include "../Common/Utility.h"
#include "GameNetWork.h"
#include "../Common/Log/FMLog.h"
#ifdef LINUX
#include <alloca.h>
#endif
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
namespace FATMING_CORE
{
	int	sNetworkReceivedPacket::ReceiveData(_TCPsocket*e_pTCPsocket)
	{
		assert(iSize == 0);
		pReceivedSocket = e_pTCPsocket;
		//first received size
		if (!SDLNet_SocketReady(e_pTCPsocket))
			return false;
		//sNetworkSendPacket,first is size,second is data structure
		int l_iHeaderSize = PACKET_HEADER_SIZE;
		int	l_iLength = SDLNet_TCP_Recv(e_pTCPsocket, &iSize, l_iHeaderSize);
		if (l_iLength != l_iHeaderSize)
		{
#ifdef DEBUG
			FMLog::Log("network fetch header size failed(not enough 4 bytes)!?ignore this packet\n", false);
#endif
			return -1;
		}
		//default TCP/IP packet size restriction is 64k
		if (iSize >= 64 * 1024 || iSize < 1)
		{
#ifdef DEBUG
			FMLog::Log("network data size not correct!?ignore this packet\n", false);
#endif
			return -1;
		}
		pData = new char[iSize];
		l_iLength = SDLNet_TCP_Recv(e_pTCPsocket, pData, iSize);
		//assert(iSize == l_iLength && "network data size not correct!?");
		if (iSize == l_iLength)
		{
			return iSize;
		}
		SAFE_DELETE_ARRAY(pData);
		//size not correct!?
		//return l_iLength - iSize;
		//now I dont wannt support streamming so fuck you please reduce packet size.
		return -1;
	}

	void	cGameNetwork::SetConnectionLostCallbackFunction(std::function<void()> e_Function)
	{
		m_ConnectionLostCallbackFunction = e_Function;
	}

	void	cGameNetwork::SetClientLostConnectionCallback(std::function<void(_TCPsocket*)> e_Function)
	{
		m_ClientLostConnectionCallback = e_Function;
	}

	std::vector<sNetworkReceivedPacket*>		cGameNetwork::GetReceivedDataPleaseDeleteAfterUseIt()
	{
		std::vector<sNetworkReceivedPacket*> l_Vector;
		{
			if (m_ReceivedDataVector.size())
			{
				//cPP11MutexHolder l_PP11MutexHolder(m_ReceivedDataMutex, L"GetReceivedDataPleaseDeleteAfterUseIt");
				cPP11MutexHolder l_PP11MutexHolder(m_ReceivedDataMutex);
				for (auto l_pData : m_ReceivedDataVector)
				{
					l_Vector.push_back(l_pData);
				}
				m_ReceivedDataVector.clear();
			}
		}
		return l_Vector;
	}
	cGameNetwork::cGameNetwork()
	{
		SDLNet_Init();
		m_pReconnectFunction = nullptr;
		m_pAllSocketToListenClientMessage = nullptr;
		m_pSocket = nullptr;
		m_eNetWorkStatus = eNWS_NONE;
		m_ConnectionLostCallbackFunction = nullptr;
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
		//printf("IP:%s\n",l_str);
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
	bool cGameNetwork::SendData(_TCPsocket * e_pTCPsocket, sNetworkSendPacket * e_pPacket)
	{
		//if e_pTCPsocket is invalid sent will be failed,so don't need to do mutex here(I can't control player lost connection.)
		if (e_pTCPsocket)
		{
			int l_iHeaderSize = PACKET_HEADER_SIZE;
			int	l_iSendSize = (int)(l_iHeaderSize + e_pPacket->iSize);
			char*l_pData = (char*)alloca(l_iSendSize);
			memcpy(l_pData, &e_pPacket->iSize, l_iHeaderSize);
			memcpy(&l_pData[l_iHeaderSize], e_pPacket->pData, e_pPacket->iSize);
			bool	l_bSent = SDLNet_TCP_Send(e_pTCPsocket, l_pData, l_iSendSize) == 0 ? false : true;
			//delete[] l_pData;
			return l_bSent;
		}
		return false;
	}
	bool cGameNetwork::SendDataToServer(sNetworkSendPacket * e_pPacket)
	{
		if (m_pSocket)
			return SendData(this->m_pSocket, e_pPacket);
		return false;
	}
	bool cGameNetwork::SendDataToAllClient(sNetworkSendPacket * e_pPacket)
	{
		if (!e_pPacket)
			return false;
		int l_iHeaderSize = PACKET_HEADER_SIZE;
		bool l_bSendDataResult = false;
		int	l_iSendSize = (int)(l_iHeaderSize + e_pPacket->iSize);
		char*l_pData = new char[l_iSendSize];
		memcpy(l_pData, &e_pPacket->iSize, l_iHeaderSize);
		memcpy(&l_pData[l_iHeaderSize], e_pPacket->pData, e_pPacket->iSize);
		//cPP11MutexHolder hold(m_ClientSocketMutex, L"SendDataToAllClient");
		cPP11MutexHolder hold(m_ClientSocketMutex);
		int	l_iSize = (int)m_ClientSocketVector.size();
		for (int i = 0; i < l_iSize; ++i)
		{
			int*l_pDebugData = (int*)l_pData;
			int*l_pDebugData2 = (int*)e_pPacket->pData;
			if (m_ClientSocketVector[i])
			{
				bool	l_bSent = SDLNet_TCP_Send(m_ClientSocketVector[i], l_pData, l_iSendSize) == 0 ? false : true;
				assert(l_bSent && "cGameNetwork::SendDataToAllClient-send data failed!?!?");
				if (!l_bSent)
					l_bSendDataResult = false;
			}
		}
		delete[] l_pData;
		return l_bSendDataResult;
	}

	bool cGameNetwork::CreateAsServer(int e_iPort,bool e_bCreateReconnectFunction)
	{
		m_IPData.m_iPort = e_iPort;
		if(1)
		{
			f_ThreadWorkingFunction l_f_ThreadWorkingFunction = std::bind(&cGameNetwork::ServerListenDataThread, this, std::placeholders::_1);
			this->ThreadDetach(l_f_ThreadWorkingFunction);
			if (!this->m_pReconnectFunction && e_bCreateReconnectFunction)
			{
				m_pReconnectFunction = new sReconnectFunction(this,true);
			}
			return true;
		}
		return false;
	}
	bool cGameNetwork::CreateAsClient(int e_iPort, const char * e_strIP, bool e_bCreateReconnectFunction)
	{
		m_IPData.m_iPort = e_iPort;
		m_IPData.m_strServerIP = e_strIP;
		if(1)
		{
			f_ThreadWorkingFunction l_f_ThreadWorkingFunction = std::bind(&cGameNetwork::ClientListenDataThread, this, std::placeholders::_1);
			this->ThreadDetach(l_f_ThreadWorkingFunction);
			if (!this->m_pReconnectFunction && e_bCreateReconnectFunction)
			{
				m_pReconnectFunction = new sReconnectFunction(this, false);
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
		FMLog::LogWithFlag(UT::ComposeMsgByFormat("fetch IP :Port: %d\n", m_IPData.m_iPort), CORE_LOG_FLAG);
		/* Resolve the argument into an IPaddress type */
		if (SDLNet_ResolveHost(&m_IPData.m_IP, e_strIP, m_IPData.m_iPort) == -1)
		{
			FMLog::LogWithFlag("fetch IP failed\n", CORE_LOG_FLAG);
			this->m_eNetWorkStatus = eNWS_NO_INTERNET;
			return false;
		}
		UINT l_uiAddr = SDL_SwapBE32(m_IPData.m_IP.host);
		FMLog::LogWithFlag(UT::ComposeMsgByFormat("IP Address : %d.%d.%d.%d\n",
			l_uiAddr >> 24,(l_uiAddr >> 16) & 0xff,(l_uiAddr >> 8) & 0xff,l_uiAddr & 0xff), CORE_LOG_FLAG);
		m_IPData.m_strHost = SDLNet_ResolveIP(&m_IPData.m_IP);
		m_pSocket = SDLNet_TCP_Open(&m_IPData.m_IP);
		//if SDLNet_TCP_Open called server's SDLNet_TCP_Accept will be triggered
		if (!m_pSocket)
		{
			FMLog::LogWithFlag("socket open failed\n", CORE_LOG_FLAG);
			this->m_eNetWorkStatus = eNWS_CONNECTION_NOT_EXISTS;
			return false;
		}
		FMLog::LogWithFlag("socket open\n", CORE_LOG_FLAG);
		return true;
	}
	void cGameNetwork::CloseSocket()
	{
		FMLog::LogWithFlag("cGameNetwork::CloseSocket\n", CORE_LOG_FLAG);
		if (m_pSocket)
		{
			_TCPsocket*l_pSocket = m_pSocket;
			m_pSocket = nullptr;
			RemoveAllClient();
			DELETE_VECTOR(m_ReceivedDataVector);
			DELETE_VECTOR(m_ClientSocketVector);
			if (m_pAllSocketToListenClientMessage)
			{
				SDLNet_FreeSocketSet(m_pAllSocketToListenClientMessage);
				m_pAllSocketToListenClientMessage = 0;
			}
			SDLNet_TCP_Close(l_pSocket);
		}
	}
	void cGameNetwork::RemoveAllClient()
	{
		//cPP11MutexHolder hold(m_ClientSocketMutex, L"RemoveAllClient");
		cPP11MutexHolder hold(m_ClientSocketMutex);
		while (m_ClientSocketVector.size())
		{
			SDLNet_TCP_Close(m_ClientSocketVector[0]);
			m_ClientSocketVector.erase(m_ClientSocketVector.begin());
		}
	}
	void cGameNetwork::AddClient(_TCPsocket * e__pTCPsocket)
	{
		//cPP11MutexHolder l_PP11MutexHolder(m_ClientSocketMutex, L"AddClient");
		cPP11MutexHolder l_PP11MutexHolder(m_ClientSocketMutex);
#ifdef DEBUG
		int	l_iSize = (int)m_ClientSocketVector.size();
		for (int i = 0; i < l_iSize; ++i)
		{
			if (m_ClientSocketVector[i]->channel == e__pTCPsocket->channel)
			{
				UT::ErrorMsg(L"Client was exist", L"add new client error");
				return;
			}
		}
#endif
		m_ClientSocketVector.push_back(e__pTCPsocket);
	}	

	bool	cGameNetwork::RemoveClient(_TCPsocket*e__pTCPsocket)
	{
		cPP11MutexHolder l_PP11MutexHolder(m_ClientSocketMutex);
		return RemoveClientWhileClientLostConnection(e__pTCPsocket);
	}

	bool	cGameNetwork::RemoveClientWhileClientLostConnection(_TCPsocket*e__pTCPsocket)
	{
		if (!e__pTCPsocket)
			return false;
		/* close the old socket, even if it's dead... */
		int	l_iSize = (int)m_ClientSocketVector.size();
		for (int i = 0; i < l_iSize; ++i)
		{
			if (m_ClientSocketVector[i] == e__pTCPsocket)
			{
				if(m_ClientLostConnectionCallback)
					m_ClientLostConnectionCallback(e__pTCPsocket);
				SDLNet_TCP_Close(e__pTCPsocket);
				m_ClientSocketVector.erase(m_ClientSocketVector.begin() + i);
				return true;
			}
		}
		return false;
	}
	bool cGameNetwork::CreateSocksetToListenData()
	{
		//FMLog::LogWithFlag("create socket\n");
		if (m_pAllSocketToListenClientMessage)
			SDLNet_FreeSocketSet(m_pAllSocketToListenClientMessage);
		int	l_iNumClient = 0;
		//is it server?
		//cPP11MutexHolder hold(m_ClientSocketMutex, L"CreateSocksetToListenData");
		cPP11MutexHolder hold(m_ClientSocketMutex);
		if (m_pSocket->iServerFlag)
			l_iNumClient = (int)m_ClientSocketVector.size();
		//+1 fo server
		m_pAllSocketToListenClientMessage = SDLNet_AllocSocketSet(l_iNumClient + 1);
		if (!m_pAllSocketToListenClientMessage)
		{
			//FMLog::LogWithFlag("SDLNet_AllocSocketSet: %s\n", SDL_GetError());
			//exit(1); /*most of the time this is a major error, but do what you want. */
			return false;
		}

		SDLNet_TCP_AddSocket(m_pAllSocketToListenClientMessage, m_pSocket);
		for (int i = 0; i < l_iNumClient; i++)
			SDLNet_TCP_AddSocket(m_pAllSocketToListenClientMessage, m_ClientSocketVector[i]);
		return true;
	}

	void	cGameNetwork::ClientListenDataThread(float e_ElpaseTime)
	{
		if (!m_pSocket)
		{
			if (!OpenSocket(m_IPData.m_iPort, m_IPData.m_strServerIP.c_str()))
			{
				goto FAILED;
			}
		}
		m_eNetWorkStatus = eNWS_CONNECTED;
		assert(!m_pSocket->iServerFlag&&"cGameNetwork::ClientListenDataThread");
		if (m_pSocket)
		{
			if (!IsNetworkAlive())
			{
				FMLog::LogWithFlag("Network not aviable\n", CORE_LOG_FLAG);
				goto FAILED;
			}
			int l_iNumready = 0;
			if (!CreateSocksetToListenData())
			{
				FMLog::LogWithFlag("CreateSocksetToListenData failed\n", CORE_LOG_FLAG);
				goto FAILED;
			}
			//FMLog::LogWithFlag("listen\n");
			l_iNumready = SDLNet_CheckSockets(m_pAllSocketToListenClientMessage, (UINT)1000);
			if (l_iNumready == -1)
			{
				//FMLog::LogWithFlag("SDLNet_CheckSockets: %s\n",SDL_GetError());
				FMLog::LogWithFlag("SDLNet_CheckSockets failed\n", CORE_LOG_FLAG);
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
					FMLog::LogWithFlag("l_pPacket->ReceiveData failed\n", CORE_LOG_FLAG);
					bool	l_bConnectionFailed = false;
					delete l_pPacket;
					goto FAILED;
				}
				{
					//FMLog::LogWithFlag("recevied message\n", CORE_LOG_FLAG);
					//cPP11MutexHolder l_PP11MutexHolder(m_ReceivedDataMutex, L"recevied message");
					cPP11MutexHolder l_PP11MutexHolder(m_ReceivedDataMutex);
					this->m_ReceivedDataVector.push_back(l_pPacket);
				}
			}
		}
		return;
	FAILED:
		FMLog::LogWithFlag("cGameNetwork::ClientListenDataThread::disconnect\n", CORE_LOG_FLAG);
		CloseSocket();
		m_eNetWorkStatus = eNWS_LOST_CONNECTION;
		if (m_ConnectionLostCallbackFunction)
			m_ConnectionLostCallbackFunction();
		this->m_bLeaveThread = true;
	}

	void cGameNetwork::ServerListenDataThread(float e_ElpaseTime)
	{
		if (!m_pSocket)
		{
			if(!OpenSocket(m_IPData.m_iPort,nullptr))
			{
				goto FAILED;
			}
		}
		m_eNetWorkStatus = eNWS_CONNECTED;
		//if (!OpenSocket(e_iPort, e_strIP))
		assert(m_pSocket->iServerFlag&&"server update thread require a server flag");
		if(m_pSocket)
		{
			if (!IsNetworkAlive())
			{
				goto FAILED;
			}
			int l_iNumready = 0;
			if (!CreateSocksetToListenData())
			{
				goto FAILED;
			}
			//FMLog::LogWithFlag("listen\n");
			l_iNumready = SDLNet_CheckSockets(m_pAllSocketToListenClientMessage, (UINT)1000);
			if (l_iNumready == -1)
			{
				//FMLog::LogWithFlag("SDLNet_CheckSockets: %s\n",SDL_GetError());
				goto FAILED;
			}
			if (l_iNumready == 0)
				return;
			//new client?
			if (m_pSocket&&m_pSocket->iServerFlag && SDLNet_SocketReady(m_pSocket))
			{
				if (m_pSocket->iServerFlag)
				{
					TCPsocket l_pNewClient = SDLNet_TCP_Accept(m_pSocket);
					if (l_pNewClient)
					{
						AddClient(l_pNewClient);
						FMLog::LogWithFlag("new client\n", CORE_LOG_FLAG);
						//continue;
					}
				}
			}
			if (m_pSocket)
			{
				if (m_pSocket->iServerFlag)
				{
					//cPP11MutexHolder l_PP11MutexHolder(m_ClientSocketMutex,L"m_pSocket->iServerFlag");
					cPP11MutexHolder l_PP11MutexHolder(m_ClientSocketMutex);
					int	l_iNumClients = (int)m_ClientSocketVector.size();
					for (int i = 0; i < l_iNumClients; i++)
					{
						_TCPsocket*l_pClient = m_ClientSocketVector[i];
						if (SDLNet_SocketReady(l_pClient))
						{
							bool	l_bFraomTemplateData = false;
							sNetworkReceivedPacket*l_pPacket = new sNetworkReceivedPacket();
							int l_iResult = l_pPacket->ReceiveData(l_pClient);
							if(l_iResult > 0)
							{
								//FMLog::LogWithFlag("recevied message\n", CORE_LOG_FLAG);
								//cPP11MutexHolder l_PP11MutexHolder(m_ReceivedDataMutex,L"recevied message");
								cPP11MutexHolder l_PP11MutexHolder(m_ReceivedDataMutex);
								m_ReceivedDataVector.push_back(l_pPacket);
							
							}
							else
							{//wrong data ignore it
								delete l_pPacket;
								FMLog::LogWithFlag("client connection failed\n", CORE_LOG_FLAG);
								if (!RemoveClientWhileClientLostConnection(l_pClient))
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
		return;
	FAILED:
		FMLog::LogWithFlag("cGameNetwork::ServerListenDataThread::disconnect\n", CORE_LOG_FLAG);
		CloseSocket();
		m_eNetWorkStatus = eNWS_LOST_CONNECTION;
		if (m_ConnectionLostCallbackFunction)
			m_ConnectionLostCallbackFunction();
		this->m_bLeaveThread = true;
	}

	void cGameNetwork::sReconnectFunction::DoReconnectWhileLostConnection(float e_fElpaseTime)
	{
		if (m_pGameNetwork)
		{
			if (m_pGameNetwork->m_eNetWorkStatus != eNWS_CONNECTED &&
				m_pGameNetwork->m_eNetWorkStatus != eNWS_TRY_TO_CONNECT)
			{
				this->m_ReConnectTime.Update(e_fElpaseTime);
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