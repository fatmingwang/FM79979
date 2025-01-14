#pragma once
#include "MyGui.h"

class cMyTreeView :public NamedTypedObject
{
	cImGuiNode* m_pCopyNode = nullptr;
	cImGuiNode* m_pDragNode = nullptr;
	cImGuiNode* m_pDropParent = nullptr;
	cImGuiNode* m_pSelectedNode = nullptr;

	ImVec2		m_pSelectedNodeRect[2];
	bool		m_bDoRename = false;
	int			m_iDropIndex = -1;
	void		RenderTreeivewPopupMenuContext();
	void		DisplayTree(cImGuiNode* e_pNode, bool e_bRenderVisibleCheckBox);
	bool		m_bAssignStartData = true;
	bool		m_bCollided = false;
	public:
	int m_iRenderFlag = ImGuiWindowFlags_NoTitleBar;
	cImGuiNode* m_pRoot = nullptr;
	ImVec2		m_vSize = ImVec2(350, 1000);
	ImVec2		m_vPosition = ImVec2(1600, 0);
	cMyTreeView();
	virtual ~cMyTreeView();
	void			Render();
	bool			IsCollided(int e_iPosX, int e_iPosY);
	cImGuiNode* GetSelectedNode()
	{
		return m_pSelectedNode;
	}
	void SetSelectedNodeNull()
	{
		m_pSelectedNode = nullptr;
	}
};
