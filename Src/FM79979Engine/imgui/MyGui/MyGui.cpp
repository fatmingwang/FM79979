#include "MyGui.h"
#include "../misc/cpp/imgui_stdlib.h"
#include "../../Core/AllCoreInclude.h"
#include "../ThirtyParty/ImGuiBuilder/additional.h"
#include <iostream>

using namespace	FATMING_CORE;
TYPDE_DEFINE_MARCO(cImGuiNode);
TYPDE_DEFINE_MARCO(cMyGuiRootNode);
TYPDE_DEFINE_MARCO(cMyGuiButton);
TYPDE_DEFINE_MARCO(cMyGuiLabel);
TYPDE_DEFINE_MARCO(cMyGuiEditBox);
TYPDE_DEFINE_MARCO(cMyGuiSliderInteger);
TYPDE_DEFINE_MARCO(cMyGuiSliderFloatValue);
TYPDE_DEFINE_MARCO(cMyGuiCheckBox);
TYPDE_DEFINE_MARCO(cMyGuiRadio);
TYPDE_DEFINE_MARCO(cMyGuiToogle);
TYPDE_DEFINE_MARCO(cMyGuiForm);
TYPDE_DEFINE_MARCO(cMyGuiPanel);
TYPDE_DEFINE_MARCO(cMyGuiComboBox);
TYPDE_DEFINE_MARCO(cMyGuiListBox);



//void	cImGuiNode::ApplySize(bool& e_bWidth, bool& e_bHeight)
//{
//	if (this->m_vSize.x >= 0)
//	{
//		ImGui::PushItemWidth(this->m_vSize.x);
//	}
//}

cImGuiNode::cImGuiNode()
{
	m_pParent = nullptr;
	m_bEnable = true;
	m_bVisible = true;
	m_bDoApplyPosition = true;
	m_bOnlyApplyPositionOnceForDragMoving = false;
	this->SetName(this->Type());
}

cImGuiNode::~cImGuiNode()
{
	SetParent(nullptr);
	//DeleteObjectAndAllChildren(this);
	DestoryWithChildren();
}

void cImGuiNode::DestoryWithChildren()
{
	auto l_uiSize = m_ChildNodeVector.size();
	std::vector<cImGuiNode*>	l_ChildNodeVector = m_ChildNodeVector;
	for (size_t i = 0; i < l_uiSize; i++)
	{
		l_ChildNodeVector[i]->SetParent(nullptr);
	}
	for (size_t i = 0; i < l_uiSize; i++)
	{
		SAFE_DELETE(l_ChildNodeVector[i]);
	}
}

