#include "StdAfx.h"
#include "TreeNodeInfo.h"


cTreeNodeInfo::~cTreeNodeInfo()
{

}

void	cTreeNodeInfo::KillAllNode( cTreeNodeInfo*e_pNode )
{
	//child
	if( e_pNode->m_TreeNodeList[0] )
	{
		KillAllNode(e_pNode->m_TreeNodeList[0]);
	}
	//next
	if( e_pNode->m_pFatherNode )
	{

		KillAllNode(this->m_TreeNodeList[0]);
	}	
}

cTreeNodeInfo*	cTreeNodeInfo::SearchSelfTreeNode(WCHAR*e_pString)
{
	for each(cTreeNodeInfo*l_pNode in m_TreeNodeList)
	{
		if( !_wcsicmp(l_pNode->m_sName,e_pString) )
			return l_pNode;
	}
	return 0;
}

cTreeNodeInfo*	cTreeNodeInfo::SearchAllTreeNode(WCHAR*e_pString,...)
{
	WCHAR*	l_pString = e_pString;
    va_list marker;
    va_start( marker, e_pString );     /* Initialize variable arguments. */
	while(e_pString)
	{			
		if( !SearchSelfTreeNode(l_pString) )
			return 0;
		l_pString = va_arg( marker, WCHAR*);
	}
	va_end( marker );              /* Reset variable arguments.      */
	return SearchSelfTreeNode(l_pString);
}