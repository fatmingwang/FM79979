#ifndef _EVENT_BUTTON_TRIGGER_H_
#define _EVENT_BUTTON_TRIGGER_H_

#include "EventBase.h"
#include "../AllCoreInclude.h"
namespace FATMING_CORE
{
	//class	cImageButton;
	class	cPuzzleImageUnit;
	//button could add a lots event while button is cliked,to setup a lot data
	//
	class	cEventButton:public cEventBase
	{
		//font scale
		GET_SET_DEC(float,m_fFontScale,GetFontScale,SetFontScale);
		//keep button is pressed
		GET_SET_DEC(UT::sTimeCounter,m_HoldTimeCounter,GetHoldTimeCounter,SetHoldTimeCounter);
		//if mouse pressed or key data singnal is true
		bool				m_bRepeatEnable;
		//all event in it will be delete.
		//it should only work at EventActiver type,but you cild programmly to add event to it.
		cBehaviorObjectList<cEventBase>	m_ButtonClickEvent;
		//text or image
		cClickMouseBehavior*	m_pButton;
		virtual bool		InternalCollide(int e_iPosX,int e_iPosY)override;
        virtual void		InternalMouseDown(int e_iPosX,int e_iPosY)override;
		virtual void		InternalMouseLeave(int e_iPosX,int e_iPosY)override;
        virtual void		InternalMouseUp(int e_iPosX,int e_iPosY)override;
		virtual void		InternalMouseMove(int e_iPosX,int e_iPosY)override;

		virtual	void		InternalInit()override;
		virtual	void		InternalUpdate(float e_fElpasedTime)override;
		virtual	void		InternalRender()override;
		//resource imagebutton for xml export
		GET_SET_DEC(cRenderObject*,m_pReferenceImage,GetReferenceImage,SetReferenceImage);
		//result,for EventButtonSelectedActiver,directly setup variable data.
		std::wstring		m_strResult;
		//while button is pressed
		std::wstring*		m_pstrPressedText;
	public:
		DEFINE_TYPE_INFO();
		cEventButton(Vector2 e_vPos,cGlyphFontRender*e_pGlyphFontRender,const WCHAR* e_strText,Vector4 e_vBGColor,cBaseImage*e_pConnectRadianImage = 0,cBaseImage*e_pLineImage = 0);
		cEventButton(cPuzzleImageUnit*e_pImage,Vector3 e_vPos);
		cEventButton(cEventButton*e_pEventButton);
		cEventButton(TiXmlElement*e_pTiXmlElement);
		EVENT_CLONE_DEFINE(cEventButton);
		virtual ~cEventButton();
		cClickMouseBehavior*		GetButton(){return m_pButton;}
		void						SetButton(cClickMouseBehavior*e_pButton,cBaseImage*e_pReferenceImage);
		bool						IsRepeatEnable(){return m_bRepeatEnable;}
		void						SetRepeatEnable(bool e_bRepeat);
		void						AddEvent(cEventBase*e_pEventBase){ m_ButtonClickEvent.AddObjectNeglectExist(e_pEventBase); }
		//virtual bool				IsSatisfiedCondition();
		void						SetColor(Vector4 e_vColor);
		//if button is text or do nothing
		void						SetBGColor(Vector4 e_vColor);
		virtual NamedTypedObject*   GetResourcePointer()override;
		virtual	TiXmlElement*		ToTiXmlElement()override;
		const WCHAR*				GetResult(){ return m_strResult.c_str(); }
		void						SetResult(const WCHAR*e_strResult){ m_strResult = e_strResult; }
	};

	#define	BUTTON_SELECTION_TYPE	L"ButtonSelectionType"
	enum	eButtonSelectionType
	{
		eBST_ONE,			//only can select one and init unselect
		eBST_ALL_CLICK,		//select until all button is clicked
		eBST_REPEAT,		//click and repeat,reset all button
		eBST_ALL_PRESSED,	//molti button pressed at same time
		eBST_MAX,
	};

