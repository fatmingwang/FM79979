#include "../../stdafx.h"
#include "MessageSender.h"
#include "../../Network/Network.h"

namespace FATMING_CORE
{
	cMessageSenderManager*g_pMessageSenderManager = nullptr;

	cMessageSenderManager::sNetworkMessageFunctionAndObjectID::sNetworkMessageFunctionAndObjectID()
	{
		pConvertType = nullptr;
	}
	cMessageSenderManager::sNetworkMessageFunctionAndObjectID::~sNetworkMessageFunctionAndObjectID()
	{
		SAFE_DELETE(pConvertType);
	}

	cMessageSender::cMessageSender()
	{
		m_pParent = nullptr;
	}

	cMessageSender::~cMessageSender()
	{
		UnregistorAll();
	}

	void	cMessageSender::UnregistorAll()
	{
		{
			auto l_Iterator = m_NetworkMessageFunctionMap.begin();
			while( l_Iterator != m_NetworkMessageFunctionMap.end() )
			{
				this->UnregNetworkMessageFunction(l_Iterator->first);
				l_Iterator = m_NetworkMessageFunctionMap.begin();
			}
		}
		{
			auto l_Iterator = m_EventFunctionMap.begin();
			while( l_Iterator != m_EventFunctionMap.end() )
			{
				this->UnregEvent(l_Iterator->first);
				l_Iterator = m_EventFunctionMap.begin();
			}
		}
		if( m_pParent )
		{
			size_t	l_uiSize = m_pParent->m_AllMessageSenderVector.size();
			for (size_t i = 0; i < l_uiSize; i++)
			{
				if(m_pParent->m_AllMessageSenderVector[i] == this)
				{
					m_pParent->m_AllMessageSenderVector.erase(m_pParent->m_AllMessageSenderVector.begin()+i);
					break;
				}
			}
			m_pParent = nullptr;
		}
	}

	void	cMessageSender::Setparent()
	{
		if( m_pParent == nullptr )
		{
			assert(g_pMessageSenderManager&&"please create cMessageSenderManager first");
			m_pParent = g_pMessageSenderManager;
			m_pParent->m_AllMessageSenderVector.push_back(this);
		}
	}

