#pragma once

#include <list>
template< class TYPE >
class cWaitForFetchFunctionObjectList :public std::list<TYPE*>
{
	std::list<TYPE*>	m_WorkingList;
	std::list<TYPE*>	m_WaitFetchList;
	//
	void	Update(float e_fElpaseTime);
public:
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
		}
		return  l_pResult;
	}
};