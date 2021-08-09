#pragma once

#ifndef _M_CEE//manage code dont support cCPP11Thread

//if you want more client please define FD_SETSIZE or its default is 64
//#ifndef FD_SETSIZE
//#define FD_SETSIZE      64
//#endif /* FD_SETSIZE */

#include "../Synchronization/CPP11Thread.h"
#include "NetworkData.h"
#include "../Common/Utility.h"

namespace FATMING_CORE
{
	class	cGameNetwork:public FATMING_CORE::cCPP11Thread,public NamedTypedObject
	{
#ifdef WASM
		int						m_iBridgeSocket = -1;
#endif
		std::mutex				m_SendDataMutex;
		bool					ThreadProcessWaitSendDataVector();
		std::map<SDLNetSocket, std::vector<sNetworkSendPacket*> > m_WaitToSendPacketVector;
	protected:
		struct sReconnectFunction
		{
			UT::sTimeCounter	m_ReConnectTime;
			bool				m_bServeFlag;
			cGameNetwork*		m_pGameNetwork;
			sReconnectFunction(cGameNetwork*e_pGameNetwork,bool e_bServeFlag,float e_fReconnectTime = 5.f)
			{
				m_pGameNetwork = e_pGameNetwork;
				m_ReConnectTime.SetTargetTime(e_fReconnectTime);
				m_bServeFlag = e_bServeFlag;
			}
			void				DoReconnectWhileLostConnection(float e_fElpaseTime);
		};
		friend struct sReconnectFunction;
		sReconnectFunction*					m_pReconnectFunction;
		virtual void						AddClient(SDLNetSocket e__pTCPsocket);
		virtual void						ServerListenDataThread(float e_ElpaseTime);
		virtual void						ClientListenDataThread(float e_ElpaseTime);
		virtual bool						InternalSendData(SDLNetSocket e_pTCPsocket, sNetworkSendPacket* e_pPacket);
		//if e_strIP is nullptr it's server
		bool								OpenSocket(int e_iPort, const char*e_strIP);
		void								CloseSocket();
		virtual void						RemoveAllClient();
		//no mutex inside of this function,because mutex called before this function
		virtual bool						RemoveClientWhileClientLostConnection(SDLNetSocket e__pTCPsocket);
		/* create a socket set that has the server socket and all the client sockets */
		_SDLNet_SocketSet*					m_pAllSocketToListenClientMessage;
		bool								CreateSocksetToListenData();
		std::function<void()>				m_ConnectionLostCallbackFunction;
		//only for server,it under m_ClientSocketMutex
		std::function<void(SDLNetSocket)>	m_ClientLostConnectionCallback;
	protected:
		sIPData								m_IPData;
		//own socket for server or client
		SDLNetSocket						m_pSocket;
		std::vector<SDLNetSocket>			m_ClientSocketVector;
		std::vector<sNetworkReceivedPacket*>m_ReceivedDataVector;
		std::mutex							m_ReceivedDataMutex;
		std::mutex							m_ClientSocketMutex;
		eNetWorkStatus						m_eNetWorkStatus;
		virtual bool						RemoveClient(SDLNetSocket e__pTCPsocket);
		//
		bool								m_bDoDisconnect;
	public:
		cGameNetwork();
		virtual ~cGameNetwork();
		DEFINE_TYPE_INFO();
		void								SetConnectionLostCallbackFunction(std::function<void()> e_Function);
		void								SetClientLostConnectionCallback(std::function<void(SDLNetSocket)> e_Function);
		eNetWorkStatus						GetNetWorkStatus() { return m_eNetWorkStatus; }
		std::vector<sNetworkReceivedPacket*>GetReceivedDataPleaseDeleteAfterUseIt();
		//below 2(SendData,SendDataToAllClient) API will add 4 byte(int) before the data,if you don't want add a int before the packet please override this
		virtual bool						SendData(SDLNetSocket e_pTCPsocket, sNetworkSendPacket*e_pPacket, bool e_bSnedByNetworkThread = true);
		virtual bool						SendDataToAllClient(sNetworkSendPacket*e_pPacket, bool e_bSnedByNetworkThread = true);
		bool								SendDataToClient(SDLNetSocket e_pTCPsocket, char* e_pData, int e_iDataLength, bool e_bSnedByNetworkThread = true);
		bool								SendDataToAllClient(char* e_pData, int e_iDataLength, bool e_bSnedByNetworkThread = true);
		template<class TYPE>bool			SendDataToClient(SDLNetSocket e_pTCPsocket,TYPE*e_pData,bool e_bSnedByNetworkThread = true);
		template<class TYPE>bool			SendDataToAllClient(TYPE*e_pData, bool e_bSnedByNetworkThread = true);
		template<class TYPE>bool			SendDataToServer(TYPE * e_pData, bool e_bSnedByNetworkThread = true);
		bool								SendDataToServer(sNetworkSendPacket*e_pPacket, bool e_bSnedByNetworkThread = true);

