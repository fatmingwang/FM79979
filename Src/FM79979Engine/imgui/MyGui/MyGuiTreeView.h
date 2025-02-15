#pragma once
#include "MyGui.h"
#include "MyGUIUtilities.h"
class cMyTreeView :public NamedTypedObject
{
	sToastMessage m_ToastMessage;
	std::vector<cImGuiNode*>m_SelectedRelatedNodeVector;
	cImGuiNode* m_pRoot = nullptr;
public:
	enum eTreeViewMenu
	{
		eTVM_COPY = 0,
		eTVM_PASTE,
		eTVM_CUT,
		eTVM_DELETE,
		eTVM_MAX
	};
	const char* m_strTreeViewMenuNameArray[eTreeViewMenu::eTVM_MAX] = 
	{
		"Copy",
		"Paste",
		"Cut",
		"Delete",
	};
protected:
	cImGuiNode* m_pCopyNode = nullptr;
	cImGuiNode* m_pCutNode = nullptr;
	cImGuiNode* m_pDragNode = nullptr;
	cImGuiNode* m_pDropParent = nullptr;
	cImGuiNode* m_pCurrentSelectedNode = nullptr;
	cImGuiNode* m_pFocusNode = nullptr;

	ImVec2		m_pSelectedNodeRect[2];
	bool		m_bDoRename = false;
	int			m_iDropIndex = -1;
	void		RenderTreeivewPopupMenuContext();
	void		DisplayTree(cImGuiNode* e_pNode);
	bool		m_bAssignStartData = true;
	bool		m_bCollided = false;
	public:
	int m_iRenderFlag = ImGuiWindowFlags_NoTitleBar;
	ImVec2		m_vSize = ImVec2(350, 1000);
	ImVec2		m_vPosition = ImVec2(1600, 0);
	cMyTreeView();
	virtual ~cMyTreeView();
	void			Render();
	bool			IsCollided(int e_iPosX, int e_iPosY);
	cImGuiNode* GetSelectedNode()
	{
		return m_pCurrentSelectedNode;
	}
	void SetSelectedNodeNull()
	{
		m_pCurrentSelectedNode = nullptr;
	}
	void	SetFocusNode(cImGuiNode* e_pNode);
	std::function<void(cImGuiNode*, const char*)>m_fMenuCallbackFunction;
public:
	void	SerRootNode(cImGuiNode*e_pImGuiNode);
};
