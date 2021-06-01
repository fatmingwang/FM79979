#pragma once
#include <functional>
#include <map>
#include <vector>
#include <string.h>
#include <mutex>
#include "../NamedTypedObject.h"
//May/14/2021 add delegate function type.
namespace FATMING_CORE
{
	struct sReceivedPacket;
	struct sNetworkReceivedPacket;

	#define GAME_PAUSE_EVENT_ID	-123456789
	#define WAIT_EMIT_EVENT_DATA_SIZE	4096
	//
	typedef std::function<bool(FATMING_CORE::sNetworkReceivedPacket*)>								NetworkMessageFunction;
	typedef std::function<bool(void*)>																EventFunction;
	//int for event ID,cMessageSender for sender.
	typedef std::function<bool(int, NamedTypedObject*,FATMING_CORE::sNetworkReceivedPacket*)>		NetworkMessageDelegate;
	typedef std::function<bool(int, NamedTypedObject*,void*)>										EventDelegate;


	//REG_NET_MESSAGE_FUNCTION(MSG_GS2CL_CHANGE_ROUTE_RESPONSE,&cTradeRouteDataFromServer::Received_MSG_GS2CL_CHANGE_ROUTE_RESPONSE);
	//REG_EVENT(eOPEM_CLOSE_TRADE_ROUTE_UI_LAYOUT,&cRegionMapChange::OnCloseLayout);
	#define REG_NET_MESSAGE_FUNCTION(MESSAGE_ID,Function)RegNetworkMessageFunction(MESSAGE_ID,std::bind(Function,this,std::placeholders::_1));
	#define	REG_EVENT(EventID,Function)RegEvent(EventID,std::bind(Function,this,std::placeholders::_1));

	#define REG_NET_MESSAGE_DELEGATE(MESSAGE_ID,Function)RegNetworkMessageDelegate(MESSAGE_ID,std::bind(Function,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
	#define	REG_DELEGATE(EventID,Function)RegDelegate(EventID,std::bind(Function,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));




	class cMessageSenderManager;

	class cMessageSender
	{
		friend class cMessageSenderManager;
		//network
		std::map<unsigned int, NetworkMessageFunction>		m_NetworkMessageFunctionMap;
		std::map<unsigned int, NetworkMessageDelegate>		m_NetworkMessageDelegateMap;
		//event
		std::map<unsigned int, EventFunction>				m_EventFunctionMap;
		std::map<unsigned int, EventDelegate>				m_EventDelegateMap;
		//
		cMessageSenderManager*m_pParent;
		//
		void												SetParent(cMessageSenderManager*e_pParent = nullptr);
	public:
		cMessageSender(cMessageSenderManager*e_pParent = nullptr);
		virtual ~cMessageSender();
		//
		bool					RegEvent(unsigned int e_usID, EventFunction e_MessageFunction);
		bool					RegDelegate(unsigned int e_usID, EventDelegate e_EventDelegate);
		//see REG_NET_MESSAGE_FUNCTION
		template <class T>bool	RegNetworkMessageFunction(unsigned int e_usID, NetworkMessageFunction e_MessageFunction);
		bool					RegNetworkMessageFunction(unsigned int e_usID, NetworkMessageFunction e_MessageFunction);
		template <class T>bool	RegNetworkMessageDelegate(unsigned int e_usID, NetworkMessageDelegate e_MessageFunction);
		bool					RegNetworkMessageDelegate(unsigned int e_usID, NetworkMessageDelegate e_MessageFunction);
		//
		bool					UnregNetworkMessageFunction(unsigned int e_usID);
		bool					UnregNetworkMessageDelegate(unsigned int e_usID);
		bool					UnregEvent(unsigned int e_usID);
		bool					UnregDelegate(unsigned int e_usID);
		void					UnregistorAll();
		//do not use this one!.
		cMessageSenderManager*	GetParent() { return m_pParent; }
	};