		bool								CreateAsServer(int e_iPort,bool e_bCreateReconnectFunction, float e_fReconnectionTimeGap = 1.f);
		bool								CreateAsClient(int e_iPort, const char*e_strIP, bool e_bCreateReconnectFunction,float e_fReconnectionTimeGap = 1.f);

		size_t								ClientCount() { return m_ClientSocketVector.size(); }
		sIPData*							GetIPData(){ return &m_IPData; }
		bool								GetIP();
		virtual	void						Init();
		virtual	void						Update(float e_fElpaseTime);
		//it's possible locked by thread
		virtual	void						Destroy();
		//
		bool								IsDoDisconnect() { return m_bDoDisconnect; }
		void								SetDoDisconnect(bool e_bDisConnect, bool e_bDeleteReConnect);
	};
	template<class TYPE>
	inline bool cGameNetwork::SendDataToClient(SDLNetSocket e_pTCPsocket, TYPE*e_pData, bool e_bSnedByNetworkThread)
	{
		bool l_bSendResult = false;
		sNetworkSendPacket l_NetworkSendPacket;
		l_NetworkSendPacket.iSize = sizeof(TYPE);
		l_NetworkSendPacket.pData = (char*)e_pData;
		l_bSendResult = SendData(e_pTCPsocket,&l_NetworkSendPacket, e_bSnedByNetworkThread);
		l_NetworkSendPacket.pData = nullptr;
		return l_bSendResult;
	}
	template<class TYPE>
	inline bool	cGameNetwork::SendDataToAllClient(TYPE*e_pData, bool e_bSnedByNetworkThread)
	{
		bool l_bSendDataResult = false;
		sNetworkSendPacket l_NetworkSendPacket;
		l_NetworkSendPacket.iSize = sizeof(TYPE);
		l_NetworkSendPacket.pData = (char*)e_pData;
		l_bSendDataResult = SendDataToAllClient(&l_NetworkSendPacket, e_bSnedByNetworkThread);
		l_NetworkSendPacket.pData = nullptr;
		return l_bSendDataResult;
	}
	template<class TYPE>
	inline bool cGameNetwork::SendDataToServer(TYPE * e_pData, bool e_bSnedByNetworkThread)
	{
		bool l_bSendResult = false;
		sNetworkSendPacket l_NetworkSendPacket;
		l_NetworkSendPacket.iSize = sizeof(TYPE);
		l_NetworkSendPacket.pData = (char*)e_pData;
		l_bSendResult = SendDataToServer(&l_NetworkSendPacket, e_bSnedByNetworkThread);
		l_NetworkSendPacket.pData = nullptr;
		return l_bSendResult;
	}

#define	NETWORK_LAZY_SEND_TO_SERVER(NETWORK_SINGLTON,DATA)													\
{																											\
	sNetworkSendPacket l_TempNetworkSendPacket123;															\
	l_TempNetworkSendPacket123.iSize = DATA.iSize;															\
	l_TempNetworkSendPacket123.pData = (char*)&DATA;														\
	bool l_bSendResult123 = NETWORK_SINGLTON::GetInstance()->SendDataToServer(&l_TempNetworkSendPacket123);	\
	l_TempNetworkSendPacket123.pData = nullptr;																\
}

#define	NETWORK_LAZY_SEND_TO_CLIENT(NETWORK_SINGLTON,SOCKET,DATA)											  \
{																											  \
	sNetworkSendPacket l_TempNetworkSendPacket123;															  \
	l_TempNetworkSendPacket123.iSize = DATA.iSize;															  \
	l_TempNetworkSendPacket123.pData = (char*)&DATA;														  \
	bool l_bSendResult123 = NETWORK_SINGLTON::GetInstance()->SendData(SOCKET, &l_TempNetworkSendPacket123);	  \
	l_TempNetworkSendPacket123.pData = nullptr;																  \
}

#define	NETWORK_LAZY_SEND_TO_ALL_CLIENT(NETWORK_SINGLTON,SOCKET,DATA)											\
{																												\
	sNetworkSendPacket l_TempNetworkSendPacket123;																\
	l_TempNetworkSendPacket123.iSize = DATA.iSize;																\
	l_TempNetworkSendPacket123.pData = (char*)&DATA;															\
	bool l_bSendResult123 = NETWORK_SINGLTON::GetInstance()->SendDataToAllClient(&l_TempNetworkSendPacket123);	\
	l_TempNetworkSendPacket123.pData = nullptr;																	\
}

#define	LAZY_SEND_NETWORK_MESSAGE(SOCKET,DATA)																  \
{																											  \
	sNetworkSendPacket l_TempNetworkSendPacket123;															  \
	l_TempNetworkSendPacket123.iSize = DATA.iSize;															  \
	l_TempNetworkSendPacket123.pData = (char*)&DATA;														  \
	bool l_bSendResult123 = this->SendData(SOCKET, &l_TempNetworkSendPacket123);							  \
	l_TempNetworkSendPacket123.pData = nullptr;																  \
}

//end namespace
}


