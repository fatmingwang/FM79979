#pragma once
#include "../imgui.h"
#include <vector>
#include <string>


enum eMyImGuiType
{
	eMIGT_NODE = 0,
	eMIGT_BUTTON = 1,
	eMIGT_LABEL,
	eMIGT_EDIT_BOX,
	eMIGT_SLIDER_I,
	eMIGT_SLIDER_F,
	eMIGT_CHECKBOX,
	eMIGT_RADIO,
	eMIGT_TOOGLE,
	eMIGT_FORM,//9
	eMIGT_PANEL = 10,
	eMIGT_MAX
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
	virtual	void				EndRender(){}
	virtual	void				InternalRender(){}
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
	void						Render();// = 0;
	static void					DeleteObjectAndAllChildren(cImGuiNode*e_pImGuiNode);
};


class cMyGuiMouseMovingData
{
public:
	bool		change_pos = false;
	bool		hover = false;
	bool		selected = false;
	bool		locked = false;
};

class cMyGuiBasicObj:public cImGuiNode,public cMyGuiMouseMovingData
{
	virtual	void				InternalRender(){}
public:
	ImVec2		m_vSize = {0,0 };
	ImVec2		m_vPos = { 0,0 };
	ImVec2		m_vSizeObj = {0,0 };
	void		RenderProperty();
};

//eMIGT_NODE = 0,
//eMIGT_BUTTON = 1,
//eMIGT_LABEL,
//eMIGT_EDIT,
//eMIGT_SLIDER_I,
//eMIGT_SLIDER_F,
//eMIGT_CHECKBOX,
//eMIGT_RADIO,
//eMIGT_TOOGLE,
//eMIGT_FORM,//9
//eMIGT_PANEL = 10,

class cMyGuiNode :public cMyGuiBasicObj
{
	virtual	void				InternalRender() {}
public:
};

class cMyGuiButton :public cMyGuiBasicObj
{
	virtual	void				InternalRender() {}
public:
};

class cMyGuiLabel :public cMyGuiBasicObj
{
	virtual	void				InternalRender() {}
public:
};

class cMyGuiEditBox :public cMyGuiBasicObj
{
	virtual	void				InternalRender() {}
public:
};

class cMyGuiSliderInteger :public cMyGuiBasicObj
{
	virtual	void				InternalRender() {}
public:
};

class cMyGuiSliderFloatValue :public cMyGuiBasicObj
{
	virtual	void				InternalRender() {}
public:
};

class cMyGuiCheckBox :public cMyGuiBasicObj
{
	virtual	void				InternalRender() {}
public:
};

class cMyGuiRadio :public cMyGuiBasicObj
{
	virtual	void				InternalRender() {}
public:
};

class cMyGuiToogle:public cMyGuiBasicObj
{
	virtual	void				InternalRender() {}
public:
	bool		border = true;

};

class cMyGuiForm :public cMyGuiBasicObj
{
	virtual	void				InternalRender() {}
public:

};

class cMyGuiPanel :public cMyGuiBasicObj
{
	virtual	void				InternalRender() {}
public:

};


cMyGuiBasicObj* GetMyGuiObj(eMyImGuiType e_eMyImGuiType);