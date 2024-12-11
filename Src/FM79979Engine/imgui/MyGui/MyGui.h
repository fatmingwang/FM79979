#pragma once

#include "../../Core/AllCommonInclude.h"
#include "../imgui.h"
#include <vector>
#include <string>

#include <functional>

#define	MYGUI_DEFAULT_IMPLEMENT()	DEFINE_TYPE_INFO();virtual std::wstring GetTypeName(){return this->Type();}

enum eMyImGuiType
{
	eMIGT_NODE = 0,
	eMIGT_BUTTON,// = 0,
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
	eMIGT_ROOT_NODE,
	eMIGT_SCROLLER,
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
protected:
	Vector4							m_RenderRect;
	f_MyImGuiExtraRenderFunction	m_ExtraRenderFunction = nullptr;
	//
	virtual void				ApplyPosition() = 0;
	ImVec2						m_vImGuiRenderPos = { 0, 0 };
	void						DestoryWithChildren();
protected:
	bool						m_bThisUseContainerPositionDontApplyarentPositionToChild = false;
	bool						m_bDoApplyPosition = false;
	//
	void						HierachyPositionRender();
	virtual	void				EndRender() = 0;
	virtual	void				InternalRender() = 0;
	virtual	void				GetRenderRect() = 0;
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
	// 
	//for editor mode(drag position) you have to set this
	GET_SET_DEC(bool,m_bOnlyApplyPositionOnceForDragMoving, GetOnlyApplyPositionOnceForDragMoving, SetOnlyApplyPositionOnceForDragMoving);
	GET_SET_DEC(bool, m_bEnable, GetEnable, SetEnable);
	GET_SET_DEC(bool, m_bVisible, IsVisible, SetVisible);
	bool							m_bCollided = false;
public:
	DEFINE_TYPE_INFO();
	virtual std::wstring GetTypeName() = 0;
	cImGuiNode();
	~cImGuiNode();
	int							m_iID = 0;
	std::string					m_strName = "Node";
	eMyImGuiType				m_eType = eMIGT_NODE;
	ImVec2 						GetLocalPosition() { return m_vLocalPos;}
	void						SetLocalPosition(const ImVec2& e_vLocalPos);
	void						SetLocalPosition(float e_fPosX, float e_fPosY);
	ImVec2						GetWorldPosition();
	void						SetWorldPosition(const ImVec2& e_vWorldPos);
	ImVec2						GetWorldImGuiRenderPosition();
	void						SetNotApplyPosition(bool e_b){m_bDoApplyPosition = e_b;}
	