void cImGuiNode::HierachyPositionRender()
{
	static ImVec2 l_vsMaxWindowSize(9999, 9999);
	ImVec2 l_vPos = this->GetLocalPosition();
	//auto l_pParent = this->GetParent();
	//if (!l_pParent)
	//{
	//	ImGui::SetNextWindowPos(l_vPos);
	//}
	//else
	{
		ImGui::SetCursorPos(l_vPos);
		ImGui::BeginChild("", l_vsMaxWindowSize, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	}
}

void cImGuiNode::EndRender()
{
}

void cImGuiNode::SetCachedWorldTransformDirty()
{
	if (!m_bPosDirty)
	{
		m_bPosDirty = true;
		for (cImGuiNode *l_ImGuiNode: m_ChildNodeVector)
		{
			l_ImGuiNode->SetCachedWorldTransformDirty();
		}
	}
}

void cImGuiNode::UpdateCachedWorldTransformIfNeeded()
{
	if (m_bPosDirty)
	{
		if (m_pParent)
		{
			m_pParent->UpdateCachedWorldTransformIfNeeded();
			m_vWorldPos = ImVec2(m_pParent->m_vWorldPos.x + m_vLocalPos.x, m_pParent->m_vWorldPos.y + m_vLocalPos.y);
			if (m_pParent->m_bThisUseContainerPositionDontApplyarentPositionToChild)
			{
				m_vImGuiRenderPos = m_vLocalPos;
			}
			else
			{
				m_vImGuiRenderPos = m_vWorldPos;
			}
		}
		else
		{
			m_vWorldPos = m_vLocalPos;
		}
		m_bPosDirty = false;
	}
}


void cImGuiNode::SetLocalPosition(const ImVec2& e_vLocalPos)
{
	if (m_vLocalPos.x != e_vLocalPos.x || m_vLocalPos.y != e_vLocalPos.y)
	{
		m_vLocalPos = e_vLocalPos;
		m_bPosDirty = false;
		SetCachedWorldTransformDirty();
	}
	m_bDoApplyPosition = true;
}

void cImGuiNode::SetLocalPosition(float e_fPosX, float e_fPosY)
{
	SetLocalPosition(ImVec2(e_fPosX,e_fPosY));
}

ImVec2 cImGuiNode::GetWorldPosition()
{
	UpdateCachedWorldTransformIfNeeded();
	return m_vWorldPos;
}

void cImGuiNode::SetWorldPosition(const ImVec2& e_vWorldPos)
{
	if (m_pParent)
	{
		ImVec2 l_vParentPosition = m_pParent->GetWorldPosition();
		ImVec2 l_vNewLocalPosition(e_vWorldPos.x - l_vParentPosition.x, e_vWorldPos.y - l_vParentPosition.y);
		this->SetLocalPosition(l_vNewLocalPosition);
	}
	else
	{
		this->SetLocalPosition(e_vWorldPos);
	}
}

ImVec2 cImGuiNode::GetWorldImGuiRenderPosition()
{
	GetWorldPosition();
	return m_vImGuiRenderPos;
}

void cImGuiNode::SetParent(cImGuiNode* e_pParent, int e_iChildIndex)
{
	if (this->m_pParent)
	{
		size_t l_uiSize = this->m_pParent->m_ChildNodeVector.size();
		for (size_t i = 0; i < l_uiSize; i++)
		{
			if (this->m_pParent->m_ChildNodeVector[i]->GetUniqueID() == this->GetUniqueID())
			{
				this->m_pParent->m_ChildNodeVector.erase(this->m_pParent->m_ChildNodeVector.begin() + i);
				break;
			}
		}
	}
	this->m_pParent = e_pParent;
	this->m_bPosDirty = true;
	if (this->m_pParent)
	{
		if (e_iChildIndex == -1)
		{
			this->m_pParent->m_ChildNodeVector.push_back(this);
		}
		else
		{
			this->m_pParent->m_ChildNodeVector.insert(this->m_pParent->m_ChildNodeVector.begin() + e_iChildIndex, this);
		}
		
	}
}

void cImGuiNode::AddChild(cImGuiNode* e_pChild, int e_iChildIndex)
{
	e_pChild->SetParent(this, e_iChildIndex);
}

bool cImGuiNode::SwapChild(int e_iIndex1, int e_iIndex2)
{
	if (e_iIndex2 < m_ChildNodeVector.size() && e_iIndex1 >=0 && e_iIndex1 != e_iIndex2)
	{
		auto l_pChild1 = m_ChildNodeVector[e_iIndex1];
		auto l_pChild2 = m_ChildNodeVector[e_iIndex2];
		m_ChildNodeVector[e_iIndex1] = l_pChild2;
		m_ChildNodeVector[e_iIndex2] = l_pChild1;
		return true;
	}
	return false;
}

void cImGuiNode::Render()
{
	if (!m_bVisible)
	{
		return;
	}
	if (this->m_bDoApplyPosition)
	{
		if (this->m_bOnlyApplyPositionOnceForDragMoving)
		{
			this->m_bDoApplyPosition = false;
		}
		ApplyPosition();
	}
	if (!m_bEnable)
	{
		ImGui::BeginDisabled();
	}
	this->InternalRender();
	if (!m_bEnable)
	{
		ImGui::EndDisabled();
	}
	if (this->m_ExtraRenderFunction)
	{
		this->m_ExtraRenderFunction(this);
	}
	for (auto l_Child : this->m_ChildNodeVector)
	{
		l_Child->Render();
	}
	EndRender();
	bool l_bRenderBorder = false;
	if (l_bRenderBorder)
	{
		//ImGui::GetWindowDrawList()->AddRect(vMin, vMax, col, 0.f, 15, 0.01f);
	}
}

void cImGuiNode::DeleteObjectAndAllChildren(cImGuiNode* e_pImGuiNode)
{
	if (e_pImGuiNode)
	{
		auto l_uiSize = e_pImGuiNode->m_ChildNodeVector.size();
		for (size_t i = 0; i < l_uiSize; i++)
		{
			DeleteObjectAndAllChildren(e_pImGuiNode->m_ChildNodeVector[i]);
		}
		delete e_pImGuiNode;
	}
}

void cMyGuiBasicObj::ApplyPosition()
{
	auto l_vPos = GetWorldImGuiRenderPosition();
	ImGui::SetCursorPos(l_vPos);
	//ImGui::SetCursorScreenPos(this->GetWorldPosition());
	
	//ImGui::SetCursorScreenPos(this->m_vLocalPos);
	//ImGui::SetCursorPos(this->m_vLocalPos);
}

void cMyGuiBasicObj::RenderBaseProperty()
{
	ImGui::SetNextWindowPos({ 0, (float)500 });
	ImGui::SetNextWindowSize({ 300, 700 - 100 });
	ImGui::Begin("property", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus);
		bool l_my_forms_active = ImGui::IsWindowFocused();
		std::string l_strName = this->m_strName;
		ImGui::InputTextEx("Name form", &l_strName, 0);
		//ImGui::InputText("name form", name, 255);
		if (ImGui::Button("Apply name"))
		{
			if (!this->m_strName.empty())
			{
				this->m_strName = l_strName;
			}
		}
		//ImVec2		l_vSize = m_vSize;
		ImVec2		l_vPos = GetLocalPosition();
		//ImVec2		l_vSizeObj = m_vSizeObj;
		ImGui::InputFloat("SizeX", &m_vSize.x, 1);
		ImGui::InputFloat("SizeY", &m_vSize.y, 1);
		ImGui::InputFloat("PosX", &l_vPos.x, 1);
		ImGui::InputFloat("PosY", &l_vPos.y, 1);
		if (m_vLocalPos.x!= l_vPos.x || m_vLocalPos.y != l_vPos.y)
		{
			this->SetLocalPosition(l_vPos);
		}
		bool l_bBorder = false;;
		bool l_bLock = false;;
		ImGui::Checkbox("Border", &l_bBorder);
		ImGui::SameLine();
		ImGui::Checkbox("Lock", &l_bLock);
	ImGui::End();

}

cMyGuiBasicObj::cMyGuiBasicObj()
{
	m_vSize.y = m_vSize.x = 0;
}

cMyGuiBasicObj::~cMyGuiBasicObj()
{
}

void cMyGuiBasicObj::RenderProperty()
{
}

float relative_for_resize(cMyGuiBasicObj & obj)
{
	//auto& g = *GImGui;
	//auto* window = g.CurrentWindow;
	//const auto& style = g.Style;
	//const auto	id = window->GetID(obj.name.c_str());
	//const auto	label_size = ImGui::CalcTextSize(obj.name.c_str(), nullptr, true);
	//const auto	frame_size = ImGui::CalcItemSize(ImVec2(0, 0), ImGui::CalcItemWidth(), (label_size.y) + style.FramePadding.y * 2.0f);
	//const auto	label_dif = (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f);
	//if (obj.size.x == 0.f && obj.size.y == 0.f)
	//	obj.size = ImVec2(frame_size.x + label_dif, frame_size.y);
	//return obj.size.x - label_dif;
	return 0.f;
};

cMyGuiPanel::cMyGuiPanel()
{
	this->SetName(cMyGuiPanel::TypeID);
	this->m_vSize = ImVec2(200, 200);
	this->m_bShowBorder = true;
	this->m_bThisUseContainerPositionDontApplyarentPositionToChild = true;
}

cMyGuiPanel::~cMyGuiPanel()
{
}

void cMyGuiPanel::ApplyPosition()
{
	cMyGuiBasicObj::ApplyPosition();
	//void cMyGuiBasicObj::ApplyPosition()
	{
		//ImGui::SetCursorPos(this->m_vLocalPos);
	}
	//auto l_vPos = GetWorldPosition();
	//ImGui::SetNextWindowPos({ l_vPos.x, l_vPos.y });
}

void cMyGuiPanel::InternalRender()
{
	ImGui::BeginChild(this->GetCharName().c_str(), this->m_vSize,true);
	// Move the cursor to the desired position for the child
	//ImGui::SetCursorScreenPos(childPos);
	//cMyGuiBasicObj::ApplyPosition();
}

void cMyGuiPanel::EndRender()
{
	ImGui::EndChild();
}

void cMyGuiPanel::RenderProperty()
{
}

void cMyGuiForm::ApplyPosition()
{
	auto l_vPos = GetWorldPosition();
	ImGui::SetNextWindowPos({ l_vPos.x, l_vPos.y });
}

void cMyGuiForm::InternalRender()
{
	if (m_vSize.x > 0 && m_vSize.y > 0)
	{
		ImGui::SetWindowSize(m_vSize);
	}
	//ImGui::Begin("BUILDER", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
	ImGui::Begin(this->GetCharName().c_str(),nullptr, m_FormFlag);
	//form is draggable
	if (!this->m_bDoApplyPosition)
	{
		ImVec2 l_CurrentFormPos = ImGui::GetWindowPos();
		//ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar
		if (m_FormFlag & ImGuiWindowFlags_NoBringToFrontOnFocus)
		{
			auto l_vFormPos = this->GetWorldPosition();
			if (l_vFormPos.x != l_CurrentFormPos.x || l_vFormPos.y != l_CurrentFormPos.y)
			{
				SetWorldPosition(l_CurrentFormPos);
			}
		}
	}
}

void cMyGuiForm::EndRender()
{
	ImGui::End();
}

cMyGuiForm::cMyGuiForm()
{
	this->SetName(cMyGuiForm::TypeID);
	m_FormFlag = 0;
	this->m_bThisUseContainerPositionDontApplyarentPositionToChild = true;
}

cMyGuiForm::~cMyGuiForm()
{
}

void cMyGuiForm::RenderProperty()
{
}

void cMyGuiToogle::InternalRender()
{
	//ImGui::ToggleButton(obj.name.c_str(), &true_bool);
}

void cMyGuiToogle::RenderProperty()
{
}

void cMyGuiRadio::InternalRender()
{
	//ImGui::RadioButton(obj.name.c_str(), true_bool);
}

void cMyGuiCheckBox::InternalRender()
{
	//ImGui::Checkbox(obj.name.c_str(), &true_bool);
}

void cMyGuiSliderFloatValue::InternalRender()
{
	//ImGui::PushItemWidth(relative_for_resize(obj));
	//ImGui::SliderFloat(obj.name.c_str(), &value_f, 0, 100);
	//ImGui::PopItemWidth();
}

void cMyGuiSliderInteger::InternalRender()
{
	//ImGui::PushItemWidth(relative_for_resize(obj));
	//ImGui::SliderInt(obj.name.c_str(), &value_i, 0, 100);
	//ImGui::PopItemWidth();
}

cMyGuiEditBox::cMyGuiEditBox()
{
	m_bMultiLines = false;
	m_strHint = "please input...";
}

void cMyGuiEditBox::InternalRender()
{
	this->m_vSize.x = 100.f;
	if (this->m_vSize.x > 0)
	{
		ImGui::PushItemWidth(this->m_vSize.x);
	}
	if (m_bMultiLines)
	{
		RenderMultiLine();
	}
	else
	{
		if (m_strHint.length())
		{
			const char* hint = "Enter your text here..."; // Hint text
			ImGui::InputTextWithHint("##hint", hint, &m_strText);
		}
		else
		{

			ImGui::InputText("Input", &m_strText);
		}
	}
	//ImGui::Text("Current Text:");
	//ImGui::TextWrapped("%s", text.c_str()); // Display the current text
	if (this->m_vSize.x > 0)
	{
		ImGui::PopItemWidth();
	}
}

void cMyGuiEditBox::RenderMultiLine()
{
	//if (m_strHint.length() && this->m_strText.length() == 0)
	//{
	//	size_t l_iLength = strlen(this->m_strText.c_str());
	//	if (l_iLength == 0)
	//	{
	//		// Render the hint
	//		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)); // Use a "disabled" color
	//		ImGui::TextUnformatted(m_strHint.c_str());
	//		ImGui::PopStyleColor();

	//		// Overlay the hint text on the input field
	//		ImGui::SetItemAllowOverlap();
	//	}
	//}
	// Render the multi-line input text box
	if (ImGui::InputTextMultiline("##multiline", &this->m_strText, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 10), ImGuiInputTextFlags_AllowTabInput))
	{
		//if (l_iLength*2 >= l_iCapacity)
		//{
		//	m_strText.reserve(l_iCapacity * 2); // Double the buffer size
		//	m_strText.resize(strlen(m_strText.c_str()));
		//}
	}


	//// Multi-line input text box
	//static std::string text = "This is a multi-line text box.\nYou can edit this text.\nFeel free to type more lines.";
	//ImGui::InputTextMultiline("##multiline",
	//						  &text[0],
	//						  text.capacity() + 1,
	//						  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 10),
	//						  ImGuiInputTextFlags_AllowTabInput);
}


