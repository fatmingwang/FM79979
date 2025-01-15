#pragma once

#include "../../Core/AllCommonInclude.h"
#include "../imgui.h"
#include <vector>
#include <string>

#include <functional>
#include "../../../include/json.hpp"

#define	MYGUI_DEFAULT_IMPLEMENT()	DEFINE_TYPE_INFO();virtual std::wstring GetTypeName()override{return this->Type();}

#define	MY_IMGUI_BASE_DATA m_vSize, m_iID, m_strImGuiName, m_strText, m_vLocalPos, m_vWorldPos

#define	LAZY_INTERNAL_SERIALIZE_FUNCTION(VariableType,Variable)				\
std::shared_ptr<VariableType> Variable;										\
virtual bool InternalSerialize(const nlohmann::json& e_Json)override		\
{																			\
	Variable = std::make_shared<VariableType>();							\
    *Variable = e_Json;														\
	m_pData = std::dynamic_pointer_cast<VariableType>(Variable);			\
	return true;															\
}																			\
virtual nlohmann::json		GetJson()override{return *Variable;}			\
public:																		\
virtual void				CreateImguiDataData()override					\
{																			\
	m_pData = std::make_unique<VariableType>();								\
	Variable = std::dynamic_pointer_cast<VariableType>(m_pData);			\
	this->SetName(this->Type());											\
	SetImGuiName(this->Type());												\
	auto l_IT = g_ImguiTypeNameAndType.find(this->Type());					\
	if (l_IT != g_ImguiTypeNameAndType.end())								\
	{																		\
		this->m_pData->m_strText = GetMyGuiObjLabel(l_IT->second);			\
	}																		\
	else																	\
	{																		\
		this->m_pData->m_strText = ValueToString(this->Type());				\
	}																		\
}																			\
protected:



namespace nlohmann
{
	template <>
	struct adl_serializer<ImVec2>
	{
		static void to_json(json& j, const ImVec2& v)
		{
			j = json{ {"x", v.x}, {"y", v.y} };
		}

		static void from_json(const json& j, ImVec2& v)
		{
			j.at("x").get_to(v.x);
			j.at("y").get_to(v.y);
		}
	};
	template <>
	struct adl_serializer<ImVec4>
	{
		static void to_json(json& j, const ImVec4& v)
		{
			j = json{ {"x", v.x}, {"y", v.y}, {"z", v.z}, {"w", v.w} };
		}

		static void from_json(const json& j, ImVec4& v)
		{
			j.at("x").get_to(v.x);
			j.at("y").get_to(v.y);
			j.at("z").get_to(v.z);
			j.at("w").get_to(v.w);
		}
	};
}


template <typename T>
class cProperty
{
private:
	T m_Value;

public:
	cProperty(T initialValue = T{}) : m_Value(initialValue)
	{
	}

	// Getter
	operator T& ()
	{
		return m_Value;
	}
	operator const T& () const
	{
		return m_Value;
	}

	// Setter
	cProperty& operator=(const T& e_Value)
	{
		m_Value = e_Value;
		return *this;
	}
};


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
	eMIGT_DATA_PICKER,
	eMIGT_MAX
};

class cMyGuiBasicObj* GetMyGuiObj(eMyImGuiType e_eMyImGuiType);
class cImGuiNode* GetImGuiNodeByType(const wchar_t* e_strType);
const char* GetMyGuiObjLabel(eMyImGuiType e_eMyImGuiType);
template<class TYPE>TYPE* GetMyGuiObjWithType(eMyImGuiType e_eMyImGuiType)
{
	return (TYPE*)GetMyGuiObj(e_eMyImGuiType);
}
extern std::map<std::wstring, eMyImGuiType> g_ImguiTypeNameAndType;


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

