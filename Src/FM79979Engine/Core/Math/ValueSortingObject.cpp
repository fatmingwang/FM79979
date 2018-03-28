#include "../stdafx.h"
#include "ValueSortingObject.h"
namespace	FATMING_CORE
{
	void Dump(float e_fValue)
	{
		//FMLog::LogWithFlag(ValueToStringW(e_fValue));
		//FMLog::LogWithFlag(L",");
	}
	void Dump(cSortObject*e_p)
	{
		//int l_iTimes = 0;
		//cSortObject*l_pSmallest = GetSmallest(e_p);
		//while( l_pSmallest )
		//{
		//l_iTimes += l_pSmallest->m_iCompareTime;
		//FMLog::LogWithFlag(ValueToStringW(l_pSmallest->m_fValue));
		//FMLog::LogWithFlag(L",");
		//l_pSmallest = l_pSmallest->m_pBigger;
		//}
		//FMLog::LogWithFlag(L"\n");
		//FMLog::LogWithFlag(L"compare time");
		//Dump(l_iTimes);
		//FMLog::LogWithFlag(L"\n");
	}

	void cSortObject::DoBigger(cSortObject*e_pObject)
	{
		++m_iCompareTime;
		if( e_pObject->m_fValue <= m_fValue )
		{
		if( m_pSmaller )
		{
		m_pSmaller->m_pBigger = e_pObject;
		e_pObject->m_pSmaller = m_pSmaller;
		}
		m_pSmaller = e_pObject;
		e_pObject->m_pBigger = this;
		return;
		}
		if( !m_pBigger )
		{
		m_pBigger = e_pObject;
		e_pObject->m_pSmaller = this;
		return;
		}
		if( m_pBigger == e_pObject )
		return;
		m_pBigger->DoBigger(e_pObject);
	}
	void cSortObject::DoSmaller(cSortObject*e_pObject)
	{
		++m_iCompareTime;
		if( e_pObject->m_fValue > m_fValue )
		{
			if( m_pBigger )
			{
				m_pBigger->m_pSmaller = e_pObject;
				e_pObject->m_pBigger = m_pBigger;
			}
			m_pBigger = e_pObject;
			e_pObject->m_pSmaller = this;
			return;
		}
		if( !m_pSmaller )
		{
			m_pSmaller = e_pObject;
			e_pObject->m_pBigger = this;
		return;
		}
		if( m_pSmaller == e_pObject )
			return;
		m_pSmaller->DoSmaller(e_pObject); 
	}

	cSortObject::cSortObject(float e_fValue)
	{
		m_iCompareTime = 0;
		m_pSmaller = 0;
		m_pBigger = 0;
		m_fValue = e_fValue;
	}
	void cSortObject::Sort(cSortObject*e_pObject)
	{
		++m_iCompareTime;
		if( e_pObject->m_fValue >m_fValue )
		{
			DoBigger(e_pObject);
		}
		else
		{
			DoSmaller(e_pObject);
		}
	}

	cSortObject*cSortObject::GetSmallest()
	{
		cSortObject*l_pSmallest = this->m_pSmaller;
		while( l_pSmallest )
		{
			if( !l_pSmallest->m_pSmaller )
				return l_pSmallest;
			l_pSmallest = l_pSmallest->m_pSmaller;
		}
		return this;
	}
//end namespace	FATMING_CORE
}