#ifndef _EVENT_DATA_NODE_H_
#define _EVENT_DATA_NODE_H_

namespace FATMING_CORE
{
	class	cEventDataNode:public NamedTypedObject
	{
		TiXmlElement*	m_pDataNode;
	public:
		cEventDataNode();
		virtual ~cEventDataNode();
		TiXmlElement*		GetDataNode();
		void				SetDataNode(TiXmlElement*e_pTiXmlElement);
	};
//end namespace
}
#endif