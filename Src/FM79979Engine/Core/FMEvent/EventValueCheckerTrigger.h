#ifndef _EVENT_VALUE_CHECKER_TRIGGER_H_
#define _EVENT_VALUE_CHECKER_TRIGGER_H_

#include "EventBase.h"
namespace FATMING_CORE
{
	//for key up event
	//<cEventValueCheck KeyData="S">
	class	cEventKeyUpDectect:public cEventBase
	{
		virtual	void	InternalInit()override;
		virtual	void	InternalUpdate(float e_fElpasedTime)override;
	public:
		DEFINE_TYPE_INFO();
		cEventKeyUpDectect(TiXmlElement*e_pTiXmlElement);
		cEventKeyUpDectect(cEventKeyUpDectect*e_pEventKeyUpDectect);
		EVENT_CLONE_DEFINE(cEventKeyUpDectect);
		virtual ~cEventKeyUpDectect();
		virtual	TiXmlElement*		ToTiXmlElement()override;
	};

	class	cEventVariable;
	//<cEventValueCheck Value="" cEventVariable="" Same="0" DataType="int" Value="1"  >
	//<cEventValueCheck cEventVariable=""  CompareEventVariable="" Same="0" DataType="int" Value="1" InitCompareEventVariable="79979"  >
	//same attribute could be ignored
	class	cEventValueCheck:public cEventBase
	{
		std::wstring*	m_pstrInitCompareEventVariableString;
		//
		bool			m_bSame;
		std::wstring*	m_pstrCheckValue;
		//eDataType		m_eDataType;
		cEventVariable*	m_pCheckTargetData;
		cEventVariable*	m_pCompareEventVariable;
		virtual	void	InternalInit()override;
		virtual	void	InternalUpdate(float e_fElpasedTime)override;
		virtual	void	InternalRender()override;
	public:
		DEFINE_TYPE_INFO();
		cEventValueCheck(TiXmlElement*e_pTiXmlElement);
		//cEventValueCheck(const WCHAR*e_strCheckValue,eDataType e_eDataType,cEventVariable*e_pCheckTargetData,bool	e_bSame);
		cEventValueCheck(cEventValueCheck*e_pEventValueCheck);
		EVENT_CLONE_DEFINE(cEventValueCheck);
		virtual ~cEventValueCheck();
		virtual	TiXmlElement*		ToTiXmlElement()override;
	};
//end namespace FATMING_CORE
}
#endif