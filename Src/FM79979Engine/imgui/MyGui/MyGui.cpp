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

ImVec2					cMyGuiBasicObj::m_vPropertyPos = ImVec2(1400, 10);
ImVec2					cMyGuiBasicObj::m_vPropertySize = ImVec2(300, 600);
bool					cMyGuiBasicObj::m_bFirstUpdate = true;

std::function<void(cImGuiNode*)>	cImGuiNode::m_sfParseFunction = nullptr;

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

using json = nlohmann::json;

cImGuiNode::cImGuiNode()
{
	//from_json(old_json, data); // This will ignore missing keys and use default values

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
	ImVec4 l_vColor = this->m_pData->m_vColor;
	if (l_vColor.w != 0)
	{
		ImGui::PushStyleColor(this->m_pData->m_ImguiStyleColorType, l_vColor);
	}
	this->InternalRender();
	if (l_vColor.w != 0)
	{
		ImGui::PopStyleColor();
	}
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

cImGuiNode* cImGuiNode::FindNodeByUID(int e_iID)
{
	if (this->m_pData->m_iID == e_iID)
	{
		return this;
	}
	for (auto l_pChild : m_ChildNodeVector)
	{
		auto l_pResult = l_pChild->FindNodeByUID(e_iID);
		if (l_pResult)
		{
			return l_pResult;
		}
	}
	return nullptr; // Not found
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
	//from_json(e_Json, *m_pData);
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
	this->m_pData->AfterSerialize();
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
		if (m_sfParseFunction)
		{
			m_sfParseFunction(l_ImguiNode);
		}
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

void	cMyGuiBasicObj::LazyColorSlider(const char* e_strLabel, ImVec4& e_vColor)
{
	//IMGUI_API bool          ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
	//IMGUI_API bool          ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
	ImGui::TextDisabled(e_strLabel);
	ImGui::ColorEdit4(e_strLabel,(float*)&e_vColor);
	//ImGui::ColorPicker4(e_strLabel, (float*)&e_vColor);
	//ImGui::SliderFloat("R", &e_vColor.x, 0, 1);
	//ImGui::SliderFloat("G", &e_vColor.y, 0, 1);
	//ImGui::SliderFloat("B", &e_vColor.z, 0, 1);
	//ImGui::SliderFloat("A", &e_vColor.w, 0, 1);
}

void cMyGuiBasicObj::InnerRenderProperty()
{
	bool l_my_forms_active = ImGui::IsWindowFocused();
	ImGui::InputTextEx("NewText", &this->m_pData->m_strText, 0);
	////ImGui::InputText("name form", name, 255);
	//if (ImGui::Button("Apply Text"))
	//{
	//	//if (!this->m_pData->m_strImGuiName.empty())
	//	{
	//		this->m_pData->m_strText = this->m_pData->m_strTempText;
	//	}
	//}
	//ImVec2		l_vSize = m_vSize;
	ImVec2		l_vPos = GetLocalPosition();
	//ImVec2		l_vSizeObj = m_vSizeObj;
	RenderHintLabel("0 is auto","Position & Size(?)");
	ImGui::InputFloat("Width", &m_pData->m_vSize.x, 1);
	ImGui::InputFloat("Height", &m_pData->m_vSize.y, 1);
	ImGui::InputFloat("PosX", &l_vPos.x, 1);
	ImGui::InputFloat("PosY", &l_vPos.y, 1);
	if (m_pData->m_vLocalPos.x!= l_vPos.x || m_pData->m_vLocalPos.y != l_vPos.y)
	{
		this->SetLocalPosition(l_vPos);
	}
	LazyColorSlider("Color", m_pData->m_vColor);
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
	if (m_bFirstUpdate)
	{
		m_bFirstUpdate = false;
		ImGui::SetNextWindowPos(m_vPropertyPos);
		ImGui::SetNextWindowSize(m_vPropertySize);
	}
	ImGui::Begin("property", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus);
	InnerRenderProperty();
	ImGui::End();

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



cMyGuiForm::cMyGuiForm()
{
	m_bShowCloseCutton = false;
	this->m_bThisUseContainerPositionDontApplyParentPositionToChild = true;
}

cMyGuiForm::~cMyGuiForm()
{
}
void cMyGuiForm::InnerRenderProperty()
{
	cMyGuiBasicObj::InnerRenderProperty();
	bool l_bBorder = false;;
	bool l_bLock = false;;
	ImGui::Checkbox("Border", &l_bBorder);
	ImGui::SameLine();
	ImGui::Checkbox("Lock", &l_bLock);
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

cMyGuiToogle::cMyGuiToogle()
{
	m_bChecked = false;
}

cMyGuiToogle::~cMyGuiToogle()
{
}

void cMyGuiToogle::InternalRender()
{
	bool l_bPrevious = m_bChecked;
	ImGui::Text(this->m_pData->m_strText.c_str());
	ImGui::SameLine();
	ImGui::ToggleButton(this->m_pData->m_strText.c_str(), &m_bChecked);

	if (l_bPrevious != m_bChecked && m_fToogleChangedFunction)
	{
		m_fToogleChangedFunction(m_bChecked);
	}
}


cMyGuiRadio::cMyGuiRadio()
{
}

cMyGuiRadio::~cMyGuiRadio()
{
}

void cMyGuiRadio::InnerRenderProperty()
{
	cMyGuiBasicObj::InnerRenderProperty();
}

void cMyGuiRadio::InternalRender()
{
	if (ImGui::RadioButton(this->m_pData->m_strText.c_str(), m_pRadioData->m_bChecked))
	{
		m_pRadioData->m_bChecked = !m_pRadioData->m_bChecked;
	}
}

cMyGuiCheckBox::cMyGuiCheckBox()
{

}

cMyGuiCheckBox::~cMyGuiCheckBox()
{
}

void cMyGuiCheckBox::InternalRender()
{
	ImGui::Checkbox(this->m_pData->m_strText.c_str(), &m_pCheckBoxData->m_bChecked);
}

void cMyGuiCheckBox::InnerRenderProperty()
{
	cMyGuiBasicObj::InnerRenderProperty();
	ImGui::Text("DefaultStatus");
	ImGui::SameLine();
	ImGui::ToggleButton("Default", &m_pCheckBoxData->m_bChecked);
}

cMyGuiSliderFloatValue::cMyGuiSliderFloatValue()
{
}

cMyGuiSliderFloatValue::~cMyGuiSliderFloatValue()
{
}

void cMyGuiSliderFloatValue::InnerRenderProperty()
{
	cMyGuiBasicObj::InnerRenderProperty();
	ImGui::InputFloat("Min", &this->m_pSliderData->m_fMin, 1);
	ImGui::InputFloat("Max", &this->m_pSliderData->m_fMax, 1);
	ImGui::InputFloat("Value", &this->m_pSliderData->m_fValue, 1);
}

void cMyGuiSliderFloatValue::InternalRender()
{
	ImGui::PushItemWidth(m_pData->m_vSize.x);
	ImGui::SliderFloat(this->m_pData->m_strImGuiName.c_str(), &this->m_pSliderData->m_fValue, m_pSliderData->m_fMin, m_pSliderData->m_fMax);
	ImGui::PopItemWidth();
}


cMyGuiSliderInteger::cMyGuiSliderInteger()
{
}

cMyGuiSliderInteger::~cMyGuiSliderInteger()
{
}

void cMyGuiSliderInteger::InnerRenderProperty()
{
	cMyGuiBasicObj::InnerRenderProperty();
	ImGui::InputInt("Min", &this->m_pSliderData->m_iMin, 1);
	ImGui::InputInt("Max", &this->m_pSliderData->m_iMax, 1);
	ImGui::InputInt("Value", &this->m_pSliderData->m_iValue, 1);
	
}

void cMyGuiSliderInteger::InternalRender()
{
	ImGui::PushItemWidth(m_pData->m_vSize.x);
	ImGui::SliderInt(this->m_pData->m_strImGuiName.c_str(), &this->m_pSliderData->m_iValue, this->m_pSliderData->m_iMin, this->m_pSliderData->m_iMax);
	ImGui::PopItemWidth();
}

cMyGuiEditBox::cMyGuiEditBox()
{
}

void cMyGuiEditBox::InnerRenderProperty()
{
	cMyGuiBasicObj::InnerRenderProperty();
	ImGui::InputTextEx("Hint Text", &this->m_pEditBoxData->m_strHint, 0);
	ImGui::InputTextEx("Label", &this->m_pEditBoxData->m_strLabel, 0);
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
	auto l_strID = "##" + this->m_pEditBoxData->m_strLabel;
	float cursor_x = ImGui::GetCursorPosX();
	ImGui::Text(this->m_pEditBoxData->m_strLabel.c_str());
	ImGui::SetCursorPosX(cursor_x);
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
			auto l_strID = "##"+this->m_pEditBoxData->m_strLabel;
			if (ImGui::InputTextWithHint(l_strID.c_str(), m_pEditBoxData->m_strHint.c_str(), &this->m_pData->m_strText, this->m_pEditBoxData->m_RenderFlag))
			{
				m_bTextChanged = true;
			}
		}
		else
		{

	//InputText(const char* label, std::string * str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
			//ImGui::InputText(const char* label, std::string * str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
			if (ImGui::InputText("Input", &this->m_pData->m_strText, this->m_pEditBoxData->m_RenderFlag, InputCallback, nullptr))
			{
				m_bTextChanged = true;
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
	if (m_bTextChanged && m_fContentChangedFunction)
	{
		m_fContentChangedFunction(this->m_pData->m_strText);
	}
	m_bTextChanged = false;
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
	if (ImGui::InputTextMultiline("##multiline", &this->m_pData->m_strText, this->m_pData->m_vSize, this->m_pEditBoxData->m_RenderFlag))
	{
		//if (l_iLength*2 >= l_iCapacity)
		//{
		//	m_strText.reserve(l_iCapacity * 2); // Double the buffer size
		//	m_strText.resize(strlen(m_strText.c_str()));
		//}
		m_bTextChanged = true;
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
	//ImVec4 l_vColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
	if (ImGui::Button(this->m_pData->m_strText.c_str(), this->m_pData->m_vSize))
	{
		if (m_fOnClickFunction)
		{
			m_fOnClickFunction(this);
		}
	}
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
			std::string l_strSelectedItem;
			if (m_pImguiComboxData->m_iSelectedIndex != -1)
			{
				l_strSelectedItem = m_pImguiComboxData->m_ItemList[m_pImguiComboxData->m_iSelectedIndex];
			}
			m_fOnSelectFunction(m_pImguiComboxData->m_iSelectedIndex, l_strSelectedItem);
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

void cMyGuiComboBox::InnerRenderProperty()
{
	cMyGuiBasicObj::InnerRenderProperty();

	ImGui::TextDisabled("ItemList");
	// Example string array
	std::vector<std::string>& stringArray = m_pImguiComboxData->m_ItemList;

	// ImGui expects a const char* array, so create one
	if (m_pImguiComboxData->m_ItemListTemplate.size() != stringArray.size())
	{
		m_pImguiComboxData->m_ItemListTemplate.clear();
		for (const auto& str : stringArray)
		{
			m_pImguiComboxData->m_ItemListTemplate.push_back(str.c_str());
		}
	}

	ImGui::InputInt("Set Array Count", &m_pImguiComboxData->newCount);

	if (m_pImguiComboxData->newCount < 1) m_pImguiComboxData->newCount = 1; // Prevent negative sizes

	if (ImGui::Button("Apply New Count"))
	{
		m_pImguiComboxData->renaming = false;
		m_pImguiComboxData->currentIndex = 0;
		if (m_pImguiComboxData->newCount != stringArray.size())
		{
			stringArray.resize(m_pImguiComboxData->newCount);
			for (int i = 0; i < m_pImguiComboxData->newCount; ++i)
			{
				if (stringArray[i].empty())
				{
					stringArray[i] = "New Item " + std::to_string(i + 1);
				}
			}
			m_pImguiComboxData->m_ItemListTemplate.clear();
			for (const auto& str : stringArray)
			{
				m_pImguiComboxData->m_ItemListTemplate.push_back(str.c_str());
			}
		}
	}

	// ListBox for items
	if (ImGui::ListBox("Items", &m_pImguiComboxData->currentIndex, m_pImguiComboxData->m_ItemListTemplate.data(), (int)m_pImguiComboxData->m_ItemListTemplate.size()))
	{
		// Handle selection change
		printf("ListBox Selected: %s\n", stringArray[m_pImguiComboxData->currentIndex].c_str());
	}

	// Display currently selected item
	ImGui::Text("Selected Item: %s", stringArray[m_pImguiComboxData->currentIndex].c_str());

	bool l_bFocus = false;
	if (ImGui::IsKeyPressed(ImGuiKey_F2) && m_pImguiComboxData->currentIndex >= 0 && m_pImguiComboxData->currentIndex < stringArray.size())
	{
		l_bFocus = true;
		m_pImguiComboxData->renaming = true;
		snprintf(m_pImguiComboxData->renameBuffer, sizeof(m_pImguiComboxData->renameBuffer), "%s", stringArray[m_pImguiComboxData->currentIndex].c_str());
	}

	if (m_pImguiComboxData->renaming && m_pImguiComboxData->currentIndex >= 0 && m_pImguiComboxData->currentIndex < stringArray.size())
	{
		if (ImGui::InputText("Rename Selected", m_pImguiComboxData->renameBuffer, sizeof(m_pImguiComboxData->renameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			stringArray[m_pImguiComboxData->currentIndex] = m_pImguiComboxData->renameBuffer;
			m_pImguiComboxData->m_ItemListTemplate[m_pImguiComboxData->currentIndex] = stringArray[m_pImguiComboxData->currentIndex].c_str();
			m_pImguiComboxData->renaming = false;
		}
	}
	// Automatically focus the input field when renaming starts
	if (ImGui::IsItemActive() == false && l_bFocus)
	{
		ImGui::SetKeyboardFocusHere(-1);
	}

	// Delete functionality on Delete key
	if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_pImguiComboxData->currentIndex >= 0 && m_pImguiComboxData->currentIndex < stringArray.size())
	{
		m_pImguiComboxData->renaming = false;
		stringArray.erase(stringArray.begin() + m_pImguiComboxData->currentIndex);
		m_pImguiComboxData->m_ItemListTemplate.clear();
		for (const auto& str : stringArray)
		{
			m_pImguiComboxData->m_ItemListTemplate.push_back(str.c_str());
		}
		if (m_pImguiComboxData->currentIndex >= stringArray.size())
		{
			m_pImguiComboxData->currentIndex = (int)stringArray.size() - 1; // Adjust index if necessary
		}
	}
}

cMyGuiListBox::cMyGuiListBox()
{
	m_bMiltiSelecteable = false;
}

cMyGuiListBox::~cMyGuiListBox()
{
}

void cMyGuiListBox::InnerRenderProperty()
{
	cMyGuiComboBox::InnerRenderProperty();
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
			std::string l_strSelectedItem;
			if (m_pImguiComboxData->m_iSelectedIndex != -1)
			{
				l_strSelectedItem = m_pImguiComboxData->m_ItemList[m_pImguiComboxData->m_iSelectedIndex];
			}
			m_fOnSelectFunction(m_pImguiComboxData->m_iSelectedIndex, l_strSelectedItem);
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
	int l_iSelectedIndex = m_iSelectedIndex;
	if (ImGui::BeginChild("ScrollingRegion", this->m_pData->m_vSize, true, ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		ImVec2 childPos = ImGui::GetWindowPos();
		m_RenderRect = Vector4(childPos.x,childPos.y, childPos.x+ this->m_pData->m_vSize.x, childPos.y+ this->m_pData->m_vSize.y);
		//ImGui::Text(this->m_pData->m_strText.c_str());
		//ImGui::TextDisabled(this->m_pData->m_strText.c_str());
		ImGui::TextColored(ImVec4(1,1,0,1), this->m_pData->m_strText.c_str());
		
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
		m_fScrollY = ImGui::GetScrollY();
		ImGui::EndChild();
	}

	// Display current scroll position
#ifdef DEBUG
	ImGui::Text("Scroll Y: %.2f", m_fScrollY);
#endif
}

void cMyGuiScroller::GetRenderRect()
{
}

void cMyGuiScroller::InnerRenderProperty()
{
	cMyGuiComboBox::InnerRenderProperty();
}
cMyGuiBasicObj* GetMyGuiObj(eMyImGuiType e_eMyImGuiType)
{
	cMyGuiBasicObj* l_pObject = nullptr;
	ImVec4 l_vColor = { 0,0,0,0 };
	const int l_ciFailedType = -1;
	int l_iStyleType = l_ciFailedType;
	switch (e_eMyImGuiType)
	{
		//case	eMIGT_NODE:
		//	l_pObject = new cMyGuiRootNode();
		//	break;
		case	eMIGT_BUTTON:
		l_iStyleType = ImGuiCol_Button;
		l_pObject = new cMyGuiButton();
		break;
		case	eMIGT_LABEL:
		l_iStyleType = ImGuiCol_Text;
		l_pObject = new cMyGuiLabel();
		break;
		case	eMIGT_EDIT_BOX:
		l_iStyleType = ImGuiCol_Text;
		l_pObject = new cMyGuiEditBox();
		break;
		case	eMIGT_SLIDER_I:
		l_iStyleType = ImGuiCol_SliderGrab;
		l_pObject = new cMyGuiSliderInteger();
		break;
		case	eMIGT_SLIDER_F:
		l_iStyleType = ImGuiCol_SliderGrab;
		l_pObject = new cMyGuiSliderFloatValue();
		break;
		case	eMIGT_CHECKBOX:
		l_iStyleType = ImGuiCol_CheckMark;
		l_pObject = new cMyGuiCheckBox();
		break;
		case	eMIGT_RADIO:
		l_iStyleType = ImGuiCol_CheckMark;
		l_pObject = new cMyGuiRadio();
		break;
		case	eMIGT_TOOGLE:
		l_pObject = new cMyGuiToogle();
		break;
		case	eMIGT_FORM:
		l_iStyleType = ImGuiCol_WindowBg;
		l_pObject = new cMyGuiForm();
		break;//9
		case	eMIGT_PANEL:
		l_iStyleType = ImGuiCol_ChildBg;
		l_pObject = new cMyGuiPanel();
		break;
		case	eMIGT_COMBO_BOX:
		l_iStyleType = ImGuiCol_FrameBg;
		l_pObject = new cMyGuiComboBox();
		break;
		case	eMIGT_LIST_BOX:
		l_pObject = new cMyGuiListBox();
		break;
		case	eMIGT_ROOT_NODE:

		l_pObject = new cMyGuiRootNode();
		break;
		case	eMIGT_SCROLLER:
		l_iStyleType = ImGuiCol_ScrollbarBg;
		l_pObject = new cMyGuiScroller();
		break;
		case eMIGT_DATA_PICKER:
		l_pObject = new cMyGuiDatePicker();
		break;
		default:
		//e_eMyImGuiType
		break;
	}
	if (l_iStyleType != l_ciFailedType)
	{
		l_vColor = ImGui::GetStyleColorVec4(l_iStyleType);
	}
	
	//e_pParent->add
	if (l_pObject)
	{
		l_pObject->CreateImguiDataData();
		l_pObject->m_pData->m_vColor = l_vColor;
		l_pObject->m_pData->m_ImguiStyleColorType = l_iStyleType;
		l_pObject->SetName(l_pObject->Type());
		l_pObject->SetImGuiName(l_pObject->Type());
	}
	if (e_eMyImGuiType == eMIGT_EDIT_BOX)
	{
		l_pObject->m_pData->m_strText = "";
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
	bool l_bDataChanged = false;
	if(DatePicker(this->m_pImguiDatePickerData->m_strText.c_str(), this->m_pImguiDatePickerData->m_iYear, this->m_pImguiDatePickerData->m_iMonth, this->m_pImguiDatePickerData->m_iDay))
	{
		l_bDataChanged = true;
	}
	ImVec2 l_vWorldPos = this->GetWorldPosition();
	const int l_ciMagixValue = 4;
	m_RenderRect = Vector4(l_vWorldPos.x, l_vWorldPos.y, l_vWorldPos.x + this->m_pData->m_vSize.x* l_ciMagixValue, l_vWorldPos.y + this->m_pData->m_vSize.y);
	ImGui::PopItemWidth();
	if (m_strDate.length() == 0 || l_bDataChanged)
	{
		//year month day
		m_strDate = ValueToString(m_pImguiDatePickerData->m_iYear) + "-" + ValueToString(m_pImguiDatePickerData->m_iMonth) + "-" + ValueToString(m_pImguiDatePickerData->m_iDay);
		if (m_fDateChangedFunction)
		{
			m_fDateChangedFunction(m_strDate);
		}
	}
}

void cMyGuiDatePicker::GetRenderRect()
{

}

void cMyGuiDatePicker::InnerRenderProperty()
{
}

cMyGuiDatePicker::cMyGuiDatePicker()
{
}

cMyGuiDatePicker::~cMyGuiDatePicker()
{
}

std::string cMyGuiDatePicker::GetDateString()
{
	return std::string();
}
