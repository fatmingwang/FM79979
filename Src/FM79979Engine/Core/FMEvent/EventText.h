#ifndef _EVENT_TEXT_H_
#define _EVENT_TEXT_H_

#include "EventObject.h"

namespace   FATMING_CORE
{
    class   cEventObject;
    class   cEventText:public cEventObject
    {
        std::wstring    m_strText;
		virtual void    InternalRender()override;
        virtual void	InternalUpdate(float e_fElpaseTime)override;
    public:
        DEFINE_TYPE_INFO();
		cEventText(Vector3 e_vPos,const WCHAR*e_strText,float e_fFontSize = 1.f,Vector4 e_vColor = Vector4::One);
        cEventText(cEventText*e_pEvent_Text);
		cEventText(TiXmlElement*e_pElement);
        virtual ~cEventText(){}
        virtual cEventObject*	Clone()override { return new cEventText(this); }
        void					SetText(WCHAR*e_str){m_strText = e_str; }
        const WCHAR*			GetText(){return m_strText.c_str();}
        //virtual void    SetRelativePosition( Vector3 e_vPos ){}
		float					GetFontSize(){ return m_vSize.x; }
		void					SetFontSize(float e_fFontSize){ m_vSize.x = e_fFontSize; }
		virtual	TiXmlElement*	ToTiXmlElement()override;
		virtual NamedTypedObject*   GetResourcePointer()override {return 0;}
    };

	//render text by xml.
	//for each root's child element to render attribute
	//<Root ViewRect="0,0,1280,1024" AttributeDistance="50,0" ElementDistance="0,100" >
	//	<Income			Describtion="ooxx:" Time="Data:2002/06/01" Amount="Amount:100" >
	//	<Income			Describtion="ooxx:" Time="Data:2002/06/01" Amount="Amount:100" >
	//</Root>
	//ensure u have made a variable indicate view page index and a EventInstance with a trigger has 2 button to set variable's value increase and decrease.
	class	cEventVariable;
	class   cEventXmlText:public cEventObject
	{
		bool			m_bXmlFromPointer;
		//to get root element children count to detect m_pRenderIndexEventVariable's value is vaild or not
		int				m_iRootChildrenCount;
		//the root's child element index
		int				m_iCurrentRootChildElementIndex;
		//how many element for a page
		int				m_iListPerPage;
		//
		cEventVariable*	m_pRenderIndexEventVariable;
		Vector2			m_vElementDistance;
		Vector2			m_vAttributeDistance;
		Vector4			m_vRenderRect;
		Vector2			m_vRootElementAttributePos;
        cNodeISAX*		m_pNodeISAX;
		std::string		m_strFileName;
		virtual void    InternalInit()override;
		virtual void    InternalUpdate(float e_fElpaseTime)override;
		virtual void    InternalRender()override;
		void			ToLastPage();
    public:
        DEFINE_TYPE_INFO();
		cEventXmlText(TiXmlElement*e_pTiXmlElement);
        cEventXmlText(const WCHAR*e_strFileName,Vector4 e_RenderRect,Vector2 e_AttributeDistance,Vector2 e_ElementDistance,const WCHAR*e_strVariableName,Vector2 e_vRootElementAttributePos,int e_iListPerPage);
        cEventXmlText(cEventXmlText*e_pEventXmlText);
        virtual ~cEventXmlText();
        EVENT_CLONE_DEFINE(cEventXmlText);
		virtual	TiXmlElement*	ToTiXmlElement()override;
		virtual NamedTypedObject*   GetResourcePointer()override {return 0;}
	};
}
//end namespace
#endif