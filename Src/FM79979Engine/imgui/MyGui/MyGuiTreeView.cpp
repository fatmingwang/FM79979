#include "MyGuiTreeView.h"
#include "../misc/cpp/imgui_stdlib.h"
#include "../../Core/AllCoreInclude.h"
#include "../ThirtyParty/ImGuiBuilder/additional.h"
#include <iostream>
#include "../ImGuiRender.h"
#include <fstream>
#include <filesystem>
#include "MyGUIUtilities.h"

cMyTreeView::cMyTreeView()
{
	this->SetName(L"cMyTreeView");
	//SetImGuiName(cMyGuiForm::TypeID);
}

cMyTreeView::~cMyTreeView()
{
}

void cMyTreeView::DisplayTree(cImGuiNode* e_pNode)
{
	if (!e_pNode)
	{
		return;
	}
	auto l_strID = ValueToString(e_pNode->GetUniqueID());

	// Display the checkbox for visibility
	bool l_bVisible = e_pNode->IsVisible();
	ImGui::PushID(e_pNode); // Unique ID for the checkbox
	if (ImGui::Checkbox("", &l_bVisible))
	{
		e_pNode->SetVisible(l_bVisible);
	}
	ImGui::PopID();
	ImGui::SameLine();

	// Configure TreeNode flags
	bool hasChildren = !e_pNode->GetChildNodeVector().empty();
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (!hasChildren)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;
	}
	if (m_pCurrentSelectedNode == e_pNode)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	}
	if (m_SelectedRelatedNodeVector.size() && std::find(m_SelectedRelatedNodeVector.begin(), m_SelectedRelatedNodeVector.end(), e_pNode) != m_SelectedRelatedNodeVector.end())
	{
		if (m_pFocusNode == e_pNode)
		{
			nodeFlags |= ImGuiTreeNodeFlags_Selected;
			ImGui::SetScrollHereY();
			m_SelectedRelatedNodeVector.clear();
		}
		else
		{
			//nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
			ImGui::SetNextItemOpen(true);
		}
	}
	bool l_bNodeOpen = false;
	const char* l_strDragDropSourceID = "TREE_NODE";
	if (m_bDoRename && this->m_pCurrentSelectedNode == e_pNode)
	{
		l_bNodeOpen = false;
		char buffer[2048];
		strncpy(buffer, this->m_pCurrentSelectedNode->GetImGuiName().c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';

		// Render an input text for renaming
		if (ImGui::InputText("##Rename", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			this->m_pCurrentSelectedNode->SetImGuiName(buffer);
			m_bDoRename = false;
			m_pCurrentSelectedNode = nullptr;
		}


		// Automatically focus the input field when renaming starts
		if (ImGui::IsItemActive() == false)
		{
			ImGui::SetKeyboardFocusHere(-1);
		}

		// If the user clicks elsewhere or presses Esc, cancel renaming
		if (!ImGui::IsItemFocused() && ImGui::IsMouseClicked(0))
		{
			m_bDoRename = false;
			m_pCurrentSelectedNode = nullptr;
		}
	}
	else
	{
		l_bNodeOpen = ImGui::TreeNodeEx(l_strID.c_str(), nodeFlags, "%s", e_pNode->GetImGuiName().c_str());
		if (!m_bDoRename)
		{
			if (ImGui::IsItemClicked())
			{
				m_pSelectedNodeRect[0] = ImGui::GetItemRectMin();
				m_pSelectedNodeRect[1] = ImGui::GetItemRectMax();
				m_pCurrentSelectedNode = e_pNode;
				cGameApp::ShowInfoOnScreen(L"node Selected",0.5f);
				m_SelectedRelatedNodeVector.clear();
			}
			// Handle drag-and-drop source
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload(l_strDragDropSourceID, &e_pNode, sizeof(cImGuiNode*));
				ImGui::Text("Dragging %s", e_pNode->GetRenderText().c_str());
				ImGui::EndDragDropSource();
			}
		}
	}
	// Render the tree node children
	if (l_bNodeOpen)
	{
		auto& l_ChildrenVector = e_pNode->GetChildNodeVector();
		for (size_t i = 0; i <= l_ChildrenVector.size(); ++i)
		{
			if (l_ChildrenVector.size() > 0)
			{
				// Push a unique ID for each drop zone
				ImGui::PushID((int)i);

				// Render drop zone between nodes (or at the end)
				ImGui::Selectable("##DropZone", false, ImGuiSelectableFlags_AllowItemOverlap, ImVec2(0, 1));
			}
			// Highlight drop zone during drag
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(l_strDragDropSourceID))
				{
					cImGuiNode* draggedNode = *(cImGuiNode**)payload->Data;

					if (draggedNode != e_pNode)
					{
						// Remove dragged node from its original parent
						auto& parentChildren = draggedNode->GetParent()->GetChildNodeVector();
						// Insert dragged node at the current position
						int l_iIndex = (int)i;
						if (l_iIndex < 0)
						{
							l_iIndex = 0;
						}
						if (l_iIndex >= l_ChildrenVector.size())
						{
							l_iIndex = (int)l_ChildrenVector.size() - 1;
						}
						m_iDropIndex = l_iIndex;

						// Update drag-and-drop tracking pointers
						m_pDragNode = draggedNode;
						m_pDropParent = e_pNode;
					}
				}
				ImGui::EndDragDropTarget();
			}
			if (l_ChildrenVector.size() > 0)
			{
				ImGui::PopID(); // Pop the unique ID for the drop zone
			}

			// Render the child node
			if (i < l_ChildrenVector.size())
			{
				DisplayTree(l_ChildrenVector[i]);
			}
		}

		ImGui::TreePop();
	}
}

