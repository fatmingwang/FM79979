#include "MessageSender.h"
#include "assert.h"
#include "../Utility.h"
#include "../Log/FMLog.h"
//#include "../../Network/Network.h"
//#include "../GameApp.h"
namespace FATMING_CORE
{
	cMessageSenderManager*g_pMessageSenderManager = nullptr;

	cMessageSenderManager::sNetworkMessageFunctionAndObjectID::sNetworkMessageFunctionAndObjectID()
	{
		//pConvertType = nullptr;
	}
	cMessageSenderManager::sNetworkMessageFunctionAndObjectID::~sNetworkMessageFunctionAndObjectID()
	{
		//SAFE_DELETE(pConvertType);
	}

	cMessageSender::cMessageSender(cMessageSenderManager*e_pParent)
	{
		m_pParent = nullptr;
		this->SetParent(e_pParent);
	}

	cMessageSender::~cMessageSender()
	{
		UnregistorAll();
	}

	void	cMessageSender::UnregistorAll()
	{
		{
			auto l_Iterator = m_NetworkMessageFunctionMap.begin();
			while (l_Iterator != m_NetworkMessageFunctionMap.end())
			{
				this->UnregNetworkMessageFunction(l_Iterator->first);
				l_Iterator = m_NetworkMessageFunctionMap.begin();
			}
		}
		{
			auto l_Iterator = m_EventFunctionMap.begin();
			while (l_Iterator != m_EventFunctionMap.end())
			{
				this->UnregEvent(l_Iterator->first);
				l_Iterator = m_EventFunctionMap.begin();
			}
		}
		if (m_pParent)
		{
			m_pParent->RemoveMessageSender((size_t)this);
			m_pParent = nullptr;
		}
	}

	void	cMessageSender::SetParent(cMessageSenderManager* e_pParent)
	{
		if (m_pParent == nullptr)
		{
			if (e_pParent == nullptr)
			{
				assert(g_pMessageSenderManager && "please create cMessageSenderManager first");
				g_pMessageSenderManager->AddMessageSender((size_t)this, this);
				m_pParent = g_pMessageSenderManager;
			}
			else
			{
				m_pParent = e_pParent;
				e_pParent->AddMessageSender((size_t)this, this);
			}
		}
	}

	bool	cMessageSender::RegEvent(unsigned int e_usID, EventFunction e_MessageFunction)
	{
		SetParent(g_pMessageSenderManager);
		if (this->m_EventFunctionMap.find(e_usID) != m_EventFunctionMap.end())
			return false;
		m_EventFunctionMap[e_usID] = e_MessageFunction;
		if (m_pParent)
		{
			std::vector<cMessageSenderManager::sEventFunctionAndType*>*l_pVector = nullptr;
			auto l_Iterator = m_pParent->m_EventFunctionAndTypeMap.find(e_usID);
			if (l_Iterator == m_pParent->m_EventFunctionAndTypeMap.end())
			{
				std::vector<cMessageSenderManager::sEventFunctionAndType*>l_Temp;
				m_pParent->m_EventFunctionAndTypeMap[e_usID] = l_Temp;
			}
			l_pVector = &m_pParent->m_EventFunctionAndTypeMap[e_usID];
			cMessageSenderManager::sEventFunctionAndType*	l_EventFunction = new cMessageSenderManager::sEventFunctionAndType();
			l_EventFunction->uiAddress = (size_t)this;
			l_EventFunction->f_EventFunction = e_MessageFunction;
			l_pVector->push_back(l_EventFunction);
			return true;
		}
return false;
	}

	bool	cMessageSender::RegNetworkMessageFunction(unsigned int e_usID, NetworkMessageFunction e_MessageFunction)
	{
		return RegNetworkMessageFunction<void>(e_usID, e_MessageFunction);
	}