	//-1 to last one
	void						SetParent(cImGuiNode* e_pParent,int e_iChildIndex = -1);
	cImGuiNode*					GetParent(){return this->m_pParent;};
	void						AddChild(cImGuiNode* e_pChild, int e_iChildIndex = -1);
	bool						SwapChild(int e_iIndex1, int e_iIndex2);
	void						Render();// = 0;
	virtual void				DebugRender();
	static	void				DeleteObjectAndAllChildren(cImGuiNode*e_pImGuiNode);
	void						SetExtraRenderFunction(f_MyImGuiExtraRenderFunction e_MyImGuiExtraRenderFunction) { m_ExtraRenderFunction = e_MyImGuiExtraRenderFunction; }
	std::vector<cImGuiNode*>&	GetChildNodeVector(){return m_ChildNodeVector;}
	virtual cImGuiNode*			Collided(int e_iPosX,int e_iPosY);
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
protected:
	virtual void					ApplyPosition()override;
	virtual	void					EndRender(){}
	virtual	void					InternalRender(){}
	virtual void					RenderBaseProperty();
	GET_SET_DEC(ImVec2, m_vSize, GetSize, SetSize);
	GET_SET_DEC(std::string, m_strText, GetText, SetText);
	virtual	void					GetRenderRect()override;
public:
	cMyGuiBasicObj();
	virtual ~cMyGuiBasicObj();
	virtual std::wstring GetTypeName()override
	{
		return L"cMyGuiBasicObj";
	}
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

class cMyGuiRootNode :public cMyGuiBasicObj
{
	static void			CallYesNoDialog(std::function<void(bool)>e_CompleteFunction, const char* e_strContent, const char* e_strYesButtonText = "Yes", const char* e_strNoButtonText = "No", const char* e_strTitle = "Yes or No");
	static void			CallConfirmDialog(std::function<void(bool)>e_CompleteFunction, const char* e_strContent, const char* e_strConfirmButtonText = "Confirm", const char* e_strTitle = "Confirm");
	static void			CallFullScreenBlackText(const char* e_strContent);
	virtual void		ApplyPosition()override;
	virtual	void		InternalRender()override;
	virtual	void		EndRender();
	bool				m_bShowYesNoDialog = false;
	bool				m_bShowConfirmDialog = false;
	bool				m_bShowFullScreenBlackText = false;
	std::function<void(bool)>m_CompleteFunction;
	std::string			m_strDialogMessage = "";
	std::string			m_strYesButtonText = "Yes";
	std::string			m_strNoButtonText = "No";
	virtual	void		GetRenderRect()override;

public:
	MYGUI_DEFAULT_IMPLEMENT();
	//virtual void		RenderProperty()override;
	//for yes no dialog something
	//f_MyImGuiExtraRenderFunction	m_ExtraLastRenderFunction = nullptr;
	void				ShowYesNoDialog(std::function<void(bool)>e_CompleteFunction, const char* e_strContent, const char* e_strYesButtonText = "Yes", const char* e_strNoButtonText = "No");
	void				ShowConfirmDialog(const char* e_strContent, const char* e_strConfirmButtonText = "Confirm" ,std::function<void(bool)>e_CompleteFunction = nullptr);
	void				ShowFullScreenBlackText(const char* e_strContent);
	virtual void		DebugRender()override;
};

class cMyGuiButton :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	GET_SET_DEC(std::string,m_strText,GetText,SetText);
public:
	MYGUI_DEFAULT_IMPLEMENT();
	cMyGuiButton();
	virtual ~cMyGuiButton();
	//virtual void		RenderProperty()override;
	std::function<void(cMyGuiButton*)>	m_fOnClickFunction;
};

class cMyGuiLabel :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
public:
	cMyGuiLabel();
	virtual ~cMyGuiLabel();
	MYGUI_DEFAULT_IMPLEMENT();
	//virtual void		RenderProperty()override;
};

class cMyGuiEditBox :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	void				RenderMultiLine();
	GET_SET_DEC(std::string, m_strHint, GetHint, SetHint);
	GET_SET_DEC(bool,m_bMultiLines, IsMultiLines, SetMultiLines);
public:
	MYGUI_DEFAULT_IMPLEMENT();
	cMyGuiEditBox();
	//virtual void		RenderProperty()override;
};

class cMyGuiSliderInteger :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	GET_SET_DEC(int, m_iMax, GetMax, SetMax);
	GET_SET_DEC(int, m_iMin, GetMin, SetMin);
	GET_SET_DEC(std::string, m_strName, GetName, SetName);
	int  m_iValue;
public:
	cMyGuiSliderInteger();
	virtual ~cMyGuiSliderInteger();
	MYGUI_DEFAULT_IMPLEMENT();
	//virtual void		RenderProperty()override;
};

class cMyGuiSliderFloatValue :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	GET_SET_DEC(float, m_fMax, GetMax, SetMax);
	GET_SET_DEC(float, m_fMin, GetMin, SetMin);
	GET_SET_DEC(std::string, m_strName, GetName, SetName);
	float  m_fValue;
public:
	cMyGuiSliderFloatValue();
	virtual ~cMyGuiSliderFloatValue();
	MYGUI_DEFAULT_IMPLEMENT();
	//virtual void		RenderProperty()override;
};

class cMyGuiCheckBox :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	GET_SET_DEC(bool, m_bChecked, IsChecked, SetChecked);
public:
	cMyGuiCheckBox();
	virtual ~cMyGuiCheckBox();
	MYGUI_DEFAULT_IMPLEMENT();
	//virtual void		RenderProperty()override;
};

