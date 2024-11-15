#pragma once

#include "../../Core/AllCommonInclude.h"
#include "../imgui.h"
#include <vector>
#include <string>

#include <functional>


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
	eMIGT_COMBO_BOX,
	eMIGT_LIST_BOX,
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

typedef std::function<void(class cImGuiNode*)> f_MyImGuiExtraRenderFunction;
class cImGuiNode:public NamedTypedObject
{
	f_MyImGuiExtraRenderFunction	m_ExtraRenderFunction = nullptr;
	//
	virtual void				ApplyPosition() = 0;
protected:
	bool						m_bDoApplyPosition = false;
	//
	void						HierachyPositionRender();
	virtual	void				EndRender() = 0;
	virtual	void				InternalRender() = 0;
	bool						m_bPosDirty = false;
	ImVec2						m_vLocalPos = { 0, 0 };
	ImVec2						m_vWorldPos = { 0, 0 };
	void						SetCachedWorldTransformDirty();
	void						UpdateCachedWorldTransformIfNeeded();
	cImGuiNode*					m_pParent;
	std::vector<cImGuiNode*>	m_ChildNodeVector;
	////not every component suppot this.
	//GET_SET_DEC(Vector2,m_vSize,GetSize,SetSize);
	//void						ApplySize(bool&e_bWidth, bool& e_bHeight);
	//for editor mode
	GET_SET_DEC(bool,m_bOnlyApplyPositionOnceForDragMoving, GetOnlyApplyPositionOnceForDragMoving, SetOnlyApplyPositionOnceForDragMoving);
public:
	DEFINE_TYPE_INFO();
	cImGuiNode();
	~cImGuiNode();
	int							m_iID = 0;
	std::string					m_strName = "Node";
	eMyImGuiType				m_eType = eMIGT_NODE;
	ImVec2 						GetLocalPosition() { return m_vLocalPos;}
	void						SetLocalPosition(const ImVec2& e_vLocalPos);
	ImVec2						GetWorldPosition();
	//-1 to last one
	void						SetParent(cImGuiNode* e_pParent,int e_iChildIndex = -1);
	cImGuiNode*					GetParent(){return this->m_pParent;};
	void						AddChild(cImGuiNode* e_pChild, int e_iChildIndex = -1);
	bool						SwapChild(int e_iIndex1, int e_iIndex2);
	void						Render();// = 0;
	static void					DeleteObjectAndAllChildren(cImGuiNode*e_pImGuiNode);
	void						SetExtraRenderFunction(f_MyImGuiExtraRenderFunction e_MyImGuiExtraRenderFunction) { m_ExtraRenderFunction = e_MyImGuiExtraRenderFunction; }
};


class cMyGuiMouseMovingData
{
public:
	bool		m_ChangePos = false;
	bool		m_bHover = false;
	bool		m_bSelected = false;
	bool		m_bLocked = false;
};

class cMyGuiBasicObj:public cImGuiNode,public cMyGuiMouseMovingData
{
	virtual void					ApplyPosition()override;
	virtual	void					EndRender(){}
	virtual	void					InternalRender(){}
	virtual void					RenderBaseProperty();
	GET_SET_DEC(ImVec2, m_vSize, GetSize, SetSize);
public:
	cMyGuiBasicObj();
	virtual ~cMyGuiBasicObj();
	ImVec2						m_vSizeObj = {0,0 };
	virtual void				RenderProperty();
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
	virtual void		ApplyPosition()override;
	virtual	void		InternalRender()override;
public:
	DEFINE_TYPE_INFO();
	//virtual void		RenderProperty()override;
};

class cMyGuiButton :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	GET_SET_DEC(std::string,m_strText,GetText,SetText);
public:
	DEFINE_TYPE_INFO();
	cMyGuiButton();
	virtual ~cMyGuiButton();
	//virtual void		RenderProperty()override;
};

class cMyGuiLabel :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
public:
	DEFINE_TYPE_INFO();
	//virtual void		RenderProperty()override;
};

class cMyGuiEditBox :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
public:
	DEFINE_TYPE_INFO();
	//virtual void		RenderProperty()override;
};

class cMyGuiSliderInteger :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
public:
	DEFINE_TYPE_INFO();
	//virtual void		RenderProperty()override;
};

class cMyGuiSliderFloatValue :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
public:
	DEFINE_TYPE_INFO();
	//virtual void		RenderProperty()override;
};

class cMyGuiCheckBox :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
public:
	DEFINE_TYPE_INFO();
	//virtual void		RenderProperty()override;
};

class cMyGuiRadio :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
public:
	DEFINE_TYPE_INFO();
	//virtual void		RenderProperty()override;
};

class cMyGuiToogle:public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
public:
	DEFINE_TYPE_INFO();
	bool		border = true;
	virtual void		RenderProperty()override;

};

class cMyGuiForm :public cMyGuiBasicObj
{
	virtual void		ApplyPosition()override;
	virtual	void		InternalRender()override;
	virtual	void		EndRender()override;
	GET_SET_DEC(ImGuiWindowFlags, m_FormFlag, GetFormFlag, SetFormFlag);
public:
	DEFINE_TYPE_INFO();
	cMyGuiForm();
	virtual ~cMyGuiForm();
	virtual void		RenderProperty()override;
};

class cMyGuiPanel :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
public:
	DEFINE_TYPE_INFO();
	virtual void		RenderProperty()override;
};

class cMyGuiComboBox :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	std::vector<std::string>	m_strDataVector;
public:
	DEFINE_TYPE_INFO();
	virtual void		RenderProperty()override;
};

class cMyGuiListBox :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
public:
	DEFINE_TYPE_INFO();
	virtual void		RenderProperty()override;
};


cMyGuiBasicObj* GetMyGuiObj(eMyImGuiType e_eMyImGuiType, cMyGuiBasicObj* e_pParent);