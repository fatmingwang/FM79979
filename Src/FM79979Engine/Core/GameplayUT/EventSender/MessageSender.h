#pragma once
#include <functional>
#include <map>

namespace FATMING_CORE
{
	struct sReceivedPacket;
}

typedef std::function<bool(FATMING_CORE::sReceivedPacket*)>		NetworkMessageFunction;
typedef std::function<bool(void*)>								EventFunction;


//REG_NET_MESSAGE_FUNCTION(MSG_GS2CL_CHANGE_ROUTE_RESPONSE,&cTradeRouteDataFromServer::Received_MSG_GS2CL_CHANGE_ROUTE_RESPONSE);
#define REG_NET_MESSAGE_FUNCTION(proto,Function)RegNetworkMessageFunction<proto>(_##proto,std::bind(Function,this,std::placeholders::_1));
//REG_EVENT(eOPEM_CLOSE_TRADE_ROUTE_UI_LAYOUT,&cRegionMapChange::OnCloseLayout);
#define	REG_EVENT(EventID,Function)RegEvent(EventID,std::bind(Function,this,std::placeholders::_1));




class cMessageSenderManager;

class cMessageSender
{
	friend class cMessageSenderManager;
	//network
	std::map<unsigned int,NetworkMessageFunction>		m_NetworkMessageFunctionMap;
	//event
	std::map<unsigned int,EventFunction>				m_EventFunctionMap;
	//
	cMessageSenderManager*m_pParent;
	//
	void												Setparent();
public:
	cMessageSender();
	virtual ~cMessageSender();
	//please keep e_pData,or it will be a wild pointer.
	bool					RegEvent(unsigned int e_usID,EventFunction e_MessageFunction);
	//see REG_NET_MESSAGE_FUNCTION
	template <class T>bool	RegNetworkMessageFunction(unsigned int e_usID,NetworkMessageFunction e_MessageFunction);
	//
	bool					UnregNetworkMessageFunction(unsigned int e_usID);
	bool					UnregEvent(unsigned int e_usID);
	void					UnregistorAll();
	//do not use this one!.
	cMessageSenderManager*	GetParent(){ return m_pParent; }
};


class cMessageSenderManager
{
	friend class cMessageSender;
	//
	struct sNetworkMessageFunctionAndObjectID
	{
		size_t uiAddress;
		NetworkMessageFunction			f_NetworkMessageFunction;
		FATMING_CORE::sReceivedPacket*	pConvertType;
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
	};
public:
	std::vector<cMessageSender*>	m_AllMessageSenderVector;
	//vector<size_t> as many as senders.
	std::map< unsigned int,std::vector<sNetworkMessageFunctionAndObjectID*> >	m_NetworkMessageFunctionAndObjectIDMap;
	//fic memory leak problem
	std::map< unsigned int,std::vector<sEventFunctionAndType*> >				m_EventFunctionAndTypeMap;
	//
	//void	RemoveObject(cMessageSender*e_pMessageSender);
	//void	AddObject(cMessageSender*e_pMessageSender);
	std::vector<sWaitEmitEvent*>									m_WaitForEmitEvent;
public:
	cMessageSenderManager();
	~cMessageSenderManager();
	bool NetworkMessageShot(unsigned int e_usID,FATMING_CORE::sReceivedPacket* e_pPacket);
	bool EventMessageShot(unsigned int e_usID,void*e_pData);
	//for emit event for frame
	void	Update(float e_fElpaseTime);
};


template <class T>bool	cMessageSender::RegNetworkMessageFunction(unsigned int e_usID,NetworkMessageFunction e_MessageFunction)
{
	Setparent();
	if( this->m_NetworkMessageFunctionMap.find(e_usID) != m_NetworkMessageFunctionMap.end())
		return false;
	m_NetworkMessageFunctionMap[e_usID] = e_MessageFunction;

	cMessageSenderManager::sNetworkMessageFunctionAndObjectID	l_sMessageFunctionAndType;
	l_sMessageFunctionAndType.pConvertType = nullptr;
	l_sMessageFunctionAndType.f_NetworkMessageFunction = e_MessageFunction;
	l_sMessageFunctionAndType.uiAddress = (size_t)this;
	m_NetworkMessageFunctionMap[e_usID] = e_MessageFunction;
	if( m_pParent )
	{
		std::vector<cMessageSenderManager::sNetworkMessageFunctionAndObjectID*>*l_pVector = nullptr;
		auto l_Iterator = m_pParent->m_NetworkMessageFunctionAndObjectIDMap.find(e_usID);
		if( l_Iterator == m_pParent->m_NetworkMessageFunctionAndObjectIDMap.end() )
		{
			std::vector<cMessageSenderManager::sNetworkMessageFunctionAndObjectID*>	l_Temp;
			m_pParent->m_NetworkMessageFunctionAndObjectIDMap[e_usID] = l_Temp;
		}
		l_pVector = &m_pParent->m_NetworkMessageFunctionAndObjectIDMap[e_usID];
		cMessageSenderManager::sNetworkMessageFunctionAndObjectID*l_pEventFunction = new cMessageSenderManager::sNetworkMessageFunctionAndObjectID;
		l_pEventFunction->f_NetworkMessageFunction = e_MessageFunction;
		l_pEventFunction->pConvertType = new T();
		l_pEventFunction->uiAddress = (size_t)this;
		l_pVector->push_back(l_pEventFunction);
		return true;
	}
	return false;
}