class cMyGuiRadio :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	GET_SET_DEC(bool, m_bChecked, IsChecked, SetChecked);
public:
	cMyGuiRadio();
	virtual ~cMyGuiRadio();
	MYGUI_DEFAULT_IMPLEMENT();
	//virtual void		RenderProperty()override;
};

class cMyGuiToogle:public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	GET_SET_DEC(bool, m_bChecked, IsChecked, SetChecked);
public:
	cMyGuiToogle();
	virtual ~cMyGuiToogle();
	MYGUI_DEFAULT_IMPLEMENT();
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
	MYGUI_DEFAULT_IMPLEMENT();
	cMyGuiForm();
	virtual ~cMyGuiForm();
	virtual void		RenderProperty()override;
};

class cMyGuiPanel :public cMyGuiBasicObj
{
	virtual void		ApplyPosition()override;
	virtual	void		InternalRender()override;
	virtual	void		EndRender()override;
	GET_SET_DEC(bool,m_bShowBorder, GetShowBorder, SetBorder);
	GET_SET_DEC(ImGuiWindowFlags, m_FormFlag, GetFormFlag, SetFormFlag);
	virtual	void		GetRenderRect()override{ }
public:
	MYGUI_DEFAULT_IMPLEMENT();
	cMyGuiPanel();
	virtual ~cMyGuiPanel();
	virtual void		RenderProperty()override;
};

class cMyGuiComboBox :public cMyGuiBasicObj
{
	virtual	void				InternalRender()override;
	//
	GET_SET_DEC(std::vector<std::string>, m_strDataVector, GetDataVector, SetDataVector);
	GET_SET_DEC(int,m_iSelectedIndex,GetSelectedIndex,SetSelectedIndex);
public:
	MYGUI_DEFAULT_IMPLEMENT();
	cMyGuiComboBox();
	virtual void				RenderProperty()override;
	std::function<void(int)>	m_fOnSelectFunction;
};



class cMyGuiListBox :public cMyGuiComboBox
{
	void				RenderMultiSelectable();
	virtual	void		InternalRender()override;
public:
	MYGUI_DEFAULT_IMPLEMENT();
	virtual void		RenderProperty()override;
};

class cMyGuiScroller :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	GET_SET_DEC(int, m_iSelectedIndex, GetSelectedIndex, SetSelectedIndex);
	virtual	void		GetRenderRect()override;
public:
	MYGUI_DEFAULT_IMPLEMENT();
	virtual void		RenderProperty()override;
	cMyGuiScroller();
	virtual ~cMyGuiScroller();
};

class cMyTreeView :public NamedTypedObject
{
	cImGuiNode* m_pCopyNode = nullptr;
	cImGuiNode* m_pDragNode = nullptr;
	cImGuiNode* m_pDropParent = nullptr;
	cImGuiNode* m_pSelectedNode = nullptr;

	ImVec2		m_pSelectedNodeRect[2];
	bool		m_bDoRename = false;
	int			m_iDropIndex = -1;
	void RenderTreeivewPopupMenuContext();
	void DisplayTree(cImGuiNode* e_pNode, bool e_bRenderVisibleCheckBox);
public:
	int m_iRenderFlag = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;
	cImGuiNode* m_pRoot = nullptr;
	cMyTreeView();
	virtual ~cMyTreeView();
	void Render();
};


cMyGuiBasicObj* GetMyGuiObj(eMyImGuiType e_eMyImGuiType);
const char*		GetMyGuiObjLabel(eMyImGuiType e_eMyImGuiType);
template<class TYPE>TYPE* GetMyGuiObjWithType(eMyImGuiType e_eMyImGuiType)
{
	return (TYPE*)GetMyGuiObj(e_eMyImGuiType);
}

template<class TYPE>TYPE* GetMyGuiObjWithType()
{
	TYPE* l_pTYPE = new TYPE();
	l_pTYPE->SetName(TYPE::TypeID);
	return l_pTYPE;
}