	class cMessageSenderManager
	{
		friend class cMessageSender;
		//
		template<class TYPE>
		struct sNetworkMessageFunctionAndObjectID
		{
			size_t	uiAddress;
			TYPE	f_NetworkMessageFunction;
		};
		template<class TYPE>
		struct sEventFunctionAndType
		{
			size_t			uiAddress;
			TYPE			f_EventFunction;
			//void*			pData;
		};
		struct sWaitEmitEvent
		{
			unsigned int		usID;
			void*				pData;
			char				cData[WAIT_EMIT_EVENT_DATA_SIZE];
			bool				bDelegateType;
			NamedTypedObject* pSender;
			sWaitEmitEvent() { bDelegateType = false; pSender = nullptr; pData = nullptr; memset(cData, 0, WAIT_EMIT_EVENT_DATA_SIZE * sizeof(char)); }
			~sWaitEmitEvent() {}
		};
		void				AddMessageSender(size_t e_uiAddress,cMessageSender*e_pMessageSender);
		void				RemoveMessageSender(size_t e_uiAddress);
	private:
		//key is address
		std::map<size_t,cMessageSender*>	m_AllMessageSenderMap;
		//
		std::map< unsigned int, std::vector<sNetworkMessageFunctionAndObjectID<NetworkMessageFunction>> >	m_NetworkMessageFunctionAndObjectIDMap;
		std::map< unsigned int, std::vector<sNetworkMessageFunctionAndObjectID<NetworkMessageDelegate>> >	m_NetworkMessageDelegateAndObjectIDMap;
		//fix memory leak problem
		std::map< unsigned int, std::vector<sEventFunctionAndType<EventFunction>> >							m_EventFunctionAndTypeMap;
		std::map< unsigned int, std::vector<sEventFunctionAndType<EventDelegate>> >							m_EventDelegateAndTypeMap;
		//
		std::vector<sWaitEmitEvent>																			m_WaitForEmitEvent;
#ifndef _M_CEE//manage code dont support thread
		std::mutex																							m_WaitForEmitEventMutex;
#endif
	public:
		cMessageSenderManager();
		~cMessageSenderManager();
		bool	NetworkMessageShot(unsigned int e_usID,sNetworkReceivedPacket*e_pNetworkReceivedPacket);
		bool	EventMessageShot(unsigned int e_usID, void*e_pData);
		//ensure size is small than WAIT_EMIT_EVENT_DATA_SIZE
		bool	EventMessageShot(unsigned int e_usID, char*e_pData, int e_iSize);
		//ensure not call recursively,event call evnet infinty
		bool	EventMessageShotImmediately(unsigned int e_usID, void* e_pData);
		//
		bool	NetworkMessageDelegateShot(NamedTypedObject* e_pSender,unsigned int e_usID, sNetworkReceivedPacket* e_pNetworkReceivedPacket);
		bool	EventMessageDelegateShot(NamedTypedObject*e_pSender,unsigned int e_usID, void* e_pData);
		bool	EventMessageDelegateShot(NamedTypedObject* e_pSender,unsigned int e_usID, char* e_pData, int e_iSize);
		bool	EventMessageDelegateShotImmediately(NamedTypedObject* e_pSender,unsigned int e_usID, void* e_pData);

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

		cMessageSenderManager::sNetworkMessageFunctionAndObjectID<NetworkMessageFunction>	l_sMessageFunctionAndType;
		l_sMessageFunctionAndType.f_NetworkMessageFunction = e_MessageFunction;
		l_sMessageFunctionAndType.uiAddress = (size_t)this;
		m_NetworkMessageFunctionMap[e_usID] = e_MessageFunction;
		if (m_pParent)
		{
			std::vector<cMessageSenderManager::sNetworkMessageFunctionAndObjectID<NetworkMessageFunction>>*l_pVector = nullptr;
			auto l_Iterator = m_pParent->m_NetworkMessageFunctionAndObjectIDMap.find(e_usID);
			if (l_Iterator == m_pParent->m_NetworkMessageFunctionAndObjectIDMap.end())
			{
				m_pParent->m_NetworkMessageFunctionAndObjectIDMap[e_usID] = std::vector<cMessageSenderManager::sNetworkMessageFunctionAndObjectID<NetworkMessageFunction>>();
			}
			l_pVector = &m_pParent->m_NetworkMessageFunctionAndObjectIDMap[e_usID];
			cMessageSenderManager::sNetworkMessageFunctionAndObjectID<NetworkMessageFunction>l_EventFunction;
			l_EventFunction.f_NetworkMessageFunction = e_MessageFunction;
			l_EventFunction.uiAddress = (size_t)this;
			l_pVector->push_back(l_EventFunction);
			return true;
		}
		return false;
	}

	template <class T>bool	cMessageSender::RegNetworkMessageDelegate(unsigned int e_usID, NetworkMessageDelegate e_MessageFunction)
	{
		SetParent();
		if (this->m_NetworkMessageDelegateMap.find(e_usID) != m_NetworkMessageDelegateMap.end())
			return false;
		m_NetworkMessageDelegateMap[e_usID] = e_MessageFunction;

		cMessageSenderManager::sNetworkMessageFunctionAndObjectID<NetworkMessageDelegate>	l_sMessageFunctionAndType;
		l_sMessageFunctionAndType.f_NetworkMessageFunction = e_MessageFunction;
		l_sMessageFunctionAndType.uiAddress = (size_t)this;
		m_NetworkMessageDelegateMap[e_usID] = e_MessageFunction;
		if (m_pParent)
		{
			std::vector<cMessageSenderManager::sNetworkMessageFunctionAndObjectID<NetworkMessageDelegate>>* l_pVector = nullptr;
			auto l_Iterator = m_pParent->m_NetworkMessageDelegateAndObjectIDMap.find(e_usID);
			if (l_Iterator == m_pParent->m_NetworkMessageDelegateAndObjectIDMap.end())
			{
				std::vector<cMessageSenderManager::sNetworkMessageFunctionAndObjectID<NetworkMessageDelegate>>	l_Temp;
				m_pParent->m_NetworkMessageDelegateAndObjectIDMap[e_usID] = l_Temp;
			}
			l_pVector = &m_pParent->m_NetworkMessageDelegateAndObjectIDMap[e_usID];
			cMessageSenderManager::sNetworkMessageFunctionAndObjectID<NetworkMessageDelegate> l_EventFunction;
			l_EventFunction.f_NetworkMessageFunction = e_MessageFunction;
			l_EventFunction.uiAddress = (size_t)this;
			l_pVector->push_back(l_EventFunction);
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