void cMyGuiLabel::InternalRender()
{
	//ImGui::Text(obj.name.c_str());
}


cMyGuiButton::cMyGuiButton()
{
	m_strText = "Button";
}

cMyGuiButton::~cMyGuiButton()
{
}

void cMyGuiButton::InternalRender()
{
	if (ImGui::Button(m_strText.c_str(), this->m_vSize))
	{
		if (m_fOnClickFunction)
		{
			m_fOnClickFunction();
		}
	}
	//ImGui::Button(obj.name.c_str(), obj.size);
}
//please us epanel and set border false to retend it's a  node
void cMyGuiRootNode::ApplyPosition()
{
	//ImGui::SetNextWindowPos(this->m_vLocalPos);
}

void cMyGuiRootNode::InternalRender()
{
}

void ShowTreeViewWindow(cImGuiNode* rootNode)
{
	cImGuiNode* l_pDragNode = nullptr;
	cImGuiNode* l_pDropParent = nullptr;
	ImVec2 minSize(400, 300);
	ImVec2 maxSize(FLT_MAX, FLT_MAX); // No maximum size constraint
	ImGui::SetNextWindowSizeConstraints(minSize, maxSize);
	if (ImGui::Begin("Tree View Window"))
	{
		// Set the size of the tree view region
		//ImVec2 treeViewSize = ImVec2(300, 400); // Width: 300px, Height: 400px
		ImVec2 treeViewSize = ImGui::GetContentRegionAvail();

		// Begin a child region to contain the tree
		ImGui::BeginChild("TreeViewRegion", treeViewSize, true, ImGuiWindowFlags_HorizontalScrollbar);

		// Display the tree starting from the root node
		if (rootNode)
		{
			DisplayTree(rootNode, &l_pDragNode, &l_pDropParent);
		}

		// End the child region
		ImGui::EndChild();
	}
	ImGui::End();
	if (l_pDropParent && l_pDragNode)
	{
		l_pDragNode->SetParent(l_pDropParent);
	}
}