	bool	cMessageSender::UnregNetworkMessageFunction(unsigned int e_usID)
	{
		if (m_pParent)
		{
			auto l_Iterator = m_pParent->m_NetworkMessageFunctionAndObjectIDMap.find(e_usID);
			if (l_Iterator != m_pParent->m_NetworkMessageFunctionAndObjectIDMap.end())
			{
				std::vector<cMessageSenderManager::sNetworkMessageFunctionAndObjectID*>*l_pVector = &l_Iterator->second;
				size_t l_uiSize = l_pVector->size();
				for (size_t i = 0; i < l_uiSize; i++)
				{
					//basicly only have one,is it need to break?
					if ((*l_pVector)[i]->uiAddress == (size_t)this)
					{
						delete (*l_pVector)[i];
						l_pVector->erase(l_pVector->begin() + i);
						--i;
						--l_uiSize;
					}
				}
				if (l_pVector->size() == 0)
				{
					m_pParent->m_NetworkMessageFunctionAndObjectIDMap.erase(l_Iterator);
				}
			}
		}
		auto l_Itreator = m_NetworkMessageFunctionMap.find(e_usID);
		if (l_Itreator != m_NetworkMessageFunctionMap.end())
		{
			m_NetworkMessageFunctionMap.erase(l_Itreator);
		}
		return true;
	}


	bool	cMessageSender::UnregEvent(unsigned int e_usID)
	{
		if (m_pParent)
		{
			auto l_Iterator = m_pParent->m_EventFunctionAndTypeMap.find(e_usID);
			if (l_Iterator != m_pParent->m_EventFunctionAndTypeMap.end())
			{
				std::vector<cMessageSenderManager::sEventFunctionAndType*>*l_pVector = &l_Iterator->second;
				size_t l_uiSize = l_pVector->size();
				for (size_t i = 0; i < l_uiSize; i++)
				{
					//basicly only have one,is it need to break?
					if ((*l_pVector)[i]->uiAddress == (size_t)this)
					{
						delete (*l_pVector)[i];
						l_pVector->erase(l_pVector->begin() + i);
						--i;
						--l_uiSize;
					}
				}
				if (l_pVector->size() == 0)
				{
					m_pParent->m_EventFunctionAndTypeMap.erase(l_Iterator);
				}
			}
		}
		auto l_Itreator = m_EventFunctionMap.find(e_usID);
		if (l_Itreator != m_EventFunctionMap.end())
		{
			m_EventFunctionMap.erase(l_Itreator);
		}
		return true;
	}

	cMessageSenderManager::cMessageSenderManager()
	{
		//assert(g_pMessageSenderManager == nullptr&&"only could have one MessageSenderManager");
		if(!g_pMessageSenderManager)
		g_pMessageSenderManager = this;
	}
	cMessageSenderManager::~cMessageSenderManager()
	{
		while(m_AllMessageSenderMap.size())
		{//UnregistorAll will erase from m_AllMessageSenderMap
			m_AllMessageSenderMap.begin()->second->UnregistorAll();
		}
		while (m_WaitForEmitEvent.size())
		{
			delete m_WaitForEmitEvent[0];
			m_WaitForEmitEvent.erase(m_WaitForEmitEvent.begin());
		}
		assert(m_NetworkMessageFunctionAndObjectIDMap.size() == 0);
		assert(m_EventFunctionAndTypeMap.size() == 0);
		g_pMessageSenderManager = nullptr;
	}

	void	cMessageSenderManager::AddMessageSender(size_t e_uiAddress, cMessageSender*e_pMessageSender)
	{
		assert(m_AllMessageSenderMap[e_uiAddress] == nullptr && "AddMessageSender!!add again!?");
		m_AllMessageSenderMap.insert(std::make_pair(e_uiAddress, e_pMessageSender));
	}

	void	cMessageSenderManager::RemoveMessageSender(size_t e_uiAddress)
	{
		auto l_Iterator = m_AllMessageSenderMap.find(e_uiAddress);
		if (l_Iterator != m_AllMessageSenderMap.end())
		{
			m_AllMessageSenderMap.erase(e_uiAddress);
		}
	}

