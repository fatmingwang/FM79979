#pragma once
#ifdef WIN32
#include "./MyGui.h"
#include "../../../include/json.hpp"

struct sMouseCursor;

//from ImGuiBuilder
//https://github.com/Code-Building/ImGuiBuilder

namespace FATMING_CORE
{
	class  cOrthogonalCamera;
}

class cImGuiNode;

class cMyImGuiUIEditor
{
	int								m_iToolboxRenderContextIndex = 1;
	int								m_iRootNodeRenderContextIndex = 0;
	class cMyGuiBasicObj*			m_pToolBoxRoot = nullptr;
	class cMyGuiRootNode*			m_pMainUIRoot = nullptr;
	class cMyGuiForm*				m_pMyGuiForm = nullptr;
	class cMyTreeView*				m_pTreeView = nullptr;
	void							GenerateToolBox();
	void							RenderMainUI();
	void							RenderToolBox();
	void							RenderMenu(class cImGuiNode*);
	void							RenderPopupMenuContext();
	void							RenderTreeivewPopupMenuContext();

	//int								m_uiUniqueID = 0;
	//int								GetUniqueID();
	enum eFILE_DIALOG_RESULT
	{
		eFDR_NONE = 0,
		eFDR_UI_LAYOUT_SAVE,
		eFDR_UI_LAYOUT_OPEN,
		eFDR_GENERATE_CODE,
		eFDR_COLOR_SAVE,
		eFDR_COLOR_OPEN,
		eFDR_FLAG_SAVE,
		eFDR_FLAG_OPEN,
		eFDR_MAX
	};
	void move_item(ImVec2& obj_pos, bool& continue_edt);
	//
	bool					m_MovingObj = false;
	bool					m_ResizeObj = false;
	sMouseCursor*			m_pMouseCursor;
	void					RenderFileDoalog();
	void					RenderMenu();
	void					RenderDebugInfo();
	void					Render1ToolBox();
	ImGuiStyle				m_dark_style = { };
	ImGuiStyle				m_custom_gui_style = { };
	//
	cImGuiNode*				m_pCollidedItem = nullptr;
	Vector2					m_vMouseClickPos;
	ImVec2					m_vObjectLocalPosition;
public:
	cMyImGuiUIEditor();
	virtual ~cMyImGuiUIEditor();
	void			Init();
	void			Render();
	void			Update(float e_fElpaeeTime);
	class  cOrthogonalCamera* m_p2DCamera = nullptr;

	void			SaveToFile(const char* e_strFileName);
	void			OpenFile(const char* e_strFileName);

};


/// <returns></returns>
bool		KeyPressed(int e_iK);
bool		HoldingKey(int key);
bool		PressedBindingKeys(int key_1, int key_2);
resize_opt	GetResizeOptWithObjectSizeAndPos(ImVec2 obj_pos, ImVec2 obj_size, float thickness = 3.f);
bool		IsItemHovered(ImVec2 obj_pos, ImVec2 obj_size, float distance);
bool		IsCollides(cImGuiNode* e_pNode, ImVec2 e_vPoint);
#endif




//// Structure to hold our tree item data
//struct TreeNodeData
//{
//	const char* name;
//	bool is_dragging = false;
//};
//
//// Array of tree nodes
//TreeNodeData treeNodes[] =
//{
//	{ "Node 1" },
//	{ "Node 2" },
//	{ "Node 3" }
//};
//
//void	TreeNodeDragDropTest()
//{
//	// Root tree node
//	if (ImGui::TreeNode("Root Node"))
//	{
//		for (int i = 0; i < IM_ARRAYSIZE(treeNodes); i++)
//		{
//			TreeNodeData& node = treeNodes[i];
//
//			// Create a tree node for each item
//			if (ImGui::TreeNode(node.name))
//			{
//
//				// Drag Source
//				if (ImGui::BeginDragDropSource())
//				{
//					ImGui::SetDragDropPayload("DND_DEMO", &i, sizeof(int));  // Set payload to index
//					ImGui::Text("Dragging %s", node.name);
//					node.is_dragging = true;
//					ImGui::EndDragDropSource();
//				}
//				else
//				{
//					node.is_dragging = false;
//				}
//
//				// Drag Target
//				if (ImGui::BeginDragDropTarget())
//				{
//					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO"))
//					{
//						IM_ASSERT(payload->DataSize == sizeof(int));
//						int sourceIndex = *(const int*)payload->Data;
//						std::swap(treeNodes[i], treeNodes[sourceIndex]);  // Swap nodes on drop
//					}
//					ImGui::EndDragDropTarget();
//				}
//				ImGui::TreePop();
//			}
//		}
//		ImGui::TreePop();
//	}
//}
//
//
////// Helper function to display JSON in ImGui
////void RenderJson(const nlohmann::json& jsonData, const std::string& name = "root") 
////{
////	if (jsonData.is_object()) {
////		// JSON Object (Dictionary)
////		if (ImGui::TreeNode(name.c_str())) {
////			for (auto& item : jsonData.items()) {
////				RenderJson(item.value(), item.key());
////			}
////			ImGui::TreePop();
////		}
////	}
////	else if (jsonData.is_array()) {
////		// JSON Array
////		if (ImGui::TreeNode((name + " (array)").c_str())) {
////			for (size_t i = 0; i < jsonData.size(); ++i) {
////				RenderJson(jsonData[i], "[" + std::to_string(i) + "]");
////			}
////			ImGui::TreePop();
////		}
////	}
////	else {
////		// JSON Value (String, Number, Boolean, etc.)
////		ImGui::Text("%s: %s", name.c_str(), jsonData.dump().c_str());
////	}
////}