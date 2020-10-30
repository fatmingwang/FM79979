#pragma once
#include <functional>
#include <map>
#include <vector>
#include <string.h>
namespace FATMING_CORE
{
	struct sReceivedPacket;
	struct sNetworkReceivedPacket;

	#define GAME_PAUSE_EVENT_ID	-123456789
	#define WAIT_EMIT_EVENT_DATA_SIZE	4096
	typedef std::function<bool(FATMING_CORE::sNetworkReceivedPacket*)>		NetworkMessageFunction;
	typedef std::function<bool(void*)>										EventFunction;


	//REG_NET_MESSAGE_FUNCTION(MSG_GS2CL_CHANGE_ROUTE_RESPONSE,&cTradeRouteDataFromServer::Received_MSG_GS2CL_CHANGE_ROUTE_RESPONSE);
	//#define REG_NET_MESSAGE_FUNCTION(proto,Function)RegNetworkMessageFunction<proto>(_##proto,std::bind(Function,this,std::placeholders::_1));
	//#define REG_NET_MESSAGE_FUNCTION(MESSAGE_ID,Function)
	#define REG_NET_MESSAGE_FUNCTION(MESSAGE_ID,Function)RegNetworkMessageFunction(MESSAGE_ID,std::bind(Function,this,std::placeholders::_1));
	//REG_EVENT(eOPEM_CLOSE_TRADE_ROUTE_UI_LAYOUT,&cRegionMapChange::OnCloseLayout);
	#define	REG_EVENT(EventID,Function)RegEvent(EventID,std::bind(Function,this,std::placeholders::_1));




	class cMessageSenderManager;

	class cMessageSender
	{
		friend class cMessageSenderManager;
		//network
		std::map<unsigned int, NetworkMessageFunction>		m_NetworkMessageFunctionMap;
		//event
		std::map<unsigned int, EventFunction>				m_EventFunctionMap;
		//
		cMessageSenderManager*m_pParent;
		//
		void												SetParent(cMessageSenderManager*e_pParent = nullptr);
	public:
		cMessageSender(cMessageSenderManager*e_pParent = nullptr);
		virtual ~cMessageSender();
		//please keep e_pData,or it will be a wild pointer.
		bool					RegEvent(unsigned int e_usID, EventFunction e_MessageFunction);
		//see REG_NET_MESSAGE_FUNCTION
		template <class T>bool	RegNetworkMessageFunction(unsigned int e_usID, NetworkMessageFunction e_MessageFunction);
		bool					RegNetworkMessageFunction(unsigned int e_usID, NetworkMessageFunction e_MessageFunction);
		//
		bool					UnregNetworkMessageFunction(unsigned int e_usID);
		bool					UnregEvent(unsigned int e_usID);
		void					UnregistorAll();
		//do not use this one!.
		cMessageSenderManager*	GetParent() { return m_pParent; }
	};


	class cMessageSenderManager
	{
		friend class cMessageSender;
		//
		struct sNetworkMessageFunctionAndObjectID
		{
			size_t uiAddress;
			NetworkMessageFunction			f_NetworkMessageFunction;
			sNetworkMessageFunctionAndObjectID();
			~sNetworkMessageFunctionAndObjectID();
		};
		struct sEventFunctionAndType
		{
			size_t			uiAddress;
			EventFunction	f_EventFunction;
			//void*			pData;
		};

