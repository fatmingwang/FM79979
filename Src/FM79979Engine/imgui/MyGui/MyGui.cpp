#include "MyGui.h"
#include "../../Core/AllCoreInclude.h"
#include "../ThirtyParty/ImGuiBuilder/additional.h"
using namespace	FATMING_CORE;
TYPDE_DEFINE_MARCO(cImGuiNode);
TYPDE_DEFINE_MARCO(cMyGuiNode);
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

cImGuiNode::cImGuiNode()
{
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
		}
		else
		{
			m_vWorldPos = m_vLocalPos;
		}
	}
}


void cImGuiNode::SetLocalPosition(const ImVec2& vLocalPos)
{
	if (m_vLocalPos.x != vLocalPos.x || m_vLocalPos.y != vLocalPos.y)
	{
		m_bPosDirty = true;
		SetCachedWorldTransformDirty();
	}
}

ImVec2 cImGuiNode::GetWorldPosition()
{
	UpdateCachedWorldTransformIfNeeded();
	return m_vWorldPos;
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
	this->InternalRender();
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
		ImVec2		l_vPos = m_vPos ;
		//ImVec2		l_vSizeObj = m_vSizeObj;
		ImGui::InputFloat("SizeX", &m_vSize.x, 1);
		ImGui::InputFloat("SizeY", &m_vSize.y, 1);
		ImGui::InputFloat("PosX", &m_vPos.x, 1);
		ImGui::InputFloat("PosY", &m_vPos.y, 1);
		if (m_vPos.x!= l_vPos.x || m_vPos.y != l_vPos.y)
		{
			this->SetLocalPosition(m_vPos);
		}
		bool l_bBorder = false;;
		bool l_bLock = false;;
		ImGui::Checkbox("Border", &l_bBorder);
		ImGui::SameLine();
		ImGui::Checkbox("Lock", &l_bLock);
	ImGui::End();

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

void cMyGuiPanel::InternalRender()
{
}

void cMyGuiPanel::RenderProperty()
{
}

void cMyGuiForm::InternalRender()
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

void cMyGuiButton::InternalRender()
{
	//ImGui::Button(obj.name.c_str(), obj.size);
}

void cMyGuiNode::InternalRender()
{
}


cMyGuiBasicObj* GetMyGuiObj(eMyImGuiType e_eMyImGuiType, cMyGuiBasicObj* e_pParent)
{
	cMyGuiBasicObj* l_pObject = nullptr;
	switch (e_eMyImGuiType)
	{
	case	eMIGT_NODE:
		l_pObject = new cMyGuiNode();
		break;
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
	}
	//e_pParent->add
	return l_pObject;
}