void cMyTreeView::RenderTreeivewPopupMenuContext()
{
	if (ImGui::BeginPopupContextWindow("bwcontextmenu"))
	{
		if (ImGui::MenuItem(m_strTreeViewMenuNameArray[eTVM_COPY]))
		{
			m_pCopyNode = this->m_pCurrentSelectedNode;
			cGameApp::ShowInfoOnScreen(L"Copy Node Selected...");
			//m_ToastMessage.ShowToast("Copy Node Selected...");
			if (this->m_fMenuCallbackFunction)
			{
				this->m_fMenuCallbackFunction(m_pCopyNode, m_strTreeViewMenuNameArray[eTVM_COPY]);
			}
		}
		if (ImGui::MenuItem(m_strTreeViewMenuNameArray[eTVM_PASTE]))
		{
			if (this->m_pCurrentSelectedNode)
			{
				if(this->m_pCopyNode)
				{
					cGameApp::ShowInfoOnScreen(L"Node Pasted from copied Node ");
					nlohmann::json json;
					this->m_pCopyNode->DoSerialize(json);
					{
						cImGuiNode*l_pNewNode = cImGuiNode::DoUnSerialize(json);
						m_pCurrentSelectedNode->AddChild(l_pNewNode);
					}
					m_pCopyNode = nullptr;
					m_pCutNode = nullptr;
				}
				else
				if (m_pCutNode && m_pCurrentSelectedNode != m_pCutNode)
				{
					m_pCutNode->SetParent(this->m_pCurrentSelectedNode);
					cGameApp::ShowInfoOnScreen(L"Node Pasted from cut node");
					m_pCopyNode = nullptr;
					m_pCutNode = nullptr;
				}
			}
			
			if (this->m_fMenuCallbackFunction)
			{
				this->m_fMenuCallbackFunction(m_pCopyNode, m_strTreeViewMenuNameArray[eTVM_PASTE]);
			}
		}
		if (ImGui::MenuItem(m_strTreeViewMenuNameArray[eTVM_CUT]))
		{
			if (this->m_fMenuCallbackFunction)
			{
				this->m_fMenuCallbackFunction(m_pCopyNode, m_strTreeViewMenuNameArray[eTVM_CUT]);
			}
			m_pCutNode = this->m_pCurrentSelectedNode;
			cGameApp::ShowInfoOnScreen(L"Cut node Selected");
		}
		if (ImGui::MenuItem(m_strTreeViewMenuNameArray[eTVM_DELETE]))
		{
			if (this->m_fMenuCallbackFunction)
			{
				this->m_fMenuCallbackFunction(m_pCurrentSelectedNode, m_strTreeViewMenuNameArray[eTVM_DELETE]);
			}
			if (this->m_pCurrentSelectedNode)
			{
				if (this->m_pCurrentSelectedNode->Type() != cMyGuiForm::TypeID)
				{
					this->m_pCurrentSelectedNode->SetParent(nullptr);
					SAFE_DELETE(this->m_pCurrentSelectedNode);
					cGameApp::ShowInfoOnScreen(L"Selected node Deleted");
				}
			}
			this->m_pCurrentSelectedNode = nullptr;
			m_pCopyNode = nullptr;
			m_pCutNode = nullptr;
		}
		ImGui::EndPopup();
	}
}