	eButtonSelectionType	StringToButtonSelectionType(const WCHAR*e_str);
	std::wstring			ValueToString(eButtonSelectionType e_eButtonSelectionType);
	//if PrefixButton button attribute is exists,it won't add into children's list
	//<cEventMultiButton Name="" ButtonSelectionType="One" StayAtLastSelect="1" PrefixButtonName="11" ClickCount="4"  >
	//<cEventMultiButton Name="" ButtonSelectionType="Repeat" StayAtLastSelect="1" PrefixButtonName="11" >
	//	<cEventButton Name="11" BaseImage="Down" Pos="300,500,0" />
	//	<cEventButton Name="22" PI="Element" PIUnit="Gate" Pos="500,500,0" />
	//	<cEventButton Name="33" Text="79979 test" BGColor="1,0,0.5,1" Pos="400,500,0" />
	//	<cEventObjectMouseUpDectet cEventObjectInstance="GeneratorTest" />
	//</cEventMultiButton>
	class cEventVariable;
	class cEventButtonSelectedActiver;
	class	cEventMultiButton:public cEventBase,public cTabPage
	{
		friend class		cEventButtonSelectedActiver;
		GET_SET_DEC(Vector4,m_vSelectedColor,GetSelectedColor,SetSelectedColor);
		GET_SET_DEC(Vector4,m_vNoneSelectedColor,GetNoneSelectedColor,SetNoneSelectedColor);
	protected:
		virtual	bool		IsAllClickBehaviorDone();
		virtual	bool		IsOneBehaviorDone();
		virtual	bool		IsRepeatBehaviorDone();
		virtual	bool		IsAllPressedBehaviorDone();
		//while variable data is changed,index will be change too.
		cEventVariable*		m_pIndexVariable;
		GET_SET_DEC(bool,m_bStayAtLastSelect,IsStayAtLastSelect,SetStayAtLastSelect);
		GET_SET_DEC(eButtonSelectionType,m_eButtonSelectionType,GetButtonSelectionType,SetButtonSelectionType);
		virtual bool		InternalCollide(int e_iPosX,int e_iPosY)override;
		virtual	void		InternalInit()override;
		virtual	void		InternalUpdate(float e_fElpasedTime)override;
		virtual	void		InternalRender()override;
		//only for children button's prifix hint,don't have update only for prifix or hint.
		cClickMouseBehavior*m_pPrefixButton;
		//=================================
		//for keyboard button funcion... until press exit button or it will continued to store the key data
		//will store children's name until filled contition.
		std::vector<cClickMouseBehavior*>	m_PressedEventButtonList;
		//-1 for non limit,until cancel button is clicked
		//cancel button as a ok button here.
		int							m_iClickCount;
		//=================================
	public:
		DEFINE_TYPE_INFO();
		cEventMultiButton();
		cEventMultiButton(TiXmlElement*e_pTiXmlElement);
		//cEventMultiButton(eButtonSelectionType e_eButtonSelectionType);
		cEventMultiButton(cEventMultiButton*e_pEventMultiButton);
		EVENT_CLONE_DEFINE(cEventMultiButton);
		virtual ~cEventMultiButton();
		virtual	void			Start(float e_fElpaseTime);
		virtual bool			IsSatisfiedCondition()override;
		virtual void			SetSatisfiedCondition(bool e_b)override;
		virtual	void			Init()override;
		virtual	void			Destroy()override;
		virtual	void			Update(float e_fElpastTime)override;
		virtual	void			Render()override;
		virtual	void			SingnalProcess(unsigned char e_usKey)override;
		virtual	void			SingnalProcess()override;
		virtual void			SetRelativePosition( Vector3 e_vPos )override;
		virtual void			DebugRender()override;
		void					MouseDown(int e_iPosX,int e_iPosY)override;
		void					MouseMove(int e_iPosX,int e_iPosY)override;
		void					MouseUp(int e_iPosX,int e_iPosY)override;
		//add button,cancel button if needed call SetCancelButton
		void					AddData(Vector2 e_vPos,cGlyphFontRender*e_pGlyphFontRender,const WCHAR* e_strText,Vector4 e_vBGColor,cBaseImage*e_pConnectRadianImage = 0,cBaseImage*e_pLineImage = 0);
		void					AddData(cPuzzleImageUnit*e_pImage,Vector3 e_vPos);
		virtual	void			CollectResourceList(cNamedTypedObjectVector<NamedTypedObject>*e_pObjectListByName)override;
		virtual	TiXmlElement*	ToTiXmlElement()override;
	};
//end namespace FATMING_CORE
}
#endif