	bool	cMessageSender::RegEvent(unsigned int e_usID,EventFunction e_MessageFunction)
	{	
		Setparent();
		if( this->m_EventFunctionMap.find(e_usID) != m_EventFunctionMap.end())
			return false;
		m_EventFunctionMap[e_usID] = e_MessageFunction;
		if( m_pParent )
		{
			std::vector<cMessageSenderManager::sEventFunctionAndType*>*l_pVector = nullptr;
			auto l_Iterator = m_pParent->m_EventFunctionAndTypeMap.find(e_usID);
			if( l_Iterator == m_pParent->m_EventFunctionAndTypeMap.end() )
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

	bool	cMessageSender::UnregNetworkMessageFunction(unsigned int e_usID)
	{
		if( m_pParent )
		{
			auto l_Iterator = m_pParent->m_NetworkMessageFunctionAndObjectIDMap.find(e_usID);
			if( l_Iterator != m_pParent->m_NetworkMessageFunctionAndObjectIDMap.end() )
			{
				std::vector<cMessageSenderManager::sNetworkMessageFunctionAndObjectID*>*l_pVector = &l_Iterator->second;
				size_t l_uiSize = l_pVector->size();
				for (size_t i = 0; i < l_uiSize; i++)
				{
					if((*l_pVector)[i]->uiAddress == (size_t)this)
					{
						delete (*l_pVector)[i];
						l_pVector->erase(l_pVector->begin()+i);
						--i;
						--l_uiSize;
					}
				}
				if( l_pVector->size() == 0 )
				{
					m_pParent->m_NetworkMessageFunctionAndObjectIDMap.erase(l_Iterator);
				}
			}
		}
		auto l_Itreator = m_NetworkMessageFunctionMap.find(e_usID);
		if( l_Itreator != m_NetworkMessageFunctionMap.end() )
		{
			m_NetworkMessageFunctionMap.erase(l_Itreator);
		}
		return true;
	}


	bool	cMessageSender::UnregEvent(unsigned int e_usID)
	{
		if( m_pParent )
		{
			auto l_Iterator = m_pParent->m_EventFunctionAndTypeMap.find(e_usID);
			if( l_Iterator != m_pParent->m_EventFunctionAndTypeMap.end() )
			{
				std::vector<cMessageSenderManager::sEventFunctionAndType*>*l_pVector = &l_Iterator->second;
				size_t l_uiSize = l_pVector->size();
				for (size_t i = 0; i < l_uiSize; i++)
				{
					if((*l_pVector)[i]->uiAddress == (size_t)this)
					{
						delete (*l_pVector)[i];
						l_pVector->erase(l_pVector->begin()+i);
						--i;
						--l_uiSize;
					}
				}
				if( l_pVector->size() == 0 )
				{
					m_pParent->m_EventFunctionAndTypeMap.erase(l_Iterator);
				}
			}
		}
		auto l_Itreator = m_EventFunctionMap.find(e_usID);
		if( l_Itreator != m_EventFunctionMap.end() )
		{
			m_EventFunctionMap.erase(l_Itreator);
		}
		return true;
	}

	cMessageSenderManager::cMessageSenderManager()
	{
		assert(g_pMessageSenderManager == nullptr&&"only could have one MessageSenderManager");
		g_pMessageSenderManager = this;
	}
	cMessageSenderManager::~cMessageSenderManager()
	{
		//for(size_t	i = 0;i<m_AllMessageSenderVector.size();++i)
		//{
		//	m_AllMessageSenderVector[i]->UnregistorAll();
		//}
		while( m_AllMessageSenderVector.size() )
		{
			m_AllMessageSenderVector[0]->UnregistorAll();
		}
		while(m_WaitForEmitEvent.size())
		{
			delete m_WaitForEmitEvent[0];
			m_WaitForEmitEvent.erase(m_WaitForEmitEvent.begin());
		}
		assert(m_NetworkMessageFunctionAndObjectIDMap.size() == 0);
		assert(m_EventFunctionAndTypeMap.size() == 0);
	}

	bool cMessageSenderManager::NetworkMessageShot(unsigned int e_usID,FATMING_CORE::sReceivedPacket* e_pPacket)
	{
		unsigned short iSize=*(unsigned short*)e_pPacket->pData;
		unsigned short iMessageID=*(((unsigned short*)(e_pPacket->pData))+1);
		printf("MessageIDInProcess:%d\n",iMessageID);
		if( e_pPacket == nullptr )
		{//for test
			auto l_Iterator = m_NetworkMessageFunctionAndObjectIDMap.find(e_usID);
			if( l_Iterator != m_NetworkMessageFunctionAndObjectIDMap.end() )
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
		if( l_Iterator2 != m_NetworkMessageFunctionAndObjectIDMap.end() )
		{
			std::vector<sNetworkMessageFunctionAndObjectID*>*l_pVector = &l_Iterator2->second;
			size_t l_uiSize = l_pVector->size();
			for (size_t i = 0; i < l_uiSize; i++)
			{
				if( 1 )//do packet data check
				{
					FATMING_CORE::sReceivedPacket*l_pPhotoBuffer = (*l_pVector)[i]->pConvertType;
	#ifdef _DEBUG

	#endif
					(*l_pVector)[i]->f_NetworkMessageFunction(l_pPhotoBuffer);
				}
				else
				{
					assert("protobuffer type error" &&0);
				}
			}
		}
		else
		{
			printf("fuck no one process this message %d\n",iMessageID);
		}
		return false;
	}

	bool cMessageSenderManager::EventMessageShot(unsigned int e_usID,void*e_pData)
	{
		sWaitEmitEvent*l_pWaitEmitEvent = new sWaitEmitEvent();
		l_pWaitEmitEvent->pData = e_pData;
		l_pWaitEmitEvent->usID = e_usID;
		m_WaitForEmitEvent.push_back(l_pWaitEmitEvent);
		return true;
	}

	void	cMessageSenderManager::Update(float e_fElpaseTime)
	{
		while( m_WaitForEmitEvent.size() )
		{
			sWaitEmitEvent*l_pWaitEmitEvent = m_WaitForEmitEvent[0];
			auto l_Iterator = m_EventFunctionAndTypeMap.find(l_pWaitEmitEvent->usID);
			if( l_Iterator != m_EventFunctionAndTypeMap.end())
			{
				std::vector<sEventFunctionAndType*>*l_pVector = &l_Iterator->second;
				for (int i = 0; i < (int)l_pVector->size(); i++)
				{
					if( (*l_pVector)[i]->f_EventFunction != nullptr )
					{
						(*l_pVector)[i]->f_EventFunction(l_pWaitEmitEvent->pData);
					}
				}
			}
			delete m_WaitForEmitEvent[0];
			m_WaitForEmitEvent.erase(m_WaitForEmitEvent.begin());
		}
	}
//end namespace FATMING_CORE
}