//_M_CEE
#endif
//
//enum eGameNetworkMessageID
//{
//	eGNMI_C2S_LOGIN = 0,
//	eGNMI_S2C_LOGIN_RESULT,
//	eGNMI_MAX,
//};
//class	cClientNetwork :public FATMING_CORE::cGameNetwork,public NamedTypedObject, public cMessageSender
//{
//	void								LostConnection();
//	bool								CurrentBonsNetworkEventProcess(FATMING_CORE::sNetworkReceivedPacket*e_pData);
//	cClientNetwork();
//	~cClientNetwork();
//public:
//	DEFINE_TYPE_INFO();
//	virtual	void						Init()override;
//	virtual	void						Update(float e_fElpaseTime)override;
//	virtual	void						Destroy()override;
//};
//
//TYPDE_DEFINE_MARCO(cClientNetwork)
//cClientNetwork::cClientNetwork()
//{
//	REG_NET_MESSAGE_FUNCTION(eGNMI_S2C_LOGIN_RESULT, &cClientNetwork::CurrentBonsNetworkEventProcess);
//	SetConnectionLostCallbackFunction(std::bind(&cClientNetwork::LostConnection, this));
//}
//
//cClientNetwork::~cClientNetwork()
//{
//	SetConnectionLostCallbackFunction(nullptr);
//}
//
//void cClientNetwork::LostConnection()
//{
//}
//bool cClientNetwork::LoginNetworkEventProcess(FATMING_CORE::sNetworkReceivedPacket*e_pData)
//{
//	int l_iSize = e_pData->iSize;
//	return true;
//}
////test
//UT::sTimeCounter g_cTC;
//void cClientNetwork::Init()
//{
//	cGameNetwork::Init();
//	this->CreateAsClient(7999, "10.168.1.120", true);
//	//test
//	g_cTC.SetTargetTime(3.f);
//}
//
//void cClientNetwork::Update(float e_fElpaseTime)
//{
//	cGameNetwork::Update(e_fElpaseTime);
//	std::vector<sNetworkReceivedPacket*>l_DataVector = GetReceivedDataPleaseDeleteAfterUseIt();
//	size_t l_uiSize = l_DataVector.size();
//	for (sNetworkReceivedPacket*l_pData : l_DataVector)
//	{
//		unsigned int l_uiID = *(unsigned int*)l_pData->pData;
//		cGameApp::m_spMessageSenderManager->NetworkMessageShot(l_uiID, l_pData);
//	}
//	DELETE_VECTOR(l_DataVector);
//	//test code
//	if (this->m_pSocket)
//	{
//		g_cTC.Update(e_fElpaseTime);
//		if (g_cTC.bTragetTimrReached)
//		{
//			g_cTC.Start();
//			sNetworkSendPacket l_NetworkSendPacket;
//			struct sTest
//			{
//				int a = 5;
//			};
//			sTest l_sTest;
//			l_NetworkSendPacket.iSize = sizeof(sTest);
//			l_NetworkSendPacket.pData = (char*)&l_sTest;
//			this->SendDataToServer(&l_NetworkSendPacket);
//			l_NetworkSendPacket.pData = nullptr;
//		}
//	}
//}
//
//void cClientNetwork::Destroy()
//{
//	cGameNetwork::Destroy();
//}