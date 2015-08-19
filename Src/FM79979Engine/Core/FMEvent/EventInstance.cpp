#include "../stdafx.h"
#include "EventInstance.h"
namespace FATMING_CORE
{
	const wchar_t*     cEventInstance::TypeID( L"cEventInstance" );

	cEventInstance::cEventInstance()
	{
		m_bTriggerAllDoneAtSameTime = false;
		m_bRepeat = false;
		m_bTriggerAllDone = false;
		m_bActiveAllDone = false;
		m_TriggerEventList.SetOwner(this);
		m_ActiveEventList.SetOwner(this);
	}

	cEventInstance::cEventInstance(cEventInstance*e_pEventInstance)
	{
		m_bTriggerAllDoneAtSameTime = e_pEventInstance->m_bTriggerAllDoneAtSameTime;
		m_bTriggerAllDone = false;
		m_bActiveAllDone = false;
		m_bRepeat = e_pEventInstance->m_bRepeat;
		m_TriggerEventList.CloneFromList(&e_pEventInstance->m_TriggerEventList);
		m_ActiveEventList.CloneFromList(&e_pEventInstance->m_ActiveEventList);
		//{ADD_CLONE_DATA_TO_NEW_VECTOR(cEventBase,(&e_pEventInstance->m_TriggerEventList),(&m_TriggerEventList));}
		//{ADD_CLONE_DATA_TO_NEW_VECTOR(cEventBase,(&e_pEventInstance->m_ActiveEventList),(&m_ActiveEventList));}
		m_TriggerEventList.SetOwner(this);
		m_ActiveEventList.SetOwner(this);
		this->SetName(e_pEventInstance->GetName());
	}

	cEventInstance*	cEventInstance::Clone()
	{
		return new cEventInstance(this);
	}

	cEventInstance::~cEventInstance()
	{

	}

	bool	cEventInstance::UpdateEventList(cNamedTypedObjectVector<cEventBase>*e_EventList,float e_fElpasedTime)
	{
		int	l_iNum = e_EventList->Count();
		if( l_iNum )
		{
			cEventBase**l_ppEvent = 0;
			cEventBase*l_pEvent = 0;
			int	l_iNumDone = 0;
			l_ppEvent = &(*e_EventList->GetList())[0];
			l_pEvent = l_ppEvent[0];
			for( int i=0;i<l_iNum;++i )
			{
				l_pEvent = l_ppEvent[i];
				if(!l_pEvent->IsSatisfiedCondition())
				{
					l_pEvent->Update(e_fElpasedTime);
				}
				if( l_pEvent->IsSatisfiedCondition() )
					++l_iNumDone;
			}
			if( l_iNumDone != l_iNum )
			{
				if( m_bTriggerAllDoneAtSameTime )
				{
					if(e_EventList == &m_TriggerEventList && l_iNumDone != 0 )
					{
						for( int i=0;i<l_iNum;++i )
						{
							if( l_ppEvent[i]->IsSatisfiedCondition() )
							{
								l_ppEvent[i]->Init();
							}
						}
					}
				}
				return false;
			}
		}
		return true;
	}

	void	cEventInstance::RenderEventList(cNamedTypedObjectVector<cEventBase>*e_EventList)
	{
		int	l_iNum = e_EventList->Count();
		cEventBase**l_ppEvent = 0;
		cEventBase*l_pEvent = 0;
		int	l_iNumDone = 0;
		if( l_iNum )
		{
			l_ppEvent = &(*e_EventList->GetList())[0];
			l_pEvent = l_ppEvent[0];
			for( int i=0;i<l_iNum;++i )
			{
				l_pEvent = l_ppEvent[i];
				l_pEvent->Render();
			}
		}
	}

	void	cEventInstance::InternalInit()
	{
		LIST_DO_FOR_ALL_CHILDREN((&m_TriggerEventList),FetchData(cEventInstance::TypeID,this));
		LIST_DO_FOR_ALL_CHILDREN((&m_ActiveEventList),FetchData(cEventInstance::TypeID,this));
		m_TriggerEventList.Init();
		m_ActiveEventList.Init();
		m_bTriggerAllDone = false;
		m_bActiveAllDone = false;
	}

	void	cEventInstance::InternalUpdate(float e_fElpasedTime)
	{
		if( !m_bTriggerAllDone )
		{
			m_bTriggerAllDone = UpdateEventList(&m_TriggerEventList,e_fElpasedTime);
		}
		else
		if( !m_bActiveAllDone )
		{
			m_bActiveAllDone = UpdateEventList(&m_ActiveEventList,e_fElpasedTime);
			this->m_bSatisfiedCondition = m_bActiveAllDone;
			if( m_bSatisfiedCondition )
			{
				if(m_bRepeat)
				{
					this->Init();
					//int	l_iNum = m_ActiveEventList.Count();
					//cEventBase**l_ppEvent = 0;
					//if( l_iNum )
					//{
					//	l_ppEvent = &(*m_ActiveEventList.GetList())[0];
					//	for( int i=0;i<l_iNum;++i )
					//		l_ppEvent[i]->SetSatisfiedCondition(false);
					//}
				}	
			}
		}
	}

	void	cEventInstance::InternalRender()
	{
		if( !m_bTriggerAllDone )
		{
			RenderEventList(&m_TriggerEventList);
		}
		if( !m_bActiveAllDone )
		{
			RenderEventList(&m_ActiveEventList);
		}
	}

	void	cEventInstance::DebugRender()
	{
		int	l_iNum = m_TriggerEventList.Count();
		cEventBase**l_ppEvent = 0;
		cEventBase*l_pEvent = 0;
		if( l_iNum )
		{
			l_ppEvent = &(*m_TriggerEventList.GetList())[0];
			l_pEvent = l_ppEvent[0];
			for( int i=0;i<l_iNum;++i )
			{
				if(!l_pEvent->IsSatisfiedCondition())
				{
					l_pEvent->DebugRender();
				}
				++l_pEvent;
			}
		}
		l_iNum = m_ActiveEventList.Count();
		if( l_iNum )
		{
			l_ppEvent = &(*this->m_ActiveEventList.GetList())[0];
			l_pEvent = l_ppEvent[0];
			for( int i=0;i<l_iNum;++i )
			{
				if(!l_pEvent->IsSatisfiedCondition())
				{
					l_pEvent->DebugRender();
				}
				++l_pEvent;
			}
		}
	}

	void	cEventInstance::CollectResourceList(cNamedTypedObjectVector<NamedTypedObject>*e_pObjectListByName)
	{
		int	l_iCount = m_TriggerEventList.Count();
		for( int i=0;i<l_iCount;++i )
		{
			m_TriggerEventList[i]->CollectResourceList(e_pObjectListByName);
		}
		l_iCount = m_ActiveEventList.Count();
		for( int i=0;i<l_iCount;++i )
		{
			m_ActiveEventList[i]->CollectResourceList(e_pObjectListByName);
		}
	}

	void	cEventInstance::InternalMouseDown(int e_iX,int e_iY)
	{
		if( !m_bTriggerAllDone )
			m_TriggerEventList.MouseDown(e_iX,e_iY);
		else
		if( !m_bActiveAllDone )
			m_ActiveEventList.MouseDown(e_iX,e_iY);
	}

	void	cEventInstance::InternalMouseUp(int e_iX,int e_iY)
	{
		if( !m_bTriggerAllDone )
			m_TriggerEventList.MouseUp(e_iX,e_iY);
		else
		if( !m_bActiveAllDone )
			m_ActiveEventList.MouseUp(e_iX,e_iY);
		m_eObjectMouseBehavior = eOMB_NONE;
	}

	void	cEventInstance::InternalMouseMove(int e_iX,int e_iY)
	{
		if( !m_bTriggerAllDone )
			m_TriggerEventList.MouseMove(e_iX,e_iY);
		else
		if( !m_bActiveAllDone )
			m_ActiveEventList.MouseMove(e_iX,e_iY);
	}

	//cEventInstanceOwner::cEventInstanceOwner(cEventInstanceOwner*e_pEventInstanceOwner,bool e_bEventInstanceFromResource)
	//{
	//	e_pEventInstanceOwner = m_pEventInstanceOwner;
	//	SetFromResource(e_bEventInstanceFromResource);
	//}

	//cEventInstanceOwner::~cEventInstanceOwner()
	//{

	//}
	//bool	cEventInstanceOwner::SetEnevtInstanceByName(WCHAR*e_strEventInstanceName)
	//{
	//	m_pCurrentEventInstance = GetObject(e_strEventInstanceName);
	//	if( m_pCurrentEventInstance )
	//		return true;
	//	return false;
	//}

	//void	cEventInstanceOwner::EventUpdate(float e_fElpasedTime)
	//{
	//	if( !m_strCurrentEventInstanceName )
	//		return;
	//	if(!wcscmp(this->m_strCurrentEventInstanceName,this->m_pCurrentEventInstance->GetName()))
	//	{
	//		this->m_pCurrentEventInstance->Update(e_fElpasedTime);
	//	}
	//	else
	//	{
	//		this->SetEnevtInstanceByName(m_strCurrentEventInstanceName);
	//		assert(m_pCurrentEventInstance);
	//		this->m_pCurrentEventInstance->Update(e_fElpasedTime);
	//	}
	//}

	//void	cEventInstanceOwner::EventRender()
	//{
	//	this->m_pCurrentEventInstance->Render();
	//}
}