bool	CheckMouseAndCurrentWindowCollision();

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
	bool						m_bThisUseContainerPositionDontApplyParentPositionToChild = false;
	bool						m_bDoApplyPosition = false;
	//
	void						HierachyPositionRender();
	virtual	void				EndRender() = 0;
	virtual	void				InternalRender() = 0;
	virtual	void				GetRenderRect() = 0;
	bool						m_bPosDirty = false;
	void						SetCachedWorldTransformDirty();
	void						UpdateCachedWorldTransformIfNeeded();
	cImGuiNode*					m_pParent;
	std::vector<cImGuiNode*>	m_ChildNodeVector;
	////not every component suppot this.
	//GET_SET_DEC(Vector2,m_vSize,GetSize,SetSize);
	//void						ApplySize(bool&e_bWidth, bool& e_bHeight);
	// 
	bool						m_bCollided = false;
	virtual bool				InternalSerialize(const nlohmann::json& e_Json);
	bool						UnSerialize(const nlohmann::json& e_Json);
	//for editor mode(drag position) you have to set this
	GET_SET_DEC(bool,m_bOnlyApplyPositionOnceForDragMoving, GetOnlyApplyPositionOnceForDragMoving, SetOnlyApplyPositionOnceForDragMoving);
	GET_SET_DEC(bool, m_bEnable, GetEnable, SetEnable);
	GET_SET_DEC(bool, m_bVisible, IsVisible, SetVisible);

	struct sImguiData
	{
		ImVec2						m_vSize;
		int							m_iID = 0;
		std::string					m_strImGuiName = "Node";
		std::string					m_strText;
		bool						m_bOnlyDoFirstAssignData = true;
		ImVec2						m_vLocalPos = { 0, 0 };
		ImVec2						m_vWorldPos = { 0, 0 };
		ImVec4						m_vColor = {0,0,0,0};
		int							m_ImguiStyleColorType = 0;
		sImguiData();
		virtual ~sImguiData() = default;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(sImguiData, m_vSize, m_iID, m_strImGuiName, m_strText, m_vLocalPos, m_vWorldPos, m_vColor);
	};
	std::shared_ptr<sImguiData>		m_pData;
	
public:
	DEFINE_TYPE_INFO();
	virtual std::wstring GetTypeName() = 0;
	cImGuiNode();
	~cImGuiNode();
	GET_SET(ImVec2, m_pData->m_vSize, GetSize, SetSize)
	GET_SET(std::string, m_pData->m_strImGuiName, GetImGuiName, SetImGuiName);
	GET_SET(std::string, m_pData->m_strText, GetText, SetText);
	void						SetImGuiName(const wchar_t* e_strImGuiNmae);
	ImVec2 						GetLocalPosition() { return this->m_pData->m_vLocalPos;}
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
	void						SetExtraRenderFunction(f_MyImGuiExtraRenderFunction e_MyImGuiExtraRenderFunction) { m_ExtraRenderFunction = e_MyImGuiExtraRenderFunction; }
	std::vector<cImGuiNode*>&	GetChildNodeVector(){return m_ChildNodeVector;}
	bool						ExportJsonFile(const char*e_strFileName);
	std::wstring 				GetNameW();
	std::string					GetRenderText();
	//
	virtual void				CreateImguiDataData() = 0;
	virtual void				DoSerialize(nlohmann::json&e_Json);
	virtual cImGuiNode*			Collided(int e_iPosX, int e_iPosY);
	virtual void				DebugRender();
	static cImGuiNode*			DoUnSerialize(const nlohmann::json& e_Json);
	static std::function<void(cImGuiNode*)>	m_sfParseFunction;
	static	void				DeleteObjectAndAllChildren(cImGuiNode* e_pImGuiNode);
	virtual nlohmann::json		GetJson();


	// Function to find a node by name (recursive)
	cImGuiNode* FindNodeByUID(int e_iID);

};


//class cMyGuiMouseMovingData
//{
//public:
//	bool		m_ChangePos = false;
//	bool		m_bHover = false;
//	bool		m_bSelected = false;
//	bool		m_bLocked = false;
//};

