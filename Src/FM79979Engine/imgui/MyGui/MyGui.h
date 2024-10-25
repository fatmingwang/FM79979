#pragma once
#include "../imgui.h"
#include <vector>
#include <string>


enum eMyImGuiType
{
	eMIGT_NODE = 0,
	eMIGT_BUTTON = 1,
	eMIGT_LABEL,
	eMIGT_EDIT,
	eMIGT_SLIDER_I,
	eMIGT_SLIDER_F,
	eMIGT_CHECKBOX,
	eMIGT_RADIO,
	eMIGT_TOOGLE,
	eMIGT_FORM,//9
	eMIGT_CHILD = 10,
	eMIGT_MAX
};




enum type_obj
{
	button = 1,
	label,
	slider_i,
	slider_f,
	checkbox,
	radio,
	toggle
};
enum class resize_opt
{
	off,
	bottom_right,
	top_left,
	top_right,
	bottom_left,
	top,
	bottom,
	left,
	right
};


class cImGuiNode
{
	bool						m_bPosDirty = false;
	ImVec2						m_vLocalPos = { 0, 0 };
	ImVec2						m_vWorldPos = { 0, 0 };
	void						SetCachedWorldTransformDirty();
	void						UpdateCachedWorldTransformIfNeeded();
	cImGuiNode*					m_pParent;
public:
	cImGuiNode();
	~cImGuiNode();
	std::vector<cImGuiNode*>	m_ChildNodeVector;
	int							m_iID = 0;
	std::string					m_strName = "Node";
	eMyImGuiType				m_eType = eMIGT_NODE;
	ImVec2 						GetLocalPosition() { return m_vLocalPos;}
	void						SetLocalPosition(const ImVec2& vLocalPos);
	ImVec2						GetWorldPosition();
	//-1 to last one
	void						SetParent(cImGuiNode* e_pParent,int e_iIndex = -1);
	cImGuiNode*					GetParent(){return this->m_pParent;};
	static void					DeleteObjectAndAllChildren(cImGuiNode*e_pImGuiNode);
};


class cMyGuiMouseMovingData
{
public:
	bool		change_pos = false;
	bool		hover = false;
	bool		delete_me = false;
	bool		selected = false;
	bool		locked = false;
};

class basic_obj:public cImGuiNode,public cMyGuiMouseMovingData
{
public:
	ImVec2		m_vSize = {0,0 };
	ImVec2		m_vPos = { 0,0 };
	ImVec2		m_vSizeObj = {0,0 };
};

struct child:public cMyGuiMouseMovingData
{
	int			id = 0;
	std::string name = "form";
	int			father = 0;
	bool		border = true;
	ImVec2		size = { };
	ImVec2		pos = { };

};

struct form
{
	int			id = 0;
	std::string name = "form";
	ImVec2		size = { };
	ImVec2		pos = { };
	uint32_t	type_me = 0;

	// AGAIN REPEAT FIX THAT!
	bool		change_pos = false;
	bool		hover = false;
	bool		delete_me = false;
	std::vector<child> child{ };
};



/// <returns></returns>
bool KeyPressed(int e_iK);
bool HoldingKey(int key);
bool PressedBindingKeys(int key_1, int key_2);
