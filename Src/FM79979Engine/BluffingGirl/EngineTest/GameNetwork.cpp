#include "stdafx.h"
#include "GameNetwork.h"
//#include "GameApp.h"
//#include "PlayerData.h"

cMultiPlayerNetwork*g_pGameNetwork = 0;


void	ConnectedThread(size_t _workParameter, size_t _pUri)
{
	cGameApp::OutputDebugInfoString("tring to connect start\n");
	cMultiPlayerNetwork*l_pGameNetwork = (cMultiPlayerNetwork*)_workParameter;
	l_pGameNetwork->Connect();
	if(l_pGameNetwork->IsConnected())
	{
		cGameApp::OutputDebugInfoString("connected\n");
	}
	else
	{
		cGameApp::OutputDebugInfoString("connect failed \n");
	}
}

void	ConnectedThreadDone(size_t _workParameter, size_t _pUri)
{
	//cMultiPlayerNetwork*l_pGameNetwork = (cMultiPlayerNetwork*)_workParameter;
	//l_pGameNetwork->m_bDoConnection = true;
	//cGameApp::OutputDebugInfoString("tring to connect end\n");
}

cMultiPlayerNetwork::cMultiPlayerNetwork()
{
	g_pGameNetwork = this;
	m_bServer = false;
	m_bDoConnection = false;
	this->m_iPort = 2266;
	for( int i=0;i<4;++i )
		m_PlayerList[i] = 0;
}

cMultiPlayerNetwork::~cMultiPlayerNetwork()
{
}

int		cMultiPlayerNetwork::GetEmptyPlayerIndex()
{
	for( int i=1;i<4;++i )
	{
		if(!m_PlayerList[i])
			return i;
	}
	return -1;
}

int		cMultiPlayerNetwork::GetNumPlayerConnected()
{
	int	l_iNum = 1;
	for( int i=1;i<4;++i )
	{
		if(m_PlayerList[i])
			++l_iNum;
	}
	return l_iNum;
}

void	cMultiPlayerNetwork::Connect()
{
	cGameApp::OutputDebugInfoString("connection test\n");
	if(!this->m_pSocket)
	{
		if(m_bServer)
		{
			cGameApp::OutputDebugInfoString("I am server\n");
			if(!this->CreateAsServer(m_iPort))
			{
				cGameApp::OutputDebugInfoString("server create failed\n");
			}
		}
		else
		{
			cGameApp::OutputDebugInfoString("I am client\n");
			if(!this->CreateAsClient(m_iPort,m_strServerIP.c_str()))
			{
				cGameApp::OutputDebugInfoString("connect to server failed\n");
			}
		}
	}
}


void	cMultiPlayerNetwork::Init()
{
	cNetwork::Init();
	m_bDoConnection = false;
	for( int i=0;i<4;++i )
		m_PlayerList[i] = 0;
}


void	cMultiPlayerNetwork::MessageProcess(sReceivedPacket*e_pPacket)
{
	sGamePacket*l_pGamePacket = (sGamePacket*)e_pPacket->pData;
	sGamePacket l_GamePacket;
	sSendPacket	l_Packet;
	l_Packet.iSize = sizeof(sGamePacket);
	l_Packet.pData = (char*)&l_GamePacket;
	if(this->m_bServer)
	{
		//if(  l_pGamePacket->iMessageID == eGMI_TO_SERVER_START_ID )
		{

		}
	}
	else
	{
		if(  l_pGamePacket->GetMessageID() == eGMI_TO_CLIENT_START_ID )
		{
			cGameApp::OutputDebugInfoString("eGMI_TO_CLIENT_JP_SCORE\n");
		}
	}
	l_Packet.pData = 0;
}

void	cMultiPlayerNetwork::Update(float e_fElpaseTime)
{
	//if(!cEngineTestApp::m_sbOpenNetWork)
	//	return;
	cNetwork::Update(e_fElpaseTime);
	if(!this->IsConnected())
	{
		if(m_bDoConnection)
		{
			this->Destroy();
			this->Init();
			FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&ConnectedThread);
			FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&ConnectedThreadDone);
			this->m_pThreadPool->ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
			m_bDoConnection = false;
		}
	}
	//else
	//{
	//	cFUSynchronizedHold hold(&m_ReceivedData);
	//	while(m_ReceivedData.size())
	//	{//new data is coming!
	//		sReceivedPacket*l_pPacket = m_ReceivedData[0];
	//		MessageProcess(l_pPacket);
	//		delete l_pPacket;
	//		m_ReceivedData.erase(m_ReceivedData.begin());
	//	}
	//}
}

void	cMultiPlayerNetwork::Render()
{
	
}

bool	cMultiPlayerNetwork::SendDataToServer(sGamePacket*e_pGamePacket,int e_iSize)
{
	sSendPacket l_Packet;
	l_Packet.pData = (char*)e_pGamePacket;
	l_Packet.iSize = sizeof(int)+e_iSize;
	cNetwork::SendDataToServer(&l_Packet);
	l_Packet.pData = 0;
	return true;
}

bool	cMultiPlayerNetwork::SendDataToAllClient(sGamePacket*e_pGamePacket,int e_iSize)
{
	sSendPacket l_Packet;
	l_Packet.pData = (char*)e_pGamePacket;
	l_Packet.iSize = sizeof(int)+e_iSize;
	cNetwork::SendDataToAllClient(&l_Packet);
	l_Packet.pData = 0;
	return true;
}

bool	cMultiPlayerNetwork::SendDataToClient(_TCPsocket*e_TCPsocket,sGamePacket*e_pGamePacket)
{
	sSendPacket l_Packet;
	l_Packet.pData = (char*)e_pGamePacket;
	if( e_pGamePacket->GetMessageID() == eGMI_TO_CLIENT_START_ID )
	{
		l_Packet.iSize = sizeof(int)+sizeof(int)+sizeof(int);
		SendData(e_TCPsocket,&l_Packet);
	}
	else
	if(e_pGamePacket->GetMessageID() == eGMI_TO_CLIENT_PLAYER_COUNT )
	{
		l_Packet.iSize = sizeof(int)+sizeof(int);
		SendData(e_TCPsocket,&l_Packet);
	}
	l_Packet.pData = 0;
	return true;
}

void	cMultiPlayerNetwork::ConnectionCreated(bool e_bServer,const char*e_strServerIP)
{
	m_bServer = e_bServer;
	if( !e_bServer )
	{
		this->m_strServerIP = e_strServerIP;
	}
	m_bDoConnection = true;
}

int		cMultiPlayerNetwork::GetPlayerIndex(_TCPsocket*e_p_TCPsocket)
{
	for( int i=1;i<4;++i )
	{
		if(m_PlayerList[i] == e_p_TCPsocket)
			return i;
	}
	return -1;
}

void	cMultiPlayerNetwork::SetPlayerIDIndex(int e_iIndex,_TCPsocket*e_p_TCPsocket)
{
	m_PlayerList[e_iIndex] = e_p_TCPsocket;
}