class cMyGuiBasicObj:public cImGuiNode//,public cMyGuiMouseMovingData
{
protected:
	ImVec2							m_vPropertyPos = ImVec2(1400, 10);
	ImVec2							m_vPropertySize = ImVec2(300, 600);
	bool							m_bNameOnTop = true;
	void							RenderNameOnTop();
	virtual void					ApplyPosition()override;
	virtual	void					EndRender()override{}
	virtual	void					InternalRender()override{}
	virtual	void					GetRenderRect()override;
	std::string						GetIDString();
	void							LazyColorSlider(const char*e_strLabel,ImVec4&e_vColor);
	virtual void					InnerRenderProperty();
public:
	cMyGuiBasicObj();
	virtual ~cMyGuiBasicObj();
	virtual std::wstring GetTypeName()override
	{
		return L"cMyGuiBasicObj";
	}
	ImVec2							m_vSizeObj = {0,0 };
	virtual void					RenderProperty();
	virtual void					CreateImguiDataData()override;
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
	virtual	void		EndRender()override;
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
	//virtual void		InnerRenderProperty()override;
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
public:
	MYGUI_DEFAULT_IMPLEMENT();
	cMyGuiButton();
	virtual ~cMyGuiButton();
	//virtual void		InnerRenderProperty()override;
	std::function<void(cMyGuiButton*)>	m_fOnClickFunction;

};

class cMyGuiLabel :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
public:
	cMyGuiLabel();
	virtual ~cMyGuiLabel();
	MYGUI_DEFAULT_IMPLEMENT();
	//virtual void		InnerRenderProperty()override;
};

class cMyGuiEditBox :public cMyGuiBasicObj
{
	bool				m_bTextChanged = false;
	void				FocusCheck();
	virtual	void		InternalRender()override;
	void				RenderMultiLine();
	bool				m_bFocused = false;
	struct sImguiEditBoxData:public sImguiData
	{
		std::string m_strHint = "please input...";
		std::string m_strLabel = "cMyGuiEditBox";
		bool		m_bMultiLines = false;
		int			m_RenderFlag = 0;// ImGuiInputTextFlags_CharsDecimal;
		sImguiEditBoxData()
		{
			this->m_vSize = ImVec2(300, 100);
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(sImguiEditBoxData, MY_IMGUI_BASE_DATA, m_strHint, m_strLabel,m_bMultiLines, m_RenderFlag);
	};
	LAZY_INTERNAL_SERIALIZE_FUNCTION(sImguiEditBoxData, m_pEditBoxData)
public:
	MYGUI_DEFAULT_IMPLEMENT();
	cMyGuiEditBox();
	std::function<void(bool)>			m_fFocusedChangedFunction;
	std::function<void(std::string)>	m_fContentChangedFunction;
	GET_SET(std::string, m_pEditBoxData->m_strHint, GetHint, SetHint);
	GET_SET(bool, m_pEditBoxData->m_bMultiLines, IsMultiLines, SetMultiLines);
	virtual void		InnerRenderProperty()override;
};

class cMyGuiSliderInteger :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	struct sImguiSliderData :public sImguiData
	{
		int m_iMax = 100;
		int m_iMin = 0;
		int m_iValue = 50;
		sImguiSliderData()
		{
			this->m_vSize.x = 200.f;
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(sImguiSliderData, MY_IMGUI_BASE_DATA, m_iMax, m_iMin, m_iValue);
	};
	LAZY_INTERNAL_SERIALIZE_FUNCTION(sImguiSliderData, m_pSliderData);
	virtual void		InnerRenderProperty()override;
public:
	cMyGuiSliderInteger();
	virtual ~cMyGuiSliderInteger();
	MYGUI_DEFAULT_IMPLEMENT();
	GET_SET(int, m_pSliderData->m_iMax, GetMax, SetMax);
	GET_SET(int, m_pSliderData->m_iMin, GetMin, SetMin);

};

class cMyGuiSliderFloatValue :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	struct sImguiSliderData :public sImguiData
	{
		float m_fMax = 100.f;
		float m_fMin = 0;
		float m_fValue = 50.f;
		sImguiSliderData()
		{
			this->m_vSize.x = 200.f;
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(sImguiSliderData, MY_IMGUI_BASE_DATA, m_fMax, m_fMin, m_fValue);
	};
	LAZY_INTERNAL_SERIALIZE_FUNCTION(sImguiSliderData, m_pSliderData);
public:
	cMyGuiSliderFloatValue();
	virtual ~cMyGuiSliderFloatValue();
	MYGUI_DEFAULT_IMPLEMENT();
	GET_SET(float, m_pSliderData->m_fMax, GetMax, SetMax);
	GET_SET(float, m_pSliderData->m_fMin, GetMin, SetMin);
	virtual void		InnerRenderProperty()override;
};

class cMyGuiCheckBox :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;