void cMyTreeView::Render()
{
	if (!m_pRoot)
	{
		return;
	}
	m_pDragNode = nullptr;
	m_pDropParent = nullptr;
	if (m_bAssignStartData)
	{
		ImGui::SetNextWindowPos(m_vPosition);
		//ImGui::SetNextWindowSize(ImVec2(l_iWidth, 500));
		ImGui::SetNextWindowSizeConstraints(m_vSize, ImVec2(FLT_MAX, 1080));
		m_bAssignStartData = false;
	}

	//ImVec2 minSize(400, 300);
	//ImVec2 maxSize(9999, 9999); // No maximum size constraint
	//ImGui::SetNextWindowSizeConstraints(minSize, maxSize);
	//if (ImGui::Begin("Tree View Window"), nullptr, m_iRenderFlag)
	if (ImGui::Begin("Tree View Window"))
	{

		// Set the size of the tree view region
		//ImVec2 treeViewSize = ImVec2(300, 400); // Width: 300px, Height: 400px
		ImVec2 treeViewSize = ImGui::GetContentRegionAvail();
		//m_bCollided = ImGui::IsWindowHovered();
		m_vPosition = ImGui::GetWindowPos();
		m_vSize = ImGui::GetWindowSize();
		this->m_bCollided = CheckMouseAndCurrentWindowCollision();
		// Begin a child region to contain the tree
		ImGui::BeginChild("TreeViewRegion", treeViewSize, true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);
		// Display the tree starting from the root node
		if (m_pRoot)
		{
			//if (this->m_SelectedRelatedNodeVector.size())
			//{
			//	this->DisplayTreeWithFocus(m_pRoot);
			//}
			//else
			{
				this->DisplayTree(m_pRoot);
			}
			
		}
		this->RenderTreeivewPopupMenuContext();
		// End the child region
		ImGui::EndChild();
	}
	ImGui::End();

	if (m_pDragNode && m_pDropParent)
	{
		m_pDragNode->SetParent(m_pDropParent, m_iDropIndex);
		m_pCurrentSelectedNode = nullptr;
	}
	if (m_pCurrentSelectedNode && ImGui::IsKeyPressed(ImGuiKey_F2))
	{
		auto l_MousePos = cGameApp::m_sMousePosition;
		Vector4 l_vRect(m_pSelectedNodeRect[0].x, m_pSelectedNodeRect[0].y, m_pSelectedNodeRect[1].x, m_pSelectedNodeRect[1].y);
		if (l_vRect.CollidePoint((int)l_MousePos.x, (int)l_MousePos.y))
			//if (ImGui::IsMouseHoveringRect(m_pSelectedNodeRect[0], m_pSelectedNodeRect[1]))
		{
			m_bDoRename = true;
		}
	}
	m_ToastMessage.Render();
}

bool cMyTreeView::IsCollided(int e_iPosX, int e_iPosY)
{
	if (m_bCollided)
	{
		int a = 0;
	}
	return m_bCollided;
	Vector4 l_vRect(m_vPosition.x, m_vPosition.y, m_vPosition.x + m_vSize.x, m_vPosition.y + m_vSize.y);
	auto l_bResult = l_vRect.CollidePoint(e_iPosX, e_iPosY);
	if (l_bResult)
	{
		int a = 0;
	}
	return l_bResult;
}

void cMyTreeView::SetFocusNode(cImGuiNode* e_pNode)
{
	if (m_pRoot)
	{
		m_pCurrentSelectedNode = nullptr;
		m_pFocusNode = nullptr;
		m_SelectedRelatedNodeVector.clear();
		auto l_Node = m_pRoot->FindNodeByUID(e_pNode->m_pData->m_iID,&m_SelectedRelatedNodeVector);
		if (l_Node)
		{
			m_SelectedRelatedNodeVector.push_back(m_pRoot);
			std::reverse(m_SelectedRelatedNodeVector.begin(), m_SelectedRelatedNodeVector.end());
			m_pFocusNode = e_pNode;
			int l_iID = l_Node->m_pData->m_iID;
		}
	}
}

void cMyTreeView::SerRootNode(cImGuiNode* e_pImGuiNode)
{
	if (m_pRoot != e_pImGuiNode)
	{
		m_pRoot = e_pImGuiNode;
		m_bDoRename = false;
		m_iDropIndex = -1;
		m_bAssignStartData = true;
		m_bCollided = false;
		m_pCopyNode = nullptr;
		m_pCutNode = nullptr;
		m_pDragNode = nullptr;
		m_pDropParent = nullptr;
		m_pCurrentSelectedNode = nullptr;
		m_pFocusNode = nullptr;
	}
}
