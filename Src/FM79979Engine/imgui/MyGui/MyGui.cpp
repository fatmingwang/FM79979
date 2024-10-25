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

#ifdef WIN32
bool KeyPressed(int e_iK)
{
	auto l_Key = GetAsyncKeyState(e_iK);
	return (GetFocus() == cGameApp::m_spOpenGLRender->m_Handle) && (l_Key & 0x8000);
}
bool HoldingKey(int key)
{
	return (GetFocus() == cGameApp::m_spOpenGLRender->m_Handle) && GetAsyncKeyState(key);
}

bool PressedBindingKeys(int key_1, int key_2)
{
	return (GetFocus() == cGameApp::m_spOpenGLRender->m_Handle) && (GetAsyncKeyState(key_1)) && (GetAsyncKeyState(key_2) & 1);
}
#else
bool KeyPressed(int e_iK)
{
	return false;
}
bool HoldingKey(int key)
{
	return false;
}

bool PressedBindingKeys(int key_1, int key_2)
{
	return false;
}
#endif