	struct sImguiCheckBoxData :public sImguiData
	{
		bool m_bChecked;
		sImguiCheckBoxData()
		{
			m_bChecked = false;
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(sImguiCheckBoxData, MY_IMGUI_BASE_DATA, m_bChecked);
	};
	LAZY_INTERNAL_SERIALIZE_FUNCTION(sImguiCheckBoxData, m_pCheckBoxData);
public:
	cMyGuiCheckBox();
	virtual ~cMyGuiCheckBox();
	MYGUI_DEFAULT_IMPLEMENT();
	GET_SET(bool, m_pCheckBoxData->m_bChecked, IsChecked, SetChecked);
	virtual void		InnerRenderProperty()override;
};

class cMyGuiRadio :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	struct sImguiRadioData :public sImguiData
	{
		bool m_bChecked;
		sImguiRadioData()
		{
			m_bChecked = false;
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(sImguiRadioData, MY_IMGUI_BASE_DATA, m_bChecked);
	};
	LAZY_INTERNAL_SERIALIZE_FUNCTION(sImguiRadioData, m_pRadioData);
public:
	cMyGuiRadio();
	virtual ~cMyGuiRadio();
	MYGUI_DEFAULT_IMPLEMENT();
	GET_SET(bool, m_pRadioData->m_bChecked, IsChecked, SetChecked);
	virtual void		InnerRenderProperty()override;
};

class cMyGuiToogle:public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	GET_SET_DEC(bool, m_bChecked, IsChecked, SetChecked);
public:
	cMyGuiToogle();
	virtual ~cMyGuiToogle();
	MYGUI_DEFAULT_IMPLEMENT();
	std::function<void(bool)>	m_fToogleChangedFunction;

};

class cMyGuiForm :public cMyGuiBasicObj
{
	virtual void		ApplyPosition()override;
	virtual	void		InternalRender()override;
	virtual	void		EndRender()override;
	// ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	GET_SET_DEC(ImGuiWindowFlags, m_FormFlag, GetFormFlag, SetFormFlag);
	GET_SET_DEC(bool,m_bShowCloseCutton, IsShowCloseCutton, SetShowCloseCutton);

	struct sImguiFormData :public sImguiData
	{
		int m_FormFlag = 0;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(sImguiFormData, MY_IMGUI_BASE_DATA,m_FormFlag);
	};
	LAZY_INTERNAL_SERIALIZE_FUNCTION(sImguiFormData, m_pFormData);
public:
	MYGUI_DEFAULT_IMPLEMENT();
	cMyGuiForm();
	virtual ~cMyGuiForm();
	virtual void		InnerRenderProperty()override;
	std::function<void(cMyGuiForm*)>		m_fFormCloseFunction;
};

class cMyGuiPanel :public cMyGuiBasicObj
{
	virtual void		ApplyPosition()override;
	virtual	void		InternalRender()override;
	virtual	void		EndRender()override;

