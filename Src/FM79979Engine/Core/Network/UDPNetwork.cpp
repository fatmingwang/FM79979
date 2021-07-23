//#include "stdafx.h"
#include "../Common/Log/FMLog.h"
#include "UDPNetwork.h"

namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cSimpleUDPClientNetwork)
	TYPDE_DEFINE_MARCO(cUDPNetwork)

	cUDPNetwork::cUDPNetwork() :cCPP11Thread(this)
	{
		m_bDoDisconnect = false;
		m_pInPacket = nullptr;
		m_pOutPacket = nullptr;
		m_pSocket = nullptr;
		if (SDLNet_Init() == -1)
		{
			FMLog::Log("Error SDLNet_Init", true);
		}
	}


	cUDPNetwork::~cUDPNetwork()
	{
		this->m_bDoDisconnect = true;
		this->CloseThreadAndWaitUntilFinish();
		CloseSocket();
		DeletePacket();
		SDLNet_Quit();
	}

	bool cUDPNetwork::OpenSocket(const char * e_strIP, int e_iPort)
	{
		if (SDLNet_ResolveHost(&m_IPData.m_IP, e_strIP, e_iPort) == -1)
		{
			FMLog::Log("UDP CreateAsClient SDLNet_ResolveHost failed!.", true);
			return false;
		}
		m_pSocket = SDLNet_UDP_Open(m_bServer ? e_iPort : 0);
		if (!m_pSocket)
		{
			FMLog::Log("UDP CreateAsClient SDLNet_UDP_Open failed!.", true);
			return false;
		}
		if (!CreatePacket())
			return false;
		/* bind server address to channel 0 */
		if (!m_bServer)
		{
			if (SDLNet_UDP_Bind(m_pSocket, 0, &m_IPData.m_IP) == -1)
			{
				FMLog::Log("UDP CreateAsClient SDLNet_UDP_Bind failed!.", true);
				return false;
			}
		}
		return true;

	}

	bool cUDPNetwork::CreatePacket()
	{
		if (m_pInPacket)
		{
			SDLNet_FreePacket(m_pInPacket);
			m_pInPacket = nullptr;
		}
		if (m_pOutPacket)
		{
			SDLNet_FreePacket(m_pOutPacket);
			m_pOutPacket = nullptr;
		}
		m_pOutPacket = SDLNet_AllocPacket(UDP_PACKET_MAX_SIZE);
		if (!m_pOutPacket)
		{
			FMLog::Log("UDP SDLNet_AllocPacket of out failed!.", true);
			return false;
		}
		m_pInPacket = SDLNet_AllocPacket(UDP_PACKET_MAX_SIZE);
		if (!m_pInPacket)
		{
			FMLog::Log("UDP SDLNet_AllocPacket of in failed!.", true);
			return false;
		}
		return true;
	}

	void cUDPNetwork::CloseSocket()
	{
		if (m_pSocket)
			SDLNet_UDP_Close(m_pSocket);
		m_pSocket = nullptr;
	}

	void cUDPNetwork::ServerListenDataThread(float e_fElpaseTime)
	{
		Sleep(1);
		eNetWorkStatus l_eNetWorkStatus = m_eNetWorkStatus;
		if (!m_pSocket)
		{
			if (!OpenSocket(nullptr, m_IPData.m_iPort))
			{
				//goto FAILED;
				m_eNetWorkStatus = eNWS_CONNECTION_NOT_EXISTS;
			}
		}
		if (m_pSocket)
		{
			m_eNetWorkStatus = eNWS_CONNECTED;
			if (!IsNetworkAlive())
			{
				m_eNetWorkStatus = eNWS_NO_INTERNET;
				//goto FAILED;
			}
			bool l_bReceivedData = true;
			while (l_bReceivedData)
			{
				auto l_iReceivedBytes = SDLNet_UDP_Recv(this->m_pSocket, this->m_pInPacket);
				if (l_iReceivedBytes > 0)
				{
					sNetworkReceivedPacket*l_pPacket = new sNetworkReceivedPacket();					
					l_pPacket->UDPReceiveDataWithoutHeaderSize(GetSDLNetSocket(m_pInPacket->address), m_pInPacket->len, (char*)m_pInPacket->data);
					{
						cPP11MutexHolder l_PP11MutexHolder(m_ReceivedDataMutex);
						m_ReceivedDataVector.push_back(l_pPacket);
					}
				}
				else
				{
					//wait next round
					Sleep(1);
					return;
					//goto FAILED;
				}
			}
		}
		if (!m_bDoDisconnect)
			return;
		FMLog::LogWithFlag("cUDPNetwork::ServerListenDataThread::disconnect", CORE_LOG_FLAG);
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

	void cUDPNetwork::ClientListenDataThread(float e_fElpaseTime)
	{
		Sleep(1);
		eNetWorkStatus l_eNetWorkStatus = m_eNetWorkStatus;
		if (!m_pSocket)
		{
			if (!OpenSocket(m_IPData.m_strServerIP.c_str(), m_IPData.m_iPort))
			{
				//goto FAILED;
				m_eNetWorkStatus = eNWS_CONNECTION_NOT_EXISTS;
			}
			if (m_pOutPacket && m_pSocket)
				this->m_pOutPacket->address = this->m_pSocket->address;
		}
		if (m_pSocket)
		{
			m_eNetWorkStatus = eNWS_CONNECTED;
			if (!IsNetworkAlive())
			{
				m_eNetWorkStatus = eNWS_NO_INTERNET;
				//goto FAILED;
			}
			bool l_bReceivedData = true;
			while (l_bReceivedData)
			{
				auto l_iReceivedBytes = SDLNet_UDP_Recv(this->m_pSocket, this->m_pInPacket);
				if (l_iReceivedBytes > 0)
				{
					sNetworkReceivedPacket* l_pPacket = new sNetworkReceivedPacket();	
					l_pPacket->UDPReceiveDataWithoutHeaderSize(GetSDLNetSocket(m_pInPacket->address), m_pInPacket->len, (char*)m_pInPacket->data);
					{
						cPP11MutexHolder l_PP11MutexHolder(m_ReceivedDataMutex);
						m_ReceivedDataVector.push_back(l_pPacket);
					}
				}
				else
				{
					//wait next round
					Sleep(1);
					return;
					//goto FAILED;
				}
			}
		}
		if (!m_bDoDisconnect)
			return;
		FMLog::LogWithFlag("cUDPNetwork::ServerListenDataThread::disconnect", CORE_LOG_FLAG);
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

	bool	cUDPNetwork::CreateAsClient(const char*e_strServerIP, int e_iPort)
	{
		m_bDoDisconnect = false;
		m_bServer = false;
		this->m_IPData.m_strServerIP = e_strServerIP;
		this->m_IPData.m_iPort = e_iPort;
		f_ThreadWorkingFunction l_f_ThreadWorkingFunction = std::bind(&cUDPNetwork::ClientListenDataThread, this, std::placeholders::_1);
		this->ThreadDetach(l_f_ThreadWorkingFunction,"cUDPNetwork::CreateAsClient");
		return true;
	}

	bool	cUDPNetwork::CreateAsServer(int e_iPort)
	{
		m_bDoDisconnect = false;
		m_bServer = true;
		this->m_IPData.m_iPort = e_iPort;
		f_ThreadWorkingFunction l_f_ThreadWorkingFunction = std::bind(&cUDPNetwork::ServerListenDataThread, this, std::placeholders::_1);
		this->ThreadDetach(l_f_ThreadWorkingFunction, "cUDPNetwork::CreateAsServer");
		return true;
	}

	std::vector<sNetworkReceivedPacket*> cUDPNetwork::GetReceivedDataPleaseDeleteAfterUseIt()
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

	bool cUDPNetwork::SendDataToClient(IPaddress & e_IPaddress, sNetworkSendPacket * e_pPacket)
	{
		if (this->m_pOutPacket)
		{
			m_pOutPacket->address = e_IPaddress;
			memcpy(m_pOutPacket->data, e_pPacket->pData, e_pPacket->iSize);
			m_pOutPacket->len = e_pPacket->iSize;
			if (!SDLNet_UDP_Send(this->m_pSocket, -1, m_pOutPacket))
			{
				return false;
			}
			return true;
		}
		return false;
	}

	bool	cUDPNetwork::SendDataToServer(sNetworkSendPacket*e_pPacket)
	{
		if (!m_pOutPacket || e_pPacket->iSize >= UDP_PACKET_MAX_SIZE)
			return false;
		memcpy(m_pOutPacket->data, e_pPacket->pData, e_pPacket->iSize);
		m_pOutPacket->len = e_pPacket->iSize;
		/* server address channel is 0 */
		if (!SDLNet_UDP_Send(m_pSocket, 0, m_pOutPacket))
		{
			return false;
		}
		return true;
	}

	void cUDPNetwork::DeletePacket()
	{
		if (m_pInPacket)
		{
			SDLNet_FreePacket(m_pInPacket);
			m_pInPacket = nullptr;
		}
		if (m_pOutPacket)
		{
			SDLNet_FreePacket(m_pOutPacket);
			m_pOutPacket = nullptr;
		}
		if (m_pInPacket)
		{
			SDLNet_FreePacket(m_pInPacket);
			m_pInPacket = nullptr;
		}
	}
}