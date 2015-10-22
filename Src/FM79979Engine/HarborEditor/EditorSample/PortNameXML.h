#pragma once

class  cIDAndName:public NamedTypedObject
{
public:
	std::wstring	m_PortName;
	int				m_iPortID;
};

class cPortsName:public cNamedTypedObjectVector<cIDAndName>,public cNodeISAX
{
	virtual	bool	MyParse(TiXmlElement*e_pRoot);
public:
};