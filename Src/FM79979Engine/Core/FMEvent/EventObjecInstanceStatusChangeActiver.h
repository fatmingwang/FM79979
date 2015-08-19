#ifndef _EVENT_OBJECT_INSTANCE_STATUS_CHANGER_ACTIVER_H_
#define _EVENT_OBJECT_INSTANCE_STATUS_CHANGER_ACTIVER_H_

namespace FATMING_CORE
{
	//=========================================
	//<cEventObjectInstanceStatusChangeActiver cEventObjectInstance="generator" StatusName="Hurt" />
	//<cEventObjectInstanceStatusChangeActiver cEventObjectInstance="generator" cEventVariable="generatorStatusName" />
	//=========================================
	class	cEventVariable;
	class	cEventObjectStatus;
	class	cEventObjectInstanceStatusChangeActiver:public cEventBase
	{
		std::wstring			m_strEventObjectInstanceName;
		//if variable is exists EventObjectStatus will set status by m_pStatusNameByVariable's value.
		cEventVariable*			m_pStatusNameByVariable;
		//
		std::wstring*			m_pstrStatusName;
		virtual	void			InternalInit();
		virtual	void			InternalUpdate(float e_fElpasedTime);
		virtual	void			InternalRender();
	public:
		DEFINE_TYPE_INFO();
		cEventObjectInstanceStatusChangeActiver(TiXmlElement*e_pTiXmlElement);
		//cEventObjectInstanceStatusChangeActiver(const WCHAR*e_strTargetStatusName,const WCHAR*e_strEventObjectInstanceName);
		cEventObjectInstanceStatusChangeActiver(cEventObjectInstanceStatusChangeActiver*e_pEventObjectInstanceStatusChangeActiver);
		EVENT_CLONE_DEFINE(cEventObjectInstanceStatusChangeActiver);
		virtual ~cEventObjectInstanceStatusChangeActiver();
		virtual	TiXmlElement*		ToTiXmlElement();
	};
//end namespace FATMING_CORE
}
#endif