	bool cMessageSenderManager::NetworkMessageShot(unsigned int e_usID, FATMING_CORE::sNetworkReceivedPacket* e_pPacket)
	{
		//FMLog::LogWithFlag(UT::ComposeMsgByFormat("MessageIDInProcess:%d\n", e_usID).c_str(), CORE_LOG_FLAG);
		if (e_pPacket == nullptr)
		{//for test
			auto l_Iterator = m_NetworkMessageFunctionAndObjectIDMap.find(e_usID);
			if (l_Iterator != m_NetworkMessageFunctionAndObjectIDMap.end())
			{
				std::vector<sNetworkMessageFunctionAndObjectID*>*l_pVector = &l_Iterator->second;
				size_t l_uiSize = l_pVector->size();
				for (size_t i = 0; i < l_uiSize; i++)
				{
					(*l_pVector)[i]->f_NetworkMessageFunction(0);
				}
			}
			return false;
		}
		auto l_Iterator2 = m_NetworkMessageFunctionAndObjectIDMap.find(e_usID);
		if (l_Iterator2 != m_NetworkMessageFunctionAndObjectIDMap.end())
		{
			std::vector<sNetworkMessageFunctionAndObjectID*>*l_pVector = &l_Iterator2->second;
			size_t l_uiSize = l_pVector->size();
			for (size_t i = 0; i < l_uiSize; i++)
			{
				if (1)//do packet data check
				{
					(*l_pVector)[i]->f_NetworkMessageFunction(e_pPacket);
				}
				else
				{
					assert("protobuffer type error" && 0);
				}
			}
		}
		else
		{
			FMLog::LogWithFlag(UT::ComposeMsgByFormat("no one process this message %d\n", e_usID).c_str(), CORE_LOG_FLAG);
		}
		return false;
	}

	bool cMessageSenderManager::EventMessageShot(unsigned int e_usID, char*e_pData, int e_iSize)
	{
		sWaitEmitEvent*l_pWaitEmitEvent = new sWaitEmitEvent();
		l_pWaitEmitEvent->usID = e_usID;
		assert(e_iSize< WAIT_EMIT_EVENT_DATA_SIZE&&L"cMessageSenderManager::EventMessageShot data size over 4096(WAIT_EMIT_EVENT_DATA_SIZE)");
		memcpy(l_pWaitEmitEvent->cData, e_pData, e_iSize);
		m_WaitForEmitEvent.push_back(l_pWaitEmitEvent);
		return true;
	}

	bool cMessageSenderManager::EventMessageShotImmediately(unsigned int e_usID, void * e_pData)
	{
		auto l_Iterator = m_EventFunctionAndTypeMap.find(e_usID);
		if (l_Iterator != m_EventFunctionAndTypeMap.end())
		{
			std::vector<sEventFunctionAndType*>*l_pVector = &l_Iterator->second;
			for (int i = 0; i < (int)l_pVector->size(); i++)
			{
				if ((*l_pVector)[i]->f_EventFunction != nullptr)
				{
					(*l_pVector)[i]->f_EventFunction(e_pData);
				}
			}
		}
		return true;
	}

	bool cMessageSenderManager::EventMessageShot(unsigned int e_usID, void*e_pData)
	{
		sWaitEmitEvent*l_pWaitEmitEvent = new sWaitEmitEvent();
		l_pWaitEmitEvent->pData = e_pData;
		l_pWaitEmitEvent->usID = e_usID;
		m_WaitForEmitEvent.push_back(l_pWaitEmitEvent);
		return true;
	}

	void	cMessageSenderManager::Update(float e_fElpaseTime)
	{
		while (m_WaitForEmitEvent.size())
		{
			sWaitEmitEvent*l_pWaitEmitEvent = m_WaitForEmitEvent[0];
			auto l_Iterator = m_EventFunctionAndTypeMap.find(l_pWaitEmitEvent->usID);
			if (l_Iterator != m_EventFunctionAndTypeMap.end())
			{
				std::vector<sEventFunctionAndType*>*l_pVector = &l_Iterator->second;
				for (int i = 0; i < (int)l_pVector->size(); i++)
				{
					if ((*l_pVector)[i]->f_EventFunction != nullptr)
					{
						if (l_pWaitEmitEvent->pData)
							(*l_pVector)[i]->f_EventFunction(l_pWaitEmitEvent->pData);
						else
							(*l_pVector)[i]->f_EventFunction((void*)l_pWaitEmitEvent->cData);
					}
				}
			}
			m_WaitForEmitEvent.erase(m_WaitForEmitEvent.begin());
			delete l_pWaitEmitEvent;
		}
	}
	//end namespace FATMING_CORE
}