void cMyGuiRootNode::EndRender()
{
	if (m_bShowYesNoDialog)
	{
		CallYesNoDialog(m_CompleteFunction, m_strDialogMessage.c_str(), m_strYesButtonText.c_str(), m_strNoButtonText.c_str());
	}
	if (m_bShowConfirmDialog)
	{
		CallConfirmDialog(m_CompleteFunction, m_strDialogMessage.c_str(), m_strYesButtonText.c_str());
	}
	if (m_bShowFullScreenBlackText)
	{
		CallFullScreenBlackText(m_strDialogMessage.c_str());
	}

	ShowTreeViewWindow(this);
	this->m_bVisible = true;
}

void cMyGuiRootNode::ShowYesNoDialog(std::function<void(bool)> e_CompleteFunction, const char* e_strContent, const char* e_strYesButtonText, const char* e_strNoButtonText)
{
	m_strDialogMessage = e_strContent;
	m_strYesButtonText = e_strYesButtonText;
	m_strNoButtonText = e_strNoButtonText;
	m_CompleteFunction = e_CompleteFunction;
	m_bShowYesNoDialog = true;
	m_CompleteFunction = [e_CompleteFunction,this](bool e_bResult)
	{
		if (e_CompleteFunction)
		{
			e_CompleteFunction(e_bResult);
		}
		m_bShowYesNoDialog = false;
	};
}

