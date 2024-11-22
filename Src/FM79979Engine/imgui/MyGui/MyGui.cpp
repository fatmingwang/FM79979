#include "MyGui.h"
#include "../../Core/AllCoreInclude.h"
#include "../ThirtyParty/ImGuiBuilder/additional.h"
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
	m_bDoApplyPosition = true;
	m_bOnlyApplyPositionOnceForDragMoving = false;
	this->SetName(this->Type());
}

cImGuiNode::~cImGuiNode()
{
	DeleteObjectAndAllChildren(this);
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
			if (this->m_pParent->m_ChildNodeVector[i] == this)
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
	if (this->m_bDoApplyPosition)
	{
		if (this->m_bOnlyApplyPositionOnceForDragMoving)
		{
			this->m_bDoApplyPosition = false;
		}
		ApplyPosition();
	}
	this->InternalRender();
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
		e_pImGuiNode->SetParent(nullptr);
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

void cMyGuiEditBox::InternalRender()
{
	//ImGui::PushItemWidth(relative_for_resize(obj));
	//ImGui::InputText(obj.name.c_str(), const_cast<char*>(buffer.c_str()), 254);
	//ImGui::PopItemWidth();
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

	}
	//ImGui::Button(obj.name.c_str(), obj.size);
}
//please us epanel and set border false to retend it's a  node
void cMyGuiRootNode::ApplyPosition()
{
	ImGui::SetNextWindowPos(this->m_vLocalPos);
}

void cMyGuiRootNode::InternalRender()
{
}

void cMyGuiComboBox::InternalRender()
{
	this->m_vSize.y = 200.f;
	if (this->m_vSize.y > 0)
	{
		ImGui::PushItemWidth(this->m_vSize.y);
	}
	// Array of items
	const char* items3[] = { "Option 1", "Option 2", "Option 3", "Option 4", "Option 5" };
	static int selectedItemIndex3 = 0; // Track selected item index
	m_strDataVector = { "Option 1", "Option 2", "Option 3", "Option 4", "Option 5" };
	const char* items4[999];
	for (int i = 0; i < m_strDataVector.size(); ++i)
	{
		items4[i] = m_strDataVector[i].c_str();
	}
	//m_strEvnNameVector = { "Option 1", "Option 2", "Option 3", "Option 4", "Option 5" };
	// Create the combo box
	if (ImGui::Combo("My ComboBox", &selectedItemIndex3, items4, m_strDataVector.size())) 
	{
		// Item selected - do something with selectedItemIndex
		ImGui::Text("You selected: %s", items4[selectedItemIndex3]);
	}
	if (this->m_vSize.y > 0)
	{
		ImGui::PopItemWidth();
	}
}

void cMyGuiComboBox::RenderProperty()
{
}

void cMyGuiListBox::InternalRender()
{
	this->m_vSize.y = 200.f;
	if (this->m_vSize.y > 0)
	{
		ImGui::PushItemWidth(this->m_vSize.y);
	}
	// Array of items
	const char* items[] = { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5","Item 6", "Item 7", "Item 8", "Item 9", "Item 10" };
	static int selectedItemIndex = 0; // Track selected item index
	// Create the listbox
	if (ImGui::ListBox("Versiones", &selectedItemIndex, items, IM_ARRAYSIZE(items)))
	{
		// Item selected - do something with selectedItemIndex
		ImGui::Text("You selected: %s", items[selectedItemIndex]);
	}
	if (this->m_vSize.y > 0)
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