#pragma once

#include <list>
template< class TYPE >
class cWaitForFetchFunctionObjectList :public std::list<TYPE*>,public NamedTypedObject
{
public:
	std::list<TYPE*>	m_WorkingList;
	std::list<TYPE*>	m_WaitFetchList;
public:
	cWaitForFetchFunctionObjectList(){}
	~cWaitForFetchFunctionObjectList()
	{
		CLEAR_AND_DELETE_OBJECT_IN_LIST(this);
	}
	TYPE * RequireAvaliableObject()
	{
		TYPE*l_pResult = nullptr;
		size_t l_uiSize = m_WaitFetchList.size();
		if (l_uiSize > 0)
		{
			l_pResult = m_WaitFetchList.back();
			m_WaitFetchList.pop_back();
			m_WorkingList.push_back(l_pResult);
		}
		else
		{
			assert(this->size() > 0 && "this type dont have any object!?cWaitForFetchFunctionObjectList");
			TYPE*l_pObject = this->back();
			//for better peformance
			//l_pResult = new TYPE(l_pObject);
			TYPE*l_pCloneObject = dynamic_cast<TYPE*>(l_pObject->Clone());
			this->push_back(l_pResult);
			m_WorkingList.push_back(l_pResult);
#ifdef DEBUG
			std::wstring l_str = l_pObject->Type();
			l_str += L":";
			l_str += l_pObject->GetName();
			l_str += L":Count not enough";
			cGameApp::OutputDebugInfoString(l_str);
#endif
		}
		return  l_pResult;
	}
};