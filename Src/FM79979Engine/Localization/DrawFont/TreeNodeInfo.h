#pragma once

class cTreeNodeInfo
{
	WCHAR	m_sName[MAX_PATH];
	std::vector<cTreeNodeInfo*>	m_TreeNodeList;
	cTreeNodeInfo*	m_pFatherNode;
	cTreeNodeInfo*	m_pNodeSlibing;
	void	KillAllNode( cTreeNodeInfo*e_pNode );

public:
	cTreeNodeInfo(WCHAR*e_pString,cTreeNodeInfo*e_pTreeNodeInfoFather = 0){ assert(e_pString); wcscpy_s(m_sName,MAX_PATH,e_pString); m_pFatherNode = e_pTreeNodeInfoFather; }
	~cTreeNodeInfo();
	cTreeNodeInfo*	SearchSelfTreeNode(WCHAR*e_pString);
	cTreeNodeInfo*	SearchAllTreeNode(WCHAR*e_pString,...);
	cTreeNodeInfo	*GetFather(){ return m_pFatherNode; }
	inline cTreeNodeInfo* operator [](WCHAR*e_pString){ return SearchSelfTreeNode(e_pString); }
};