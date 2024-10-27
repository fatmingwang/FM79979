#include "MyGui.h"
#include "../../Core/AllCoreInclude.h"
using namespace	FATMING_CORE;
cImGuiNode::cImGuiNode()
{
}

cImGuiNode::~cImGuiNode()
{
	DeleteObjectAndAllChildren(this);
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

void cImGuiNode::SetParent(cImGuiNode* e_pParent, int e_iIndex)
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
		if (e_iIndex == -1)
		{
			this->m_pParent->m_ChildNodeVector.push_back(this);
		}
		else
		{
			this->m_pParent->m_ChildNodeVector.insert(this->m_pParent->m_ChildNodeVector.begin() + e_iIndex, this);
		}
		
	}
}

void cImGuiNode::Render()
{
	//// set pos for next obj render
	//ImGui::SetCursorPos(this->GetLocalPosition());
	////ImGui::GetCursorScreenPos();



	//	// previous object, before rendering the others
	//	for (auto new_id = obj.id - 1; new_id < m_objs.size(); ++new_id)
	//	{
	//		m_objs[new_id].id = new_id;
	//		//std::cout << obj_render_me[new_id].id << std::endl;
	//	}
	//	return; //not to render the object
	//}

	//// To type any obj for render set in case
	//// to render all obj for 1 time in loop

	//// buffer for inputs
	//std::string		buffer = "text here";
	//int				value_i = 0;
	//float			value_f = 0;
	//static bool		true_bool = false;
	//auto			normal_select = (m_current_item == (obj.name + ":" + std::to_string(obj.id)));


	//auto relative_for_resize = [](basic_obj& obj) -> float
	//	{
	//		auto& g = *GImGui;
	//		auto* window = g.CurrentWindow;
	//		const auto& style = g.Style;
	//		const auto	id = window->GetID(obj.name.c_str());
	//		const auto	label_size = ImGui::CalcTextSize(obj.name.c_str(), nullptr, true);
	//		const auto	frame_size = ImGui::CalcItemSize(ImVec2(0, 0), ImGui::CalcItemWidth(), (label_size.y) + style.FramePadding.y * 2.0f);
	//		const auto	label_dif = (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f);
	//		if (obj.size.x == 0.f && obj.size.y == 0.f)
	//			obj.size = ImVec2(frame_size.x + label_dif, frame_size.y);
	//		return obj.size.x - label_dif;
	//	};

	//// render obj
	//switch (obj.my_type)
	//{
	//case eMyImGuiType::eMIGT_BUTTON:
	//	ImGui::Button(obj.name.c_str(), obj.size);
	//	break;
	//case eMIGT_LABEL:
	//	ImGui::Text(obj.name.c_str());
	//	break;
	//case eMIGT_EDIT:
	//{
	//	ImGui::PushItemWidth(relative_for_resize(obj));
	//	ImGui::InputText(obj.name.c_str(), const_cast<char*>(buffer.c_str()), 254);
	//	ImGui::PopItemWidth();
	//	break;
	//}
	//case eMIGT_SLIDER_I:
	//{
	//	ImGui::PushItemWidth(relative_for_resize(obj));
	//	ImGui::SliderInt(obj.name.c_str(), &value_i, 0, 100);
	//	ImGui::PopItemWidth();
	//	break;
	//}
	//case eMIGT_SLIDER_F:
	//{
	//	ImGui::PushItemWidth(relative_for_resize(obj));
	//	ImGui::SliderFloat(obj.name.c_str(), &value_f, 0, 100);
	//	ImGui::PopItemWidth();
	//	break;
	//}
	//case eMIGT_CHECKBOX:
	//	ImGui::Checkbox(obj.name.c_str(), &true_bool);

	//	break;
	//case eMIGT_RADIO:
	//	ImGui::RadioButton(obj.name.c_str(), true_bool);

	//	break;
	//case eMIGT_TOOGLE:
	//	ImGui::ToggleButton(obj.name.c_str(), &true_bool);

	//	break;
	//default:
	//	break;
	//}
	//obj.size_obj = ImGui::GetItemRectSize();

	//bool scrollEnableY = ImGui::GetScrollMaxY() > 0.f;

	//auto scrollPosY = ImGui::GetScrollY();

	//ImVec2 oldPos = obj.pos;

	//if (scrollEnableY)
	//{
	//	obj.pos.y -= scrollPosY;
	//}

	//if ((obj.selected || normal_select) && current_form_id == m_active_window_id)
	//{
	//	ImGui::DrawObjBorder(obj);
	//}

	//// get size and hover of object
	////obj.hover		= ImGui::IsItemHovered( );
	//obj.hover = my_IsItemHovered(obj.pos, obj.size_obj, 5.f);

	//// Set family and type child etc for propri and execution modification on type
	//auto left_clicked = ImGui::IsMouseClicked(0, false);
	//auto right_clicked = ImGui::IsMouseClicked(1, false);
	//auto show_context = normal_select;
	//if (obj.hover && (left_clicked || right_clicked) && show_context == false)
	//{
	//	show_context = !left_clicked;
	//	if (GetKeyState(VK_CONTROL) & 0x8000)
	//		obj.selected = !obj.selected;
	//	else
	//	{
	//		for (auto& o_obj : m_objs)
	//			o_obj.selected = false;
	//	}

	//	m_current_item = obj.name + ':' + std::to_string(obj.id);
	//	m_family = obj.form;
	//	m_grandchild = obj.child;
	//	m_index = obj.id;
	//	m_type = obj.my_type;
	//}

	//ImGui::PushAllColorsDark(m_dark_style);

	//if (show_context && ImGui::BeginPopupContextItem("##obj_context"))
	//{
	//	//style.ButtonTextAlign
	//	auto& g = *GImGui;
	//	auto backup1_y = g.Style.ButtonTextAlign.y;
	//	auto backup2_y = g.Style.FramePadding.y;
	//	g.Style.FramePadding.y = -1.3f;
	//	g.Style.ButtonTextAlign.y = 0.f;
	//	ImVec2 btn_size = { 60.f, 12.f };
	//	if (ImGui::Button("delete", btn_size))
	//	{
	//		obj.delete_me = true;
	//		m_current_item = "";
	//		m_type = -1;
	//	}
	//	if (ImGui::Button("copy", btn_size))
	//	{
	//		copy_obj((eMyImGuiType)obj.my_type, obj.child, obj.size, obj.pos, false, false, true);
	//	}
	//	ImGui::Checkbox("lock", &obj.locked);
	//	g.Style.ButtonTextAlign.y = backup1_y;
	//	g.Style.FramePadding.y = backup2_y;
	//	ImGui::EndPopup();
	//}

	//ImGui::PopAllColorsCustom();

	//if (obj.hover)
	//	SetCursor(this->cursor.m_arrow_all);

	//obj.pos = oldPos;

	//resize_obj(obj, normal_select);
	ImVec2 l_vPos = this->GetLocalPosition();
	auto l_pParent = this->GetParent();
	if (!l_pParent)
	{
		ImGui::SetNextWindowPos(l_vPos);
	}
	else
	{
		ImGui::SetCursorPos(l_vPos);
	}
	this->InternalRender();
	for (auto l_Child : this->m_ChildNodeVector)
	{
		l_Child->Render();
	}
	EndRender();
	if (!l_pParent)
	{
		ImGui::SetNextWindowPos(l_vPos);
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

void cMyGuiBasicObj::RenderProperty()
{
}

cMyGuiBasicObj* GetMyGuiObj(eMyImGuiType e_eMyImGuiType)
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
	return l_pObject;
}