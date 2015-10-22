#include "stdafx.h"
#include "PortNameXML.h"


bool	cPortsName::MyParse(TiXmlElement*e_pRoot)
{
	e_pRoot = e_pRoot->FirstChildElement();
	while( e_pRoot )
	{
		cIDAndName*l_pIDAndName = new cIDAndName();
		const WCHAR*l_strName = e_pRoot->Attribute(L"name");
		std::wstring l_strNameWithout_ = wcstok((wchar_t*)l_strName,L"_");
		l_strNameWithout_ = wcstok(nullptr,L"_");
		l_pIDAndName->SetName(l_strNameWithout_);
		const WCHAR*l_strText = e_pRoot->GetText();
		l_pIDAndName->m_PortName = l_strText;
		l_pIDAndName->m_iPortID = GetInt(l_strNameWithout_.c_str());
		this->AddObjectNeglectExist(l_pIDAndName);
		e_pRoot = e_pRoot->NextSiblingElement();
	}
	return true;	
}