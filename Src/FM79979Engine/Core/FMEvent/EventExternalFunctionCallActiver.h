#ifndef _EVENT_EXTERNAL_FUNCTION_CALL_ACTIVER_ACTIVER_H_
#define _EVENT_EXTERNAL_FUNCTION_CALL_ACTIVER_ACTIVER_H_

namespace FATMING_CORE
{
	class	cEventVariable;
	class	cEventExternalFunctionCallActiver:public cEventBase
	{
		std::wstring					m_strExternamFunctionName;
		virtual	void					InternalInit()override;
		virtual	void					InternalUpdate(float e_fElpasedTime)override;
	public:
		DEFINE_TYPE_INFO();
		cEventExternalFunctionCallActiver(TiXmlElement*e_pTiXmlElement);
		cEventExternalFunctionCallActiver(cEventExternalFunctionCallActiver*e_pEventExternalFunctionCallActiver);
		EVENT_CLONE_DEFINE(cEventExternalFunctionCallActiver);
		virtual ~cEventExternalFunctionCallActiver();
		virtual	TiXmlElement*		ToTiXmlElement()override;
	};
}

#endif