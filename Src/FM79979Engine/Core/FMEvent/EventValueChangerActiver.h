#ifndef _EVENT_VALUE_CHANGER_ACTIVER_H_
#define _EVENT_VALUE_CHANGER_ACTIVER_H_

#include "../GameplayUT/RestrictValue.h"
#include "../GameplayUT//ListValueChanger.h"
#include "EventBase.h"
namespace FATMING_CORE
{
	#define	VALUE_CHANGE_TYPE	L"ValueChangeType"
	enum	eValueeChangeType
	{
		eCVT_INCREASE = 0,
		eCVT_DECREASE,
		eCVT_TARGET_VALUE,
		eCVT_RANDOM_VALUE,//between the maximum and minimum
		eCVT_MAX
	};

	eValueeChangeType	GetValueeChangeType(const WCHAR*e_strValue);
	WCHAR*				ValueToStringW(eValueeChangeType e_eValueeChangeType);
//====================================================
// now only suport int data type
//====================================================
	//<TriggerEvent>
		//<cEventValueChangeActiver Name="Test" ValueChangeType="Increase" >
		//	<cRestrictValue />
		//</cEventValueChangeActiver>
		//if u want to directly to change event variable just use below.
		//<cEventValueChangeActiver Name="Test" ValueChangeType="TargetValue" cEventVariable="79979" TargetValue="79979" />
		//<cEventValueChangeActiver Name="Test" cEventVariable="79979" TargetEventVariable="2266" />	
	//79979
	//</TriggerEvent>
	class	cEventVariable;
	class	cEventValueChangeActiver:public cEventBase
	{
		GET_SET_DEC(cEventVariable*,m_pEventVariable,GetEventVariable,SetEventVariable);
		GET_SET_DEC(cEventVariable*,m_pTargetEventVariable,GetTargetEventVariable,SetTargetEventVariable);
		GET_SET_DEC(std::wstring*,m_pstrTargetString,GetTargetString,SetTargetString);
		GET_SET_DEC(eValueeChangeType,m_eValueeChangeType,GeteValueeChangeType,SeteValueeChangeType);
		GET_SET_DEC(cRestrictValueBase*,m_pRestrictValue,GetRestrictValue,SetRestrictValue);
		virtual	void			InternalInit();
		virtual	void			InternalUpdate(float e_fElpasedTime);
		virtual	void			InternalRender();
	public:
		DEFINE_TYPE_INFO();
		cEventValueChangeActiver(TiXmlElement*e_pTiXmlElement);
//		cEventValueChangeActiver(WCHAR*e_strName,eValueeChangeType	e_eValueeChangeType,cRestrictValueBase*e_pRestrictValue,WCHAR*e_strTargetValue = 0);
		cEventValueChangeActiver(cEventValueChangeActiver*e_pRestrictValueActiver);
		EVENT_CLONE_DEFINE(cEventValueChangeActiver);
		virtual ~cEventValueChangeActiver();
//		cRestrictValueBase*	GetRestrictValue(){return m_pRestrictValue;}
		virtual	TiXmlElement*		ToTiXmlElement();
	};
	//<cEventListValueSelectionActiver Name="Test" ValueChangeType="TargetValue" Variable="79979"  >
	//<cEventListValueSelectionActiver Name="Test" ValueChangeType="Increase"  >
		//	<cListValueChanger  />
	//</cEventListValueSelectionActiver>
	class	cEventListValueSelectionActiver:public cEventBase
	{
		//if m_eValueeChangeType is target set selected value by m_pstrTargetValue,or m_pstrTargetValue will be null
		std::wstring*			m_pstrTargetValue;
		eValueeChangeType		m_eValueeChangeType;
		//data for event activer ro trigger
		cListValuChangerBase*	m_pListValuChangerBase;
		cEventVariable*			m_pEventVariable;
		virtual	void	InternalInit();
		virtual	void	InternalUpdate(float e_fElpasedTime);
		virtual	void	InternalRender();
    public:
		DEFINE_TYPE_INFO();
		cEventListValueSelectionActiver(TiXmlElement*e_pTiXmlElement);
		//cEventListValueSelectionActiver(cListValuChangerBase*e_pListValuChangerBase,eValueeChangeType e_eValueeChangeType,WCHAR*e_strValue = 0);
		cEventListValueSelectionActiver(cEventListValueSelectionActiver*e_pListValueChangerActiver);
		EVENT_CLONE_DEFINE(cEventListValueSelectionActiver);
		virtual ~cEventListValueSelectionActiver();

		cListValuChangerBase*		GetListValueChanger();

		cIntListValueChanger*		GetIntListValueChanger();
		cFloatListValueChanger*		GetFloatListValueChanger();
		cStringListValueChanger*	GetStringListValueChanger();
		cWStringListValueChanger*	GetWStringListValueChanger();
		virtual	TiXmlElement*		ToTiXmlElement();
	};
//end namespace FATMING_CORE
}
#endif