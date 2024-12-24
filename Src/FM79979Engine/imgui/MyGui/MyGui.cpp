#include "MyGui.h"
#include "../misc/cpp/imgui_stdlib.h"
#include "../../Core/AllCoreInclude.h"
#include "../ThirtyParty/ImGuiBuilder/additional.h"
#include <iostream>
#include "../ImGuiRender.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include "MyGUIUtilities.h"

static int g_siMyHuiUID = 0;
cImGuiNode::sImguiData::sImguiData()
{
	this->m_iID = g_siMyHuiUID;
	g_siMyHuiUID++;
}

using namespace	FATMING_CORE;
TYPDE_DEFINE_MARCO(cImGuiNode);
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
TYPDE_DEFINE_MARCO(cMyGuiScroller);
TYPDE_DEFINE_MARCO(cMyGuiRootNode);
TYPDE_DEFINE_MARCO(cMyGuiDatePicker);

std::map<std::wstring, eMyImGuiType> g_ImguiTypeNameAndType =
{
	{cImGuiNode::TypeID,eMyImGuiType::eMIGT_NODE},
	{cMyGuiButton::TypeID,eMyImGuiType::eMIGT_BUTTON},
	{cMyGuiLabel::TypeID,eMyImGuiType::eMIGT_LABEL},
	{cMyGuiEditBox::TypeID,eMyImGuiType::eMIGT_EDIT_BOX},
	{cMyGuiSliderInteger::TypeID,eMyImGuiType::eMIGT_SLIDER_I},
	{cMyGuiSliderFloatValue::TypeID,eMyImGuiType::eMIGT_SLIDER_F},
	{cMyGuiCheckBox::TypeID,eMyImGuiType::eMIGT_CHECKBOX},
	{cMyGuiRadio::TypeID,eMyImGuiType::eMIGT_RADIO},
	{cMyGuiToogle::TypeID,eMyImGuiType::eMIGT_TOOGLE},
	{cMyGuiForm::TypeID,eMyImGuiType::eMIGT_FORM},
	{cMyGuiPanel::TypeID,eMyImGuiType::eMIGT_PANEL},
	{cMyGuiComboBox::TypeID,eMyImGuiType::eMIGT_COMBO_BOX},
	{cMyGuiListBox::TypeID,eMyImGuiType::eMIGT_LIST_BOX},
	{cMyGuiRootNode::TypeID,eMyImGuiType::eMIGT_ROOT_NODE},
	{cMyGuiScroller::TypeID,eMyImGuiType::eMIGT_SCROLLER},
	{cMyGuiDatePicker::TypeID,eMyImGuiType::eMIGT_DATA_PICKER}
};

cImGuiNode::cImGuiNode()
{
	//JsonTesting();
	m_pParent = nullptr;
	m_bEnable = true;
	m_bVisible = true;
	m_bDoApplyPosition = true;
	m_bOnlyApplyPositionOnceForDragMoving = false;
	//this->SetImGuiName(GetTypeName());
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
			m_pData->m_vWorldPos = ImVec2(m_pParent->m_pData->m_vWorldPos.x + m_pData->m_vLocalPos.x, m_pParent->m_pData->m_vWorldPos.y + m_pData->m_vLocalPos.y);
			if (m_pParent->m_bThisUseContainerPositionDontApplyParentPositionToChild)
			{
				m_vImGuiRenderPos = m_pData->m_vLocalPos;
			}
			else
			{
				m_vImGuiRenderPos = m_pData->m_vWorldPos;
			}
		}
		else
		{
			m_pData->m_vWorldPos = m_pData->m_vLocalPos;
		}
		m_bPosDirty = false;
	}
}


void cImGuiNode::SetImGuiName(const wchar_t* e_strImGuiNmae)
{
	m_pData->m_strImGuiName = ValueToString(e_strImGuiNmae);
}