void cMyGuiRootNode::ShowConfirmDialog(const char* e_strContent, const char* e_strConfirmButtonText, std::function<void(bool)> e_CompleteFunction)
{
	m_bShowConfirmDialog = true;
	m_strDialogMessage = e_strContent;
	m_strYesButtonText = e_strConfirmButtonText;
	m_CompleteFunction = [e_CompleteFunction, this](bool e_bResult)
	{
		if (e_CompleteFunction)
		{
			e_CompleteFunction(e_bResult);
		}
		m_bShowConfirmDialog = false;
	};
}

void cMyGuiRootNode::ShowFullScreenBlackText(const char* e_strContent)
{
	if (e_strContent == nullptr)
	{
		m_bShowFullScreenBlackText = false;
	}
	else
	{
		m_bShowFullScreenBlackText = true;
		this->m_strDialogMessage = e_strContent;
	}
}

void cMyGuiComboBox::InternalRender()
{
	if (this->m_vSize.x > 0)
	{
		ImGui::PushItemWidth(this->m_vSize.x);
	}
	const char* l_strTemp[999];
	for (int i = 0; i < m_strDataVector.size(); ++i)
	{
		l_strTemp[i] = m_strDataVector[i].c_str();
	}
	//m_strEvnNameVector = { "Option 1", "Option 2", "Option 3", "Option 4", "Option 5" };
	// Create the combo box
	if (ImGui::Combo("My ComboBox", &m_iSelectedIndex, l_strTemp,(int)m_strDataVector.size()))
	{
		if (m_fOnSelectFunction)
		{
			m_fOnSelectFunction(m_iSelectedIndex);
		}
		
	}
	if (this->m_vSize.x > 0)
	{
		ImGui::PopItemWidth();
	}
}