	struct sImguiPanelData :public sImguiData
	{
		bool	m_bShowBorder = true;
		int		m_iFormFlag = ImGuiWindowFlags_ChildWindow;
		sImguiPanelData()
		{
			this->m_vSize = ImVec2(200, 200);
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(sImguiPanelData, MY_IMGUI_BASE_DATA, m_iFormFlag, m_bShowBorder);
	};
	LAZY_INTERNAL_SERIALIZE_FUNCTION(sImguiPanelData, m_pPanelData);
	virtual	void		GetRenderRect()override{}
public:
	MYGUI_DEFAULT_IMPLEMENT();
	cMyGuiPanel();
	virtual ~cMyGuiPanel();
	GET_SET(bool, m_pPanelData->m_bShowBorder, GetShowBorder, SetBorder);
	GET_SET(ImGuiWindowFlags, m_pPanelData->m_iFormFlag, GetFormFlag, SetFormFlag);
};

class cMyGuiComboBox :public cMyGuiBasicObj
{
	virtual	void				InternalRender()override;
	//
protected:
	struct sImguiComboxData :public sImguiData
	{
		std::vector<std::string>	m_ItemList;
		int	m_iSelectedIndex;
		sImguiComboxData()
		{
			m_ItemList = { "1","2","3" };
			m_vSize.x = 200.f;
			m_vSize.y = 200.f;
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(sImguiComboxData, MY_IMGUI_BASE_DATA, m_ItemList, m_iSelectedIndex);
	};
	LAZY_INTERNAL_SERIALIZE_FUNCTION(sImguiComboxData, m_pImguiComboxData);
protected:
	std::vector<const char*>	m_ItemTempList;
public:
	MYGUI_DEFAULT_IMPLEMENT();
	cMyGuiComboBox();
	GET_SET(int, m_pImguiComboxData->m_iSelectedIndex, GetSelectedIndex, SetSelectedIndex);
	virtual void				InnerRenderProperty()override;
	std::vector<std::string>	GetItemList(){return m_pImguiComboxData->m_ItemList;}
	virtual void	SetItemList(std::vector<std::string>e_ItemList);
	std::function<void(int,std::string)>	m_fOnSelectFunction;
};



class cMyGuiListBox :public cMyGuiComboBox
{
	void				RenderMultiSelectable();
	virtual	void		InternalRender()override;
	GET_SET_DEC(bool, m_bMiltiSelecteable, IsMiltiSelecteable, SetMiltiSelecteable)
	std::vector<bool>	m_SelectedIndices;
public:
	cMyGuiListBox();
	virtual ~cMyGuiListBox();
	MYGUI_DEFAULT_IMPLEMENT();
	virtual void		InnerRenderProperty()override;
	virtual void		SetItemList(std::vector<std::string>e_ItemList)override;
};

class cMyGuiScroller :public cMyGuiComboBox
{
	virtual	void		InternalRender()override;
	GET_SET_DEC(int, m_iSelectedIndex, GetSelectedIndex, SetSelectedIndex);
	virtual	void		GetRenderRect()override;
public:
	MYGUI_DEFAULT_IMPLEMENT();
	virtual void		InnerRenderProperty()override;
	cMyGuiScroller();
	virtual ~cMyGuiScroller();
};

class cMyGuiDatePicker :public cMyGuiBasicObj
{
	virtual	void		InternalRender()override;
	virtual	void		GetRenderRect()override;
	struct sImguiDatePickerData :public sImguiData
	{
		int m_iYear = 2025;
		int m_iMonth = 1;
		int	m_iDay = 1;
		sImguiDatePickerData()
		{
			m_vSize.x = 100.f;
			m_vSize.y = 30.f;
			this->m_strText = "Date:";
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(sImguiDatePickerData, MY_IMGUI_BASE_DATA, m_iYear, m_iMonth, m_iDay);
	};
	LAZY_INTERNAL_SERIALIZE_FUNCTION(sImguiDatePickerData, m_pImguiDatePickerData);
	std::string			m_strDate;
public:
	MYGUI_DEFAULT_IMPLEMENT();
	virtual void		InnerRenderProperty()override;
	cMyGuiDatePicker();
	virtual ~cMyGuiDatePicker();
	std::function<void(std::string)>m_fDateChangedFunction;
	std::string			GetDateString();
};


template<class TYPE>TYPE* GetMyGuiObjWithType()
{
	TYPE* l_pTYPE = new TYPE();
	l_pTYPE->CreateImguiDataData();
	l_pTYPE->SetName(TYPE::TypeID);
	l_pTYPE->SetImGuiName(TYPE::TypeID);
	return l_pTYPE;
}