void cImGuiNode::SetLocalPosition(const ImVec2& e_vLocalPos)
{
	if (m_pData->m_vLocalPos.x != e_vLocalPos.x || m_pData->m_vLocalPos.y != e_vLocalPos.y)
	{
		m_pData->m_vLocalPos = e_vLocalPos;
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
	return m_pData->m_vWorldPos;
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
	m_bCollided = false;
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

	GetRenderRect();

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

void cImGuiNode::DebugRender()
{
	if (!m_bVisible)
	{
		return;
	}
	Vector2 l_vRenderPos(m_RenderRect.x, m_RenderRect.y);
	//cGameApp::RenderFont(l_vRenderPos, UT::ComposeMsgByFormat(L"Min:%d,%d\nMax:%d,%d", (int)m_RenderRect.x, (int)m_RenderRect.y, (int)m_RenderRect.z, (int)m_RenderRect.w).c_str());
	cGameApp::RenderFont(l_vRenderPos, this->GetName());
	if (m_bCollided)
	{
		RenderRectangle(l_vRenderPos, m_RenderRect.Width(), m_RenderRect.Height(), Vector4::Yellow);
	}
	else
	{
		RenderRectangle(l_vRenderPos, m_RenderRect.Width(), m_RenderRect.Height(), Vector4::Green);
	}
	
	for (auto l_Child : this->m_ChildNodeVector)
	{
		l_Child->DebugRender();
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

nlohmann::json cImGuiNode::GetJson()
{
	return *m_pData;
}

cImGuiNode*cImGuiNode::Collided(int e_iPosX, int e_iPosY)
{
	if (this->m_bVisible)
	{
		cImGuiNode* l_pCollidedObject = nullptr;
		for (auto l_Child : this->m_ChildNodeVector)
		{
			l_pCollidedObject = l_Child->Collided(e_iPosX, e_iPosY);
			if (l_pCollidedObject)
			{
				return l_pCollidedObject;
			}
		}
		if (m_RenderRect.CollidePoint(e_iPosX, e_iPosY))
		{
			this->m_bCollided = true;
			return this;
		}
	}
	return nullptr;
}

bool cImGuiNode::ExportJsonFile(const char* e_strFileName)
{
	nlohmann::json json;
	this->DoSerialize(json);

	// Write JSON to file
	std::ofstream file(e_strFileName);
	if (file.is_open())
	{
		file << json.dump(4); // Pretty print with 4 spaces
		file.close();
		//std::cout << "Tree saved to " << filename << std::endl;
		return true;
	}	
	return false;
}


std::string ValueToString(ImVec2 e_v)
{
	return ValueToString(Vector2(e_v.x, e_v.y));
}

ImVec2	GetImVec2(std::string e_str)
{
	auto l_vPos = GetVector2(e_str.c_str());
	return ImVec2(l_vPos.x, l_vPos.y);
}

void cImGuiNode::DoSerialize(nlohmann::json& e_JSON)
{
	const char* l_strChildrenKey = "Children";
	std::string l_strNodeType = ValueToString(Type());
	e_JSON[l_strNodeType] = this->GetJson();
	if (GetChildNodeVector().size())
	{
		e_JSON[l_strNodeType][l_strChildrenKey] = nlohmann::json::array();
		for (auto l_Child : GetChildNodeVector())
		{
			nlohmann::json l_ChildJson;
			l_Child->DoSerialize(l_ChildJson);
			e_JSON[l_strNodeType][l_strChildrenKey].push_back(l_ChildJson);
		}
	}
}

bool cImGuiNode::InternalSerialize(const nlohmann::json& e_Json)
{
	m_pData = std::make_shared<sImguiData>();
	*m_pData = e_Json;
	return true;
}

bool cImGuiNode::UnSerialize(const nlohmann::json& e_Json)
{
	// Ensure the JSON has at least one key-value pair
	if (!e_Json.is_object() || e_Json.empty())
	{
		return false;
	}
	std::string l_strType = e_Json.begin().key();

	auto l_Object = e_Json[l_strType];
	InternalSerialize(l_Object);
	int l_iID = l_Object["m_iID"];
	if (l_iID > g_siMyHuiUID)
	{
		g_siMyHuiUID = l_iID + 1;
	}
	if (l_Object.contains("Children") && l_Object["Children"].is_array())
	{
		for (const auto& child : l_Object["Children"])
		{
			auto l_pChildNode = cImGuiNode::DoUnSerialize(child);
			if (l_pChildNode)
			{
				AddChild(l_pChildNode);
			}
		}
	}
	return true;
}

std::string cImGuiNode::GetRenderText()
{
	auto l_strText = this->GetText();
	if (strlen(l_strText.c_str()) == 0)
	{
		l_strText = ValueToString(this->Type());
	}
	return l_strText;
}

cImGuiNode*cImGuiNode::DoUnSerialize(const nlohmann::json& e_Json)
{
	// Ensure the JSON has at least one key-value pair
	if (!e_Json.is_object() || e_Json.empty())
	{
		return nullptr;
	}
	std::string l_strType = e_Json.begin().key();

	auto l_ImguiNode = GetImGuiNodeByType(ValueToStringW(l_strType).c_str());
	if (l_ImguiNode)
	{
		l_ImguiNode->UnSerialize(e_Json);
		return l_ImguiNode;
	}
	return l_ImguiNode;
}

std::wstring cImGuiNode::GetNameW()
{
	return std::wstring();
}

cMyGuiBasicObj::cMyGuiBasicObj()
{
}

cMyGuiBasicObj::~cMyGuiBasicObj()
{
}

void cMyGuiBasicObj::RenderNameOnTop()
{
	if (m_bNameOnTop)
	{
		auto l_strName = this->GetImGuiName();
		auto l_PosX = ImGui::GetCursorPosX();
		ImGui::Text(l_strName.c_str());
		ImGui::SetCursorPosX(l_PosX);
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
		std::string l_strName = this->m_pData->m_strImGuiName;
		ImGui::InputTextEx("Name form", &l_strName, 0);
		//ImGui::InputText("name form", name, 255);
		if (ImGui::Button("Apply name"))
		{
			if (!this->m_pData->m_strImGuiName.empty())
			{
				this->m_pData->m_strImGuiName = l_strName;
			}
		}
		//ImVec2		l_vSize = m_vSize;
		ImVec2		l_vPos = GetLocalPosition();
		//ImVec2		l_vSizeObj = m_vSizeObj;
		ImGui::InputFloat("SizeX", &m_pData->m_vSize.x, 1);
		ImGui::InputFloat("SizeY", &m_pData->m_vSize.y, 1);
		ImGui::InputFloat("PosX", &l_vPos.x, 1);
		ImGui::InputFloat("PosY", &l_vPos.y, 1);
		if (m_pData->m_vLocalPos.x!= l_vPos.x || m_pData->m_vLocalPos.y != l_vPos.y)
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

void	cMyGuiBasicObj::GetRenderRect()
{
	auto l_MinRect = ImGui::GetItemRectMin();
	auto l_MaxRect = ImGui::GetItemRectMax();
	m_RenderRect = Vector4(l_MinRect.x, l_MinRect.y, l_MaxRect.x, l_MaxRect.y);
}

std::string cMyGuiBasicObj::GetIDString()
{
	return ValueToString(this->m_pData->m_iID);
}

void cMyGuiBasicObj::RenderProperty()
{
}

void cMyGuiBasicObj::CreateImguiDataData()
{
	m_pData = std::make_shared<sImguiData>();
	auto l_IT = g_ImguiTypeNameAndType.find(this->Type());
	if (l_IT != g_ImguiTypeNameAndType.end())
	{
		this->m_pData->m_strText = GetMyGuiObjLabel(l_IT->second);
	}
	else
	{
		this->m_pData->m_strText = ValueToString(this->Type());
	}
}

float relative_for_resize(std::string&e_strName,ImVec2&e_vSize)
{
	return 0.f;
	//auto g = ImGui::GetCurrentContext();
	//const auto	label_size = ImGui::CalcTextSize(e_strName.c_str(), nullptr, true);
	//const auto	frame_size = ImGui::CalcItemSize(ImVec2(0, 0), ImGui::CalcItemWidth(), (label_size.y));
	//const auto	label_dif = (label_size.x > 0.0f ? label_size.x : 0.0f);
	//if (e_vSize.x == 0.f && e_vSize.y == 0.f)
	//{
	//	e_vSize = ImVec2(frame_size.x + label_dif, frame_size.y);
	//}
	//return e_vSize.x - label_dif;
};

void f_MySkipScissor(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
	//cmd->m_bSkipScissor = false;
}


cMyGuiPanel::cMyGuiPanel()
{
	this->m_bThisUseContainerPositionDontApplyParentPositionToChild = true;
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
	//static bool l_bSkipScissor = true;
	//ImDrawList* draw_list = ImGui::GetWindowDrawList();
	//draw_list->AddCallback(f_MySkipScissor, &l_bSkipScissor);
	//child_size, true, ImGuiWindowFlags_NoBackground))

	if (ImGui::BeginChild(this->GetRenderText().c_str(), this->m_pData->m_vSize, true, this->m_pPanelData->m_iFormFlag))
	{
		ImVec2 childPos = ImGui::GetWindowPos();
		m_RenderRect = Vector4(childPos.x, childPos.y, childPos.x + m_pData->m_vSize.x, childPos.y + m_pData->m_vSize.y);
	}
}

void cMyGuiPanel::EndRender()
{
	//ImGui::PopClipRect();
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
	//ImGui::Begin("BUILDER", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
	//m_FormFlag = ImGuiWindowFlags_NoTitleBar| ImGuiWindowFlags_NoBackground;

	bool l_bOpen = true;
	//ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(m_pData->m_vSize, ImGuiCond_Always);
	//ImGui::Begin(this->GetImGuiName().c_str(), &l_bOpen, m_FormFlag);
	ImGui::Begin(this->GetRenderText().c_str(), m_bShowCloseCutton?&l_bOpen:nullptr, m_FormFlag);// 
	//ImGui::PushClipRect(ImVec2(-9999, -9999), ImVec2(9999, 9999), true);
	if (m_pData->m_vSize.x > 0 && m_pData->m_vSize.y > 0)
	{
		ImGui::SetWindowSize(m_pData->m_vSize);
	}
	if (!l_bOpen)
	{
		if (m_fFormCloseFunction)
		{
			m_fFormCloseFunction(this);
		}
		int a = 0;
	}	
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	viewport->m_bSkipViewportClip = true;
	//ImDrawList* draw_list = ImGui::GetWindowDrawList();
	//static bool l_bSkipScissor = true;
	//draw_list->AddCallback(f_MySkipScissor, &l_bSkipScissor);
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
	//ImGui::PopClipRect();
	ImGui::End();
}

cMyGuiForm::cMyGuiForm()
{
	m_bShowCloseCutton = false;
	this->m_bThisUseContainerPositionDontApplyParentPositionToChild = true;
}

cMyGuiForm::~cMyGuiForm()
{
}

void cMyGuiForm::RenderProperty()
{
}

cMyGuiToogle::cMyGuiToogle()
{
	m_bChecked = false;
}

cMyGuiToogle::~cMyGuiToogle()
{
}

void cMyGuiToogle::InternalRender()
{
	ImGui::Text(this->m_pData->m_strText.c_str());
	ImGui::SameLine();
	ImGui::ToggleButton(this->m_pData->m_strText.c_str(), &m_bChecked);
}

void cMyGuiToogle::RenderProperty()
{
}

void cMyGuiRadio::InternalRender()
{
	if(ImGui::RadioButton(this->m_pData->m_strText.c_str(), m_bChecked))
	{
		m_bChecked = !m_bChecked;
	}
}

cMyGuiRadio::cMyGuiRadio()
{
	m_bChecked = false;
}

cMyGuiRadio::~cMyGuiRadio()
{
}

void cMyGuiCheckBox::InternalRender()
{
	ImGui::Checkbox(this->m_pData->m_strText.c_str(), &m_bChecked);
}

cMyGuiCheckBox::cMyGuiCheckBox()
{
	m_bChecked = false;
}

cMyGuiCheckBox::~cMyGuiCheckBox()
{
}

cMyGuiSliderFloatValue::cMyGuiSliderFloatValue()
{
}

cMyGuiSliderFloatValue::~cMyGuiSliderFloatValue()
{
}

void cMyGuiSliderFloatValue::InternalRender()
{
	ImGui::PushItemWidth(m_pData->m_vSize.x);
	ImGui::SliderFloat(this->m_pData->m_strImGuiName.c_str(), &this->m_psSliderData->m_fValue, m_psSliderData->m_fMin, m_psSliderData->m_fMax);
	ImGui::PopItemWidth();
}

void cMyGuiSliderInteger::InternalRender()
{
	ImGui::PushItemWidth(m_pData->m_vSize.x);
	ImGui::SliderInt(this->m_pData->m_strImGuiName.c_str(), &this->m_psSliderData->m_iValue, this->m_psSliderData->m_iMin, this->m_psSliderData->m_iMax);
	ImGui::PopItemWidth();
}

cMyGuiSliderInteger::cMyGuiSliderInteger()
{
}

cMyGuiSliderInteger::~cMyGuiSliderInteger()
{
}

cMyGuiEditBox::cMyGuiEditBox()
{
}

int InputCallback(ImGuiInputTextCallbackData* data)
{
	// Avoid calling NewFrame(), Render(), or EndFrame() here!

	//SetImGuiMouseEnable(false, 1);
	return 1;
}


void cMyGuiEditBox::FocusCheck()
{
	bool l_bPrevious = m_bFocused;
	m_bFocused = ImGui::IsItemFocused();
	// Get the bounding box of the InputText widget
	ImVec2 itemMin = ImGui::GetItemRectMin();
	ImVec2 itemMax = ImGui::GetItemRectMax();

	// Get the current mouse position
	ImVec2 mousePos = ImGui::GetMousePos();

	// Check if the mouse is within the widget's bounds
	bool isMouseInRange = (mousePos.x >= itemMin.x && mousePos.x <= itemMax.x &&
						   mousePos.y >= itemMin.y && mousePos.y <= itemMax.y);

	// If the widget is focused and the mouse leaves the area, clear the focus
	if (m_bFocused && !isMouseInRange)
	{
		ImGui::SetItemDefaultFocus();
		m_bFocused = false;
	}

}

void cMyGuiEditBox::InternalRender()
{
	//this->m_pData->m_vSize.x = 100.f;
	if (this->m_pData->m_vSize.x > 0)
	{
		ImGui::PushItemWidth(this->m_pData->m_vSize.x);
	}
	if (this->m_pEditBoxData->m_bMultiLines)
	{
		RenderMultiLine();
	}
	else
	{
		if (this->m_pEditBoxData->m_strHint.length())
		{
			const char* hint = "Enter your text here..."; // Hint text
			auto l_strID = this->m_pData->m_strImGuiName;
			if (ImGui::InputTextWithHint(l_strID.c_str(), hint, &this->m_pData->m_strText))
			{
			}
		}
		else
		{

	//InputText(const char* label, std::string * str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
			//ImGui::InputText(const char* label, std::string * str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
			if (ImGui::InputText("Input", &this->m_pData->m_strText, ImGuiInputTextFlags_CallbackAlways, InputCallback, nullptr))
			{
			}
		}
	}
	bool l_bPrevious = m_bFocused;
	if (1)
	{
		m_bFocused = ImGui::IsItemFocused();
	}
	else
	{
		FocusCheck();
	}
	if (l_bPrevious != m_bFocused)
	{
		if (m_bFocused)
		{
			EditboxFocusChangedChangeMouseEnable(m_bFocused);
		}
		else
		{
			AllImGuiMouseEnable();
		}
		if (m_fFocusedChangedFunction)
		{
			m_fFocusedChangedFunction(m_bFocused);
		}
	}
	//ImGui::Text("Current Text:");
	//ImGui::TextWrapped("%s", text.c_str()); // Display the current text
	if (this->m_pData->m_vSize.x > 0)
	{
		ImGui::PopItemWidth();
	}
}

void cMyGuiEditBox::RenderMultiLine()
{
	//if (m_strHint.length() && this->this->m_pData->m_strText.length() == 0)
	//{
	//	size_t l_iLength = strlen(this->this->m_pData->m_strText.c_str());
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
	if (ImGui::InputTextMultiline("##multiline", &this->m_pData->m_strText, this->m_pData->m_vSize, ImGuiInputTextFlags_AllowTabInput))
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
	ImGui::Text(this->m_pData->m_strText.c_str());
}

cMyGuiLabel::cMyGuiLabel()
{
}

cMyGuiLabel::~cMyGuiLabel()
{
}


cMyGuiButton::cMyGuiButton()
{
}

cMyGuiButton::~cMyGuiButton()
{
}

void cMyGuiButton::InternalRender()
{
	if (ImGui::Button(this->m_pData->m_strText.c_str(), this->m_pData->m_vSize))
	{
		if (m_fOnClickFunction)
		{
			m_fOnClickFunction(this);
		}
	}
	//ImGui::Button(obj.name.c_str(), obj.size);
}
void cMyGuiRootNode::CallConfirmDialog(std::function<void(bool)>e_CompleteFunction, const char* e_strContent, const char* e_strConfirmButtonText, const char* e_strTitle)
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


void cMyGuiRootNode::CallYesNoDialog(std::function<void(bool)>e_CompleteFunction, const char* e_strContent, const char* e_strYesButtonText, const char* e_strNoButtonText, const char* e_strTitle)
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

void cMyGuiRootNode::CallFullScreenBlackText(const char* e_strContent)
{
	ImGui::OpenPopup("\t\t\t");
	if (ImGui::BeginPopupModal("\t\t\t", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(e_strContent);

		ImGui::EndPopup();
	}
}
//please us epanel and set border false to retend it's a  node
void cMyGuiRootNode::ApplyPosition()
{
	//ImGui::SetNextWindowPos(this->m_vLocalPos);
}

void cMyGuiRootNode::InternalRender()
{
}
void cMyGuiRootNode::EndRender()
{
	if (m_bShowYesNoDialog)
	{
		CallYesNoDialog(m_CompleteFunction, m_strDialogMessage.c_str(), m_strYesButtonText.c_str(), m_strNoButtonText.c_str());
	}
	else
	if (m_bShowConfirmDialog)
	{
		CallConfirmDialog(m_CompleteFunction, m_strDialogMessage.c_str(), m_strYesButtonText.c_str());
	}
	else
	if (m_bShowFullScreenBlackText)
	{
		CallFullScreenBlackText(m_strDialogMessage.c_str());
	}
	this->m_bVisible = true;
}

void cMyGuiRootNode::GetRenderRect()
{

}

void cMyGuiRootNode::ShowYesNoDialog(std::function<void(bool)> e_CompleteFunction, const char* e_strContent, const char* e_strYesButtonText, const char* e_strNoButtonText)
{
	m_strDialogMessage = e_strContent;
	m_strYesButtonText = e_strYesButtonText;
	m_strNoButtonText = e_strNoButtonText;
	m_CompleteFunction = e_CompleteFunction;
	m_bShowYesNoDialog = true;
	m_bShowConfirmDialog = false;
	m_bShowFullScreenBlackText = false;
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
	m_bShowYesNoDialog = false;
	m_bShowFullScreenBlackText = false;
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
	m_bShowYesNoDialog = false;
	m_bShowConfirmDialog = false;
	m_bShowFullScreenBlackText = false;
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

void cMyGuiRootNode::DebugRender()
{
	cMyGuiBasicObj::DebugRender();
}

void cMyGuiComboBox::InternalRender()
{
	if (this->m_pData->m_vSize.x > 0)
	{
		ImGui::PushItemWidth(this->m_pData->m_vSize.x);
	}
	if (m_ItemTempList.size() != m_pImguiComboxData->m_ItemList.size())
	{
		SetItemList(m_pImguiComboxData->m_ItemList);
	}
	const char** l_pItemTempList = nullptr;
	int l_iCount = (int)m_ItemTempList.size();
	if (l_iCount)
	{
		l_pItemTempList = &m_ItemTempList[0];
	}
	if (ImGui::Combo(this->GetRenderText().c_str(), &this->m_pImguiComboxData->m_iSelectedIndex, l_pItemTempList, l_iCount))
	{
		if (m_fOnSelectFunction)
		{
			m_fOnSelectFunction(m_pImguiComboxData->m_iSelectedIndex);
		}
		
	}
	if (this->m_pData->m_vSize.x > 0)
	{
		ImGui::PopItemWidth();
	}
}

cMyGuiComboBox::cMyGuiComboBox()
{
}

void cMyGuiComboBox::SetItemList(std::vector<std::string> e_ItemList)
{
	this->m_pImguiComboxData->m_ItemList = e_ItemList;
	m_ItemTempList.clear();
	for (std::string&l_IT : this->m_pImguiComboxData->m_ItemList)
	{
		m_ItemTempList.push_back(l_IT.c_str());
	}
}

void cMyGuiComboBox::RenderProperty()
{
}

cMyGuiListBox::cMyGuiListBox()
{
	m_bMiltiSelecteable = false;
}

cMyGuiListBox::~cMyGuiListBox()
{
}

void cMyGuiListBox::RenderProperty()
{
}

void cMyGuiListBox::SetItemList(std::vector<std::string> e_ItemList)
{
	cMyGuiComboBox::SetItemList(e_ItemList);
	int l_iCount = (int)m_ItemTempList.size();
	m_SelectedIndices = std::vector<bool>(l_iCount, false);
}

void cMyGuiListBox::RenderMultiSelectable()
{
	//this->RenderNameOnTop();
	auto l_strName = this->GetRenderText();
	int l_iCount = (int)m_ItemTempList.size();
	if (ImGui::BeginListBox(l_strName.c_str(), this->m_pData->m_vSize))
	//if (ImGui::BeginListBox("##listbox", this->m_pData->m_vSize))
	{
		for (size_t i = 0; i < l_iCount; ++i)
		{
			ImGui::Selectable(m_ItemTempList[i], m_SelectedIndices[i], ImGuiSelectableFlags_AllowDoubleClick);
			// Handle multi-select with modifier keys (Shift or Ctrl)
			bool l_bCtrl = true;// ImGui::IsKeyDown(ImGuiKey_RightCtrl) | ImGui::IsKeyDown(ImGuiKey_LeftCtrl);
			if (ImGui::IsItemClicked() && l_bCtrl)
			{
				m_SelectedIndices[i] = !m_SelectedIndices[i];
			}
		}
		ImGui::EndListBox();
	}
}

void cMyGuiListBox::InternalRender()
{
	if (m_bMiltiSelecteable)
	{
		RenderMultiSelectable();
		return;
	}
	if (this->m_pData->m_vSize.x > 0)
	{
		ImGui::PushItemWidth(this->m_pData->m_vSize.x);
	}
	if (m_ItemTempList.size() != m_pImguiComboxData->m_ItemList.size())
	{
		SetItemList(m_pImguiComboxData->m_ItemList);
	}
	const char** l_pItemTempList = nullptr;
	int l_iCount = (int)m_ItemTempList.size();
	if (l_iCount)
	{
		l_pItemTempList = &m_ItemTempList[0];
	}
	//this->RenderNameOnTop();
	auto l_strName = this->GetRenderText();
	if (ImGui::ListBox(l_strName.c_str(), &m_pImguiComboxData->m_iSelectedIndex, l_pItemTempList, l_iCount))
	//if (ImGui::ListBox("##listbox", &m_iSelectedIndex, l_pItemTempList, l_iCount))
	{
		if (m_fOnSelectFunction)
		{
			m_fOnSelectFunction(m_pImguiComboxData->m_iSelectedIndex);
		}
	}
	if (this->m_pData->m_vSize.x > 0)
	{
		ImGui::PopItemWidth();
	}
}

cMyGuiScroller::cMyGuiScroller()
{
	m_iSelectedIndex = 0;
}

cMyGuiScroller::~cMyGuiScroller()
{
}

void cMyGuiScroller::InternalRender()
{
	static float scroll_y = 0.0f; // Current scroll position
	int l_iSelectedIndex = m_iSelectedIndex;
	if (ImGui::BeginChild("ScrollingRegion", this->m_pData->m_vSize, true, ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		ImVec2 childPos = ImGui::GetWindowPos();
		m_RenderRect = Vector4(childPos.x,childPos.y, childPos.x+ this->m_pData->m_vSize.x, childPos.y+ this->m_pData->m_vSize.y);
		ImGui::Text(this->m_pData->m_strText.c_str());
		if (m_ItemTempList.size() != m_pImguiComboxData->m_ItemList.size())
		{
			SetItemList(m_pImguiComboxData->m_ItemList);
		}
		int l_iCount = (int)m_ItemTempList.size();
		// Display items in the scrolling region
		for (int i = 0; i < l_iCount; i++)
		{
			if (i == l_iSelectedIndex)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // Yellow for selection

			}
			if (ImGui::Selectable(m_ItemTempList[i], m_iSelectedIndex == i))
			{
				// Update selected index on click
				m_iSelectedIndex = i;
			}

			//ImGui::Image(textures[i].id, textures[i].size);
			//ImGui::Text("%s", items[i]);
			if (i == l_iSelectedIndex)
			{
				ImGui::PopStyleColor();
			}
		}

		// Track scrolling position
		scroll_y = ImGui::GetScrollY();
		ImGui::EndChild();
	}

	// Display current scroll position
#ifdef DEBUG
	ImGui::Text("Scroll Y: %.2f", scroll_y);
#endif
}

void cMyGuiScroller::GetRenderRect()
{
}

void cMyGuiScroller::RenderProperty()
{
}

cMyTreeView::cMyTreeView()
{
	this->SetName(L"cMyTreeView");
	//SetImGuiName(cMyGuiForm::TypeID);
}

cMyTreeView::~cMyTreeView()
{
}

void cMyTreeView::DisplayTree(cImGuiNode* e_pNode, bool e_bRenderVisibleCheckBox)
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
	if (m_pSelectedNode == e_pNode)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	}
	bool l_bNodeOpen = false;
	const char* l_strDragDropSourceID = "TREE_NODE";
	if (m_bDoRename && this->m_pSelectedNode == e_pNode)
	{
		l_bNodeOpen = false;
		char buffer[128];
		strncpy(buffer, this->m_pSelectedNode->GetRenderText().c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';

		// Render an input text for renaming
		if (ImGui::InputText("##Rename", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			this->m_pSelectedNode->SetImGuiName(buffer);
			m_bDoRename = false;
			m_pSelectedNode = nullptr;
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
			m_pSelectedNode = nullptr;
		}
	}
	else
	{
		l_bNodeOpen = ImGui::TreeNodeEx(l_strID.c_str(), nodeFlags, "%s", e_pNode->GetRenderText().c_str());
		if (!m_bDoRename)
		{
			if (ImGui::IsItemClicked())
			{
				m_pSelectedNodeRect[0] = ImGui::GetItemRectMin();
				m_pSelectedNodeRect[1] = ImGui::GetItemRectMax();
				m_pSelectedNode = e_pNode;
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
				DisplayTree(l_ChildrenVector[i], e_bRenderVisibleCheckBox);
			}
		}

		ImGui::TreePop();
	}
}


void cMyTreeView::RenderTreeivewPopupMenuContext()
{
	if (ImGui::BeginPopupContextWindow("bwcontextmenu"))
	{
		if (ImGui::MenuItem("Copy"))
		{
			m_pCopyNode = this->m_pSelectedNode;
		}
		if (ImGui::MenuItem("Paste"))
		{

		}
		if (ImGui::MenuItem("Cut"))
		{

		}
		if (ImGui::MenuItem("Delete"))
		{
			if (this->m_pSelectedNode)
			{
				if (this->m_pSelectedNode->Type() != cMyGuiForm::TypeID)
				{
					this->m_pSelectedNode->SetParent(nullptr);
					SAFE_DELETE(this->m_pSelectedNode);
				}
			}
			this->m_pSelectedNode = nullptr;
			m_pCopyNode = nullptr;
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
	if (ImGui::Begin("Tree View Window"), nullptr, m_iRenderFlag)
	{

		// Set the size of the tree view region
		//ImVec2 treeViewSize = ImVec2(300, 400); // Width: 300px, Height: 400px
		ImVec2 treeViewSize = ImGui::GetContentRegionAvail();
		//m_bCollided = ImGui::IsWindowHovered();
		m_vPosition = ImGui:: GetWindowPos();
		m_vSize = ImGui::GetWindowSize();
		this->m_bCollided = CheckMouseAndCurrentWindowCollision();
		// Begin a child region to contain the tree
		ImGui::BeginChild("TreeViewRegion", treeViewSize, true, ImGuiWindowFlags_AlwaysVerticalScrollbar| ImGuiWindowFlags_AlwaysHorizontalScrollbar);
		// Display the tree starting from the root node
		if (m_pRoot)
		{
			this->DisplayTree(m_pRoot, true);
		}
		this->RenderTreeivewPopupMenuContext();
		// End the child region
		ImGui::EndChild();
	}
	ImGui::End();

	if (m_pDragNode && m_pDropParent)
	{
		m_pDragNode->SetParent(m_pDropParent, m_iDropIndex);
		m_pSelectedNode = nullptr;
	}
	if (m_pSelectedNode && ImGui::IsKeyPressed(ImGuiKey_F2))
	{
		auto l_MousePos = cGameApp::m_sMousePosition;
		Vector4 l_vRect(m_pSelectedNodeRect[0].x, m_pSelectedNodeRect[0].y, m_pSelectedNodeRect[1].x, m_pSelectedNodeRect[1].y);
		if(l_vRect.CollidePoint((int)l_MousePos.x, (int)l_MousePos.y))
		//if (ImGui::IsMouseHoveringRect(m_pSelectedNodeRect[0], m_pSelectedNodeRect[1]))
		{
			m_bDoRename = true;
		}
	}
}

bool cMyTreeView::IsCollided(int e_iPosX, int e_iPosY)
{
	if (m_bCollided)
	{
		int a = 0;
	}
	return m_bCollided;
	Vector4 l_vRect(m_vPosition.x, m_vPosition.y, m_vPosition.x+ m_vSize.x, m_vPosition.y+ m_vSize.y);
	auto l_bResult = l_vRect.CollidePoint(e_iPosX,e_iPosY);
	if (l_bResult)
	{
		int a = 0;
	}
	return l_bResult;
}

cMyGuiBasicObj* GetMyGuiObj(eMyImGuiType e_eMyImGuiType)
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
		case	eMIGT_ROOT_NODE:
		l_pObject = new cMyGuiRootNode();
		break;
		case	eMIGT_SCROLLER:
		l_pObject = new cMyGuiScroller();
		break;
		case eMIGT_DATA_PICKER:
		l_pObject = new cMyGuiDatePicker();
		break;
	}
	//e_pParent->add
	if (l_pObject)
	{
		l_pObject->CreateImguiDataData();
		l_pObject->SetName(l_pObject->Type());
		l_pObject->SetImGuiName(l_pObject->Type());
	}
	return l_pObject;
}



cImGuiNode* GetImGuiNodeByType(const wchar_t* e_strType)
{
	auto l_IT = g_ImguiTypeNameAndType.find(e_strType);
	if (l_IT != g_ImguiTypeNameAndType.end())
	{
		return GetMyGuiObj(l_IT->second);
	}
	return nullptr;
}

const char* GetMyGuiObjLabel(eMyImGuiType e_eMyImGuiType)
{
	//eMIGT_NODE = 0,
	//eMIGT_BUTTON,// = 0,
	//eMIGT_LABEL,
	//eMIGT_EDIT_BOX,
	//eMIGT_SLIDER_I,
	//eMIGT_SLIDER_F,
	//eMIGT_CHECKBOX,
	//eMIGT_RADIO,
	//eMIGT_TOOGLE,
	//eMIGT_FORM,//9
	//eMIGT_PANEL = 10,
	//eMIGT_COMBO_BOX,
	//eMIGT_LIST_BOX,
	//eMIGT_ROOT_NODE,
	//eMIGT_SCROLLER,
	const char* l_strTypeAndLabel[] =
	{
		"Node",
		"Button",
		"Label",
		"EditBox",
		"SLiderIntger",
		"SLiderFloat",
		"CheckBox",
		"Radio",
		"Toogle",
		"Form",
		"Panel",
		"ComboBox",
		"ListBox",
		"RootNode",
		"Scroller",
		"DatePicker"
	};
	return l_strTypeAndLabel[e_eMyImGuiType];
}

void cMyGuiDatePicker::InternalRender()
{
	ImGui::PushItemWidth(m_pData->m_vSize.x);
	if(DatePicker(this->m_pImguiDatePickerData->m_strText.c_str(), this->m_pImguiDatePickerData->m_iYear, this->m_pImguiDatePickerData->m_iMonth, this->m_pImguiDatePickerData->m_iDay))
	{

	}
	ImVec2 l_vWorldPos = this->GetWorldPosition();
	const int l_ciMagixValue = 4;
	m_RenderRect = Vector4(l_vWorldPos.x, l_vWorldPos.y, l_vWorldPos.x + this->m_pData->m_vSize.x* l_ciMagixValue, l_vWorldPos.y + this->m_pData->m_vSize.y);
	ImGui::PopItemWidth();
}

void cMyGuiDatePicker::GetRenderRect()
{

}

void cMyGuiDatePicker::RenderProperty()
{
}

cMyGuiDatePicker::cMyGuiDatePicker()
{
}

cMyGuiDatePicker::~cMyGuiDatePicker()
{
}
