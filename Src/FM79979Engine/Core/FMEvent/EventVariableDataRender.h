#ifndef _EVENT_VARIABLE_DATA_RENDER_H_
#define _EVENT_VARIABLE_DATA_RENDER_H_

namespace FATMING_CORE
{
	//<cEventObject Type="VariableRender" Name="AuditImgObj">
	//	<cEventVariableDataRender Pos="100,200,0" cEventVariable="AuditPassword" AnchorType="Right" />
	//</cEventObject>
	class	cEventVariable;
	class   cEventVariableDataRender:public cEventObject
	{
		std::wstring						m_strPrefixText;
		virtual NamedTypedObject*           GetResourcePointer()override;
		virtual void    InternalRender()override;
		cEventVariable*	m_pEventVariable;
		eImageAnchorType	m_eImageAnchorType;
	public:
		DEFINE_TYPE_INFO();
		cEventVariableDataRender(TiXmlElement*e_pTiXmlElement);
		cEventVariableDataRender(cEventVariable*e_pNameAndData,Vector3 e_vPos,const WCHAR*e_strPrefixText);
		cEventVariableDataRender(cEventVariableDataRender*e_pEventVariableDataRender);
		virtual ~cEventVariableDataRender();
		EVENT_CLONE_DEFINE(cEventVariableDataRender);
		virtual	TiXmlElement*				ToTiXmlElement()override;
		void								SetPrefixText(const WCHAR*e_str){ m_strPrefixText = e_str; }
		void								SetEventVariable(cEventVariable*e_pEventVariable){ m_pEventVariable = e_pEventVariable; }
	};

}

#endif