#pragma once
//http://headerphile.com/sdl2/sdl2-part-12-multiplayer/

#define	UDP_PACKET_MAX_SIZE	65500
#include "GameNetWork.h"
#include "../Common/Template/TemplateClass.h"
#include "../Common/EventSender/MessageSender.h"

namespace FATMING_CORE
{
	class cUDPNetwork :public FATMING_CORE::cCPP11Thread,public NamedTypedObject
	{
		sIPData					m_IPData;
		_UDPsocket*				m_pSocket;
		UDPpacket*				m_pInPacket;
		UDPpacket*				m_pSizeHeaderPacket;//for check real data packet
		UDPpacket*				m_pOutPacket;
		eNetWorkStatus			m_eNetWorkStatus;
		bool					m_bDoDisconnect;
		std::mutex				m_ReceivedDataMutex;
		//true for server false for client
		bool					m_bServer;
		bool					OpenSocket(const char*e_strIP, int e_iPort);
		virtual		bool		CreatePacket();
		void					DeletePacket();
		void					CloseSocket();
		virtual		void		ServerListenDataThread(float e_fElpaseTime);
		virtual		void		ClientListenDataThread(float e_fElpaseTime);
		std::function<void()>	m_ConnectionLostCallbackFunction;
		//
		std::vector<sNetworkReceivedPacket*>	m_ReceivedDataVector;
		std::map<int, float>					m_IPAndLastReceivedTimeMap;
	public:
		cUDPNetwork();
		~cUDPNetwork();
		DEFINE_TYPE_INFO();
		virtual bool						CreateAsClient(const char*e_strServerIP, int e_iPort);
		virtual bool						CreateAsServer(int e_iPort);
		virtual void						Update(float e_fElpaseTime) = 0;
		std::vector<sNetworkReceivedPacket*>GetReceivedDataPleaseDeleteAfterUseIt();
		eNetWorkStatus						GetNetWorkStatus() { return m_eNetWorkStatus; }
		virtual bool						SendDataToClient(IPaddress&e_IPaddress, sNetworkSendPacket*e_pPacket);
		template<class TYPE>bool			SendDataToClient(IPaddress&e_IPaddress, TYPE*e_pData)
		{
			bool l_bSendResult = false;
			sNetworkSendPacket l_NetworkSendPacket;
			l_NetworkSendPacket.iSize = sizeof(TYPE);
			l_NetworkSendPacket.pData = (char*)e_pData;
			l_bSendResult = SendDataToClient(e_IPaddress, &l_NetworkSendPacket);
			l_NetworkSendPacket.pData = nullptr;
			return l_bSendResult;
		}
		template<class TYPE>bool			SendDataToServer(TYPE * e_pData)
		{
			bool l_bSendResult = false;
			sNetworkSendPacket l_NetworkSendPacket;
			l_NetworkSendPacket.iSize = sizeof(TYPE);
			l_NetworkSendPacket.pData = (char*)e_pData;
			l_bSendResult = SendDataToServer(&l_NetworkSendPacket);
			l_NetworkSendPacket.pData = nullptr;
			return l_bSendResult;
		}
		bool				SendDataToServer(sNetworkSendPacket*e_pPacket);
	};

	//the UDP need quickly to send data so need another cMessageSenderManager for send message
	class cSimpleUDPClientNetwork :public cUDPNetwork, public cSingltonTemplate<cSimpleUDPClientNetwork>,
		public cMessageSenderManager
	{
		cSimpleUDPClientNetwork(){}
		virtual ~cSimpleUDPClientNetwork(){}
		std::function<void(cSimpleUDPClientNetwork*e_pSimpleUDPClientNetwork,float e_fElpaseTime)>	m_UpdateFunction;
	public:
		DEFINE_TYPE_INFO();
		SINGLETON_BASIC_FUNCTION(cSimpleUDPClientNetwork);
		virtual void	Update(float e_fElpaseTIme)override
		{
			if (m_UpdateFunction)
			{
				m_UpdateFunction(this, e_fElpaseTIme);
			}
		}
		void			SetUpdateFunction(std::function<void(cSimpleUDPClientNetwork*e_pSimpleUDPClientNetwork, float e_fElpaseTime)>e_UpdateFunction)
		{
			m_UpdateFunction = e_UpdateFunction;
			//auto l_Functio = [](cSimpleUDPClientNetwork*e_pSimpleUDPClientNetwork, float e_fElpaseTime)
			//{
				//auto l_pReceivedDataVector = e_pSimpleUDPClientNetwork->GetReceivedDataPleaseDeleteAfterUseIt();
				//for (auto l_pData : l_pReceivedDataVector)
				//{
				//	sBaseNetworkMessage*l_pBaseNetworkMessage = (sBaseNetworkMessage*)l_pData->pData;
				//	NetworkMessageShot(l_pBaseNetworkMessage->iMessage, l_pData);
				//}
				//DELETE_VECTOR(l_pReceivedDataVector);
			//}
		}
	};
//end namespace FATMING_CORE
}