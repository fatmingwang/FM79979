#include "stdafx.h"
#include "EventDataNode.h"
namespace FATMING_CORE
{

	cEventDataNode::cEventDataNode()
	{
		m_pDataNode = 0;
	}

	cEventDataNode::~cEventDataNode()
	{
		SAFE_DELETE(m_pDataNode);
	}

	TiXmlElement*		cEventDataNode::GetDataNode()
	{
		return m_pDataNode;
	}

	void				cEventDataNode::SetDataNode(TiXmlElement*e_pTiXmlElement)
	{
		SAFE_DELETE(m_pDataNode);
		m_pDataNode = (TiXmlElement*)e_pTiXmlElement->Clone();
		this->SetName(e_pTiXmlElement->Value());
	}

}