cMyGuiComboBox::cMyGuiComboBox()
{
	m_iSelectedIndex = 0;
	m_strDataVector = { "none" };
	this->m_vSize.x = 200.f;
}

void cMyGuiComboBox::RenderProperty()
{
}

void cMyGuiListBox::RenderMultiSelectable()
{
	static std::vector<std::string> items = { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5" };
	static std::vector<bool> selected(items.size(), false); // Track selected states
	// Start List Box
	if (ImGui::BeginListBox("##multiselect", ImVec2(-FLT_MIN, 6 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (size_t i = 0; i < items.size(); ++i)
		{
			// Display checkbox for each item
			ImGui::Selectable(items[i].c_str(), selected[i], ImGuiSelectableFlags_AllowDoubleClick);

			// Handle multi-select with modifier keys (Shift or Ctrl)
			bool l_bCtrl = ImGui::IsKeyDown(ImGuiKey_RightCtrl) | ImGui::IsKeyDown(ImGuiKey_LeftCtrl);
			if (ImGui::IsItemClicked() && l_bCtrl)
			{
				selected[i] = !selected[i];
			}
		}
		ImGui::EndListBox();
	}
}

void cMyGuiListBox::InternalRender()
{
	//RenderMultiSelectable();
	//return;
	this->m_vSize.y = 200.f;
	if (this->m_vSize.x > 0)
	{
		ImGui::PushItemWidth(this->m_vSize.x);
	}
	const char* l_strTemp[999];
	for (int i = 0; i < m_strDataVector.size(); ++i)
	{
		l_strTemp[i] = m_strDataVector[i].c_str();
	}
	// Create the listbox
	int l_iCount = (int)m_strDataVector.size();
	if (ImGui::ListBox("Versiones", &m_iSelectedIndex, l_strTemp, l_iCount))
	{
		// Item selected - do something with selectedItemIndex
		//ImGui::Text("You selected: %s", items[selectedItemIndex]);
		if (m_fOnSelectFunction)
		{
			m_fOnSelectFunction(m_iSelectedIndex);
		}
	}
	if (this->m_vSize.x > 0)
	{
		ImGui::PopItemWidth();
	}
	//ImGui::Begin("Custom Scrollable List Example");

	//// Define the size of the child window (Width, Height)
	//ImVec2 childSize = ImVec2(0, 100.0f); // 100 pixels height, you can adjust this to control how much of the list is visible

	//// Begin the child window, which will act as the scrollable area
	//ImGui::BeginChild("ListBoxChild", childSize, true, ImGuiWindowFlags_None);

	//// Loop through the items and create selectable items inside the child window
	//for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
	//	// Highlight selected item
	//	if (ImGui::Selectable(items[i], selectedItemIndex == i)) {
	//		selectedItemIndex = i;
	//	}
	//}

	//ImGui::EndChild(); // End the child window

	//if (selectedItemIndex != -1) {
	//	ImGui::Text("You selected: %s", items[selectedItemIndex]);
	//}

	//ImGui::End();
}

void cMyGuiListBox::RenderProperty()
{
}

cMyGuiBasicObj* GetMyGuiObj(eMyImGuiType e_eMyImGuiType, cMyGuiBasicObj* e_pParent)
{
	cMyGuiBasicObj* l_pObject = nullptr;
	switch (e_eMyImGuiType)
	{
	//case	eMIGT_NODE:
	//	l_pObject = new cMyGuiRootNode();
	//	break;
	case	eMIGT_BUTTON:
		l_pObject = new cMyGuiButton();
		break;
	case	eMIGT_LABEL:
		l_pObject = new cMyGuiLabel();
		break;
	case	eMIGT_EDIT_BOX:
		l_pObject = new cMyGuiEditBox();
		break;
	case	eMIGT_SLIDER_I:
		l_pObject = new cMyGuiSliderInteger();
		break;
	case	eMIGT_SLIDER_F:
		l_pObject = new cMyGuiSliderFloatValue();
		break;
	case	eMIGT_CHECKBOX:
		l_pObject = new cMyGuiCheckBox();
		break;
	case	eMIGT_RADIO:
		l_pObject = new cMyGuiRadio();
		break;
	case	eMIGT_TOOGLE:
		l_pObject = new cMyGuiToogle();
		break;
	case	eMIGT_FORM:
		l_pObject = new cMyGuiForm();
		break;//9
	case	eMIGT_PANEL:
		l_pObject = new cMyGuiPanel();
		break;
	case	eMIGT_COMBO_BOX:
		l_pObject = new cMyGuiComboBox();
		break;
	case	eMIGT_LIST_BOX:
		l_pObject = new cMyGuiListBox();
		break;
	}
	//e_pParent->add
	return l_pObject;
}

void CallConfirmDialog(std::function<void(bool)>e_CompleteFunction, const char* e_strContent, const char* e_strConfirmButtonText, const char* e_strTitle)
{
	//{
	ImGui::OpenPopup(e_strTitle);
	//}

	if (ImGui::BeginPopupModal(e_strTitle, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(e_strContent);
		ImGui::Separator();
		// "Yes" button
		if (ImGui::Button(e_strConfirmButtonText))
		{
			//userChoice = true; // Yes selected
			e_CompleteFunction(true);
			//showPopup = false; // Close popup
			ImGui::CloseCurrentPopup();
			//std::cout << "User selected: Yes\n"; // Handle choice
		}

		ImGui::EndPopup();
	}
}


void CallYesNoDialog(std::function<void(bool)>e_CompleteFunction, const char* e_strContent, const char* e_strYesButtonText, const char* e_strNoButtonText, const char* e_strTitle)
{
	//static bool showPopup = true;
	//static bool userChoice = false; // Stores the user's choice (true = Yes, false = No)

	//// Button to trigger the popup
	//if (ImGui::Button("Open Yes/No Dialog"))
	//{
	//	showPopup = true; // Trigger popup display
	//}

	//// Create a modal popup
	//if (showPopup)
	//{
	ImGui::OpenPopup(e_strTitle);
	//}

	if (ImGui::BeginPopupModal(e_strTitle, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(e_strContent);
		ImGui::Separator();

		// "Yes" button
		if (ImGui::Button(e_strYesButtonText))
		{
			//userChoice = true; // Yes selected
			e_CompleteFunction(true);
			//showPopup = false; // Close popup
			ImGui::CloseCurrentPopup();
			//std::cout << "User selected: Yes\n"; // Handle choice
		}

		ImGui::SameLine();

		// "No" button
		if (ImGui::Button(e_strNoButtonText))
		{
			e_CompleteFunction(false);
			//userChoice = false; // No selected
			//showPopup = false; // Close popup
			ImGui::CloseCurrentPopup();
			//std::cout << "User selected: No\n"; // Handle choice
		}

		ImGui::EndPopup();
	}
}

void CallFullScreenBlackText(const char* e_strContent)
{
	ImGui::OpenPopup("\t\t\t");
	if (ImGui::BeginPopupModal("\t\t\t", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(e_strContent);

		ImGui::EndPopup();
	}
}

//struct TreeNode
//{
//	std::string name;
//	std::vector<TreeNode> children;
//};

void DisplayTree(cImGuiNode* e_pNode, cImGuiNode** e_ppDragNode, cImGuiNode** e_ppDropParent, bool e_bRenderVisibleCheckBox)
{
	if (!e_pNode)
	{
		return;
	}
	auto l_strID = ValueToString(e_pNode->GetUniqueID());
	// Display the tree node with a checkbox
	bool l_bVisible = e_pNode->IsVisible(); // Assume cImGuiNode has an IsChecked() method
	ImGui::PushID(e_pNode); // Push a unique ID for the checkbox
	if (ImGui::Checkbox("", &l_bVisible))
	{
		e_pNode->SetVisible(l_bVisible); // Assume cImGuiNode has a SetChecked() method
	}
	ImGui::PopID(); // Restore ID stack
	ImGui::SameLine(); // Align the checkbox with the tree node

	// Determine if this node has children
	bool hasChildren = !e_pNode->GetChildNodeVector().empty();
	// Set TreeNodeEx flags
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (!hasChildren)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Leaf; // Mark as a leaf to remove the arrow
	}
	//if (ImGui::TreeNode(l_strID.c_str(), "%s", e_pNode->GetCharName().c_str()))
	if (ImGui::TreeNodeEx(l_strID.c_str(), nodeFlags, "%s", e_pNode->GetCharName().c_str()))
	{
		if (ImGui::BeginDragDropSource())
		{
			//std::cout << "Dragging Node: " << e_pNode->GetCharName() << std::endl;
			ImGui::SetDragDropPayload("TREE_NODE", &e_pNode, sizeof(cImGuiNode*));
			ImGui::Text("Dragging %s", e_pNode->GetCharName().c_str());
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TREE_NODE"))
			{
				cImGuiNode** payload_node = (cImGuiNode**)payload->Data;
				//std::cout << "Dropping Node on: " << e_pNode->GetCharName() << std::endl;
				if (payload_node && *payload_node)
				{
					*e_ppDragNode = *payload_node;
					*e_ppDropParent = e_pNode;
				}
			}
			ImGui::EndDragDropTarget();
		}

		for (auto& child : e_pNode->GetChildNodeVector())
		{
			DisplayTree(child, e_ppDragNode, e_ppDropParent, e_bRenderVisibleCheckBox);
		}

		ImGui::TreePop();
	}
}