		struct sWaitEmitEvent
		{
			unsigned int	usID;
			void*			pData;
			char			cData[WAIT_EMIT_EVENT_DATA_SIZE];
			sWaitEmitEvent() { pData = nullptr; memset(cData, sizeof(char), WAIT_EMIT_EVENT_DATA_SIZE); }
			~sWaitEmitEvent() {}
		};
		void				AddMessageSender(size_t e_uiAddress,cMessageSender*e_pMessageSender);
		void				RemoveMessageSender(size_t e_uiAddress);
	private:
		//key is address
		std::map<size_t,cMessageSender*>	m_AllMessageSenderMap;
		//
		std::map< unsigned int, std::vector<sNetworkMessageFunctionAndObjectID*> >	m_NetworkMessageFunctionAndObjectIDMap;
		//fix memory leak problem
		std::map< unsigned int, std::vector<sEventFunctionAndType*> >				m_EventFunctionAndTypeMap;
		//
		std::vector<sWaitEmitEvent*>												m_WaitForEmitEvent;
	public:
		cMessageSenderManager();
		~cMessageSenderManager();
		bool	NetworkMessageShot(unsigned int e_usID,sNetworkReceivedPacket*e_pNetworkReceivedPacket);
		bool	EventMessageShot(unsigned int e_usID, void*e_pData);
		//ensure size is small than WAIT_EMIT_EVENT_DATA_SIZE
		bool	EventMessageShot(unsigned int e_usID, char*e_pData, int e_iSize);
		//ensure not call recursively,event call evnet infinty
		bool	EventMessageShotImmediately(unsigned int e_usID, void*e_pData);
		//for emit event for frame
		void	Update(float e_fElpaseTime);
		void	ClearEvent();
	};


	template <class T>bool	cMessageSender::RegNetworkMessageFunction(unsigned int e_usID, NetworkMessageFunction e_MessageFunction)
	{
		SetParent();
		if (this->m_NetworkMessageFunctionMap.find(e_usID) != m_NetworkMessageFunctionMap.end())
			return false;
		m_NetworkMessageFunctionMap[e_usID] = e_MessageFunction;

		cMessageSenderManager::sNetworkMessageFunctionAndObjectID	l_sMessageFunctionAndType;
		l_sMessageFunctionAndType.f_NetworkMessageFunction = e_MessageFunction;
		l_sMessageFunctionAndType.uiAddress = (size_t)this;
		m_NetworkMessageFunctionMap[e_usID] = e_MessageFunction;
		if (m_pParent)
		{
			std::vector<cMessageSenderManager::sNetworkMessageFunctionAndObjectID*>*l_pVector = nullptr;
			auto l_Iterator = m_pParent->m_NetworkMessageFunctionAndObjectIDMap.find(e_usID);
			if (l_Iterator == m_pParent->m_NetworkMessageFunctionAndObjectIDMap.end())
			{
				std::vector<cMessageSenderManager::sNetworkMessageFunctionAndObjectID*>	l_Temp;
				m_pParent->m_NetworkMessageFunctionAndObjectIDMap[e_usID] = l_Temp;
			}
			l_pVector = &m_pParent->m_NetworkMessageFunctionAndObjectIDMap[e_usID];
			cMessageSenderManager::sNetworkMessageFunctionAndObjectID*l_pEventFunction = new cMessageSenderManager::sNetworkMessageFunctionAndObjectID;
			l_pEventFunction->f_NetworkMessageFunction = e_MessageFunction;
			l_pEventFunction->uiAddress = (size_t)this;
			l_pVector->push_back(l_pEventFunction);
			return true;
		}
		return false;
	}
}

//enum eEventMessage
//{
//	eREGISTER_IN = 300,
//	eREGISTER_OUT,
//	eREGISTER_MAX,
//};
//
//class cRegisterManager : public cMessageSender
//{
//	bool					ExchangeOutEventFuntion(void *e_pData)
//	{
//		int l_iValue = *(int*)e_pData;
//	}
//	bool					ExchangeInWithPointer(void *e_pData)
//	{
//		sSomething l_pSomething = (sSomething*)e_pData;
//	}
//};
//cRegisterManager::cRegisterManager()
//{
//	REG_EVENT(eREGISTER_OUT, &cRegisterManager::ExchangeOutEventFuntion);
//	REG_EVENT(eREGISTER_IN, &cRegisterManager::ExchangeInWithPointer);
//}
//
////send message with specific data
//int64 l_i64OutCount = 50;
//cGameApp::m_spMessageSenderManager->EventMessageShot(eREGISTER_OUT, (char*)&l_i64OutCount, sizeof(int64));
//struct sSomething;
//void*l_pPointer = (void*)sSomething;
//cGameApp::m_spMessageSenderManager->EventMessageShot(eREGISTER_IN, l_pPointer);