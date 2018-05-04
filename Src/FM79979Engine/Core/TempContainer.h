#pragma once

#include <vector>
#include "CommonDefine.h"
#include "GameplayUT/Log/FMLog.h"

template <class	T>class	cTempContainer
{
public:
	T**		m_ppObjects;
	int		m_iCount;
	int		m_iNumWorking;
	cTempContainer()
	{
		m_iNumWorking = 0;
		m_ppObjects = 0;
		m_iCount = 0;
	}
	void	SetCount(int e_iCount)
	{
		Clear();
		m_iNumWorking = 0;
		m_iCount = e_iCount;
		m_ppObjects = new T*[e_iCount];
		for (int i = 0; i < e_iCount; ++i)
			m_ppObjects[i] = nullptr;
	}
	void	ChangeCount(int e_iCount)
	{
		int l_iCount = e_iCount;
		int l_iNumWorking = m_iNumWorking;
		//copy old data
		T**l_ppObjects = new T*[e_iCount];
		for (int i = 0; i < l_iNumWorking; ++i)
		{
			l_ppObjects[i] = m_ppObjects[i];
		}
		this->Clear();
		this->m_ppObjects = l_ppObjects;
		this->m_iCount = l_iCount;
		this->m_iNumWorking = l_iNumWorking;
	}
	virtual ~cTempContainer(){ SAFE_DELETE_ARRAY(m_ppObjects); }
	void	Remove(int e_iIndex)
	{
		assert(m_iNumWorking > 0 && e_iIndex > -1 && e_iIndex<m_iNumWorking);
		m_ppObjects[e_iIndex] = m_ppObjects[m_iNumWorking-1];
		m_ppObjects[m_iNumWorking - 1] = nullptr;
		--m_iNumWorking;
	}
	void	Add(T*e_pObject)
	{
		m_ppObjects[m_iNumWorking] = e_pObject;
		++m_iNumWorking;
	}
	void	Clear()
	{
		m_iCount = 0;
		m_iNumWorking = 0;
		SAFE_DELETE_ARRAY(m_ppObjects);
	}
	int	GetObjectIndex(T*e_pObject)
	{
		int l_iCount = m_iNumWorking;
		for (int i = 0; i < l_iCount; i++)
		{
			if (m_iNumWorking == m_ppObjects[i])
				return i;
		}
		return -1;;
	}
};

//https://baptiste-wicht.com/posts/2012/12/cpp-benchmark-vector-list-deque.html
template< class TYPE >
class cWaitForFetchFunctionObjectList :public std::vector<TYPE*>, public NamedTypedObject
{
public:
	cTempContainer<TYPE>	m_WorkingList;
	cTempContainer<TYPE>	m_WaitFetchList;
	//std::list<TYPE*>		m_WorkingList;
	//std::list<TYPE*>		m_WaitFetchList;
public:
	cWaitForFetchFunctionObjectList(int e_iWorkingCount = 200)
	{
		this->reserve(e_iWorkingCount);
	}
	virtual ~cWaitForFetchFunctionObjectList()
	{
		DELETE_VECTOR((*this));
	}
	void	PushToWaitFetchList()
	{
		int l_iSize = (int)this->size();
		//for more capacity
		m_WorkingList.SetCount(l_iSize * 2);
		m_WaitFetchList.SetCount(l_iSize * 2);
		for (int i = 0; i < l_iSize; i++)
		{
			auto l_pObject = (*this)[i];
			m_WaitFetchList.m_ppObjects[i] = l_pObject;
		}
		m_WaitFetchList.m_iNumWorking = l_iSize;
	}
	TYPE*	RequireAvaliableObject()
	{
		TYPE*l_pResult = nullptr;
		if (m_WaitFetchList.m_iNumWorking > 0)
		{
			l_pResult = m_WaitFetchList.m_ppObjects[0];
			m_WaitFetchList.Remove(0);
			m_WorkingList.Add(l_pResult);
		}
		else
		{
			assert(this->size() > 0 && "this type dont have any object!?cWaitForFetchFunctionObjectList");
			TYPE*l_pObject = this->back();
			//for better peformance
			//l_pResult = new TYPE(l_pObject);
			l_pResult = dynamic_cast<TYPE*>(l_pObject->Clone());
			l_pResult->Init();
			this->push_back(l_pResult);

			if (m_WorkingList.m_iCount < (int)this->size())
			{
				m_WorkingList.ChangeCount(m_WorkingList.m_iCount * 2);
				m_WaitFetchList.ChangeCount(m_WaitFetchList.m_iCount * 2);
			}
			m_WorkingList.Add(l_pResult);
#ifdef DEBUG
			std::wstring l_str = l_pObject->Type();
			l_str += L":";
			l_str += l_pObject->GetName();
			l_str += L":Count not enough";
			FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
#endif
		}
		return  l_pResult;
	}

	void	WorkObjectFinish(int e_iWorkingObjectIndex)
	{
		m_WaitFetchList.Add(m_WorkingList.m_ppObjects[e_iWorkingObjectIndex]);
		m_WorkingList.Remove(e_iWorkingObjectIndex);
	}
	cTempContainer<TYPE>*	GetWorkingList() { return &m_WorkingList; }
};