#include "../stdafx.h"
#include "EventButtonTrigger.h"
#include "../AllCoreInclude.h"
#include "EventPackageRunner.h"
#include "EventManager.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cEventButton);
	TYPDE_DEFINE_MARCO(cEventMultiButton);
	cEventButton::cEventButton(TiXmlElement*e_pTiXmlElement)
	{
		m_fFontScale = 1.f;
		m_HoldTimeCounter.SetTargetTime(0.f);
		m_pReferenceImage = 0;
		m_bRepeatEnable = false;
		m_pButton = 0;
		m_pstrPressedText = 0;
		if( !e_pTiXmlElement )
			return;
		bool	l_bKeyData = false;
		unsigned char	l_ucKeyData = -1;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventButton::TypeID);
		Vector3	l_vPos(Vector3::Zero);
		std::wstring	l_strText;
		cBaseImage*		l_pImage = 0;
		cPuzzleImage*l_pPI = 0;
		Vector4	l_vColor(0,0,0,0);
		Vector4	l_vFontColor = Vector4::One;
		bool	l_bRepeat = false;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				this->SetName(l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_REPEAT)
			{
				//this one doesn't work now...because I a lazy to do it...
				l_bRepeat = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME("PressedText")
			{
				m_pstrPressedText = new std::wstring;
				*m_pstrPressedText = l_strValue;
			}
			else
			COMPARE_NAME("Pos")
			{
				l_vPos = VALUE_TO_VECTOR3;
			}
			else
			COMPARE_NAME("Text")
			{
				l_strText = l_strValue;
			}
			else
			COMPARE_NAME("BGColor")
			{
				l_vColor = VALUE_TO_VECTOR4;
			}
			else
			COMPARE_NAME("FontScale")
			{
				m_fFontScale = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME_WITH_DEFINE(cPuzzleImage::TypeID)
			{
				l_pPI = cGameApp::GetPuzzleImage(l_strValue);
				if( !l_pPI )
				{
					std::string	l_strFileName = g_pCurrentEventManager->GetFullpathFileName(UT::CharToWchar(cPuzzleImage::GetFileName((WCHAR*)l_strValue)).c_str());
					l_pPI = cGameApp::GetPuzzleImageByFileName(UT::CharToWchar(l_strFileName.c_str()));
				}
			}
			else
			COMPARE_NAME_WITH_DEFINE(cPuzzleImageUnit::TypeID)
			{
				if( !l_pPI )
				{
					l_strText = l_strValue;
					l_strText += L" load pi file failed";
				}
				else
				{
					l_pImage = l_pPI->GetObject(l_strValue);
				}
			}
			else
			COMPARE_NAME_WITH_DEFINE(cBaseImage::TypeID)
			{
				l_pImage = cGameApp::m_spImageParser->GetBaseImage(l_strValue);
				if( !l_pImage )
				{
					std::string l_strFileName = g_pCurrentEventManager->GetFullpathFileName(l_strValue);
					l_strFileName += ".png";
					l_pImage = new cBaseImage(l_strFileName.c_str());
					if( l_pImage->GetImageIndex() == 0 )
					{
						delete l_pImage;
						l_strText = l_strValue;
						l_strText += L" load Image file failed";
					}
					else
					{
						cGameApp::m_spImageParser->AddObjectNeglectExist(l_pImage);
					}
				}
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_REPEAT)
			{
				m_bRepeatEnable = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_KEY_DATA)
			{
				l_bKeyData = true;
				l_ucKeyData = (unsigned char)l_strValue[0];
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_KEY_VALUE)
			{
				l_bKeyData = true;
				l_ucKeyData = (unsigned char)VALUE_TO_INT;
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_PRESSED_TIME)
			{
				float	l_fTime = VALUE_TO_FLOAT;
				m_HoldTimeCounter.SetTargetTime(l_fTime);
			}
			else
			COMPARE_NAME_WITH_DEFINE(L"FontColor")
			{
				l_vFontColor = VALUE_TO_VECTOR4;
			}
			else
			COMPARE_NAME_WITH_DEFINE(L"Result")
			{
				this->m_strResult = l_strValue;
			}
		PARSE_NAME_VALUE_END
		if( !l_pImage  )
		{
			m_pButton = new cTextButton(Vector2(l_vPos.x,l_vPos.y),g_pCurrentEventManager->m_pEventFont,l_vColor,l_strText.c_str(),0,0);
			cTextButton*l_pImageButton = (cTextButton*)m_pButton;
			if( m_pstrPressedText )
				l_pImageButton->SetPressedText(m_pstrPressedText->c_str());
			l_pImageButton->SetFontColor(l_vFontColor);
			m_pButton->SetName(this->GetName());
			l_pImageButton->SetPressedRepeatMouseUp(m_bRepeatEnable);
			l_pImageButton->GetGlyphFontRender()->SetScale(this->m_fFontScale);
		}
		else
		{
			m_pReferenceImage = l_pImage;
			this->m_pButton = new cImageButton(l_pImage,l_vPos);
			cImageButton*l_pImageButton = (cImageButton*)m_pButton;
			//this one doesn't work now...because I a lazy to do it?...
			//l_pImageButton->SetPressedRepeatMouseUp(l_bRepeat);
			this->m_vCollisionRange = m_pButton->GetCollisionRange();
			l_pImageButton->SetPressedRepeatMouseUp(m_bRepeatEnable);
		}
		unsigned char	l_key = 'L';
		if( l_bKeyData )
			m_pButton->SetKeyData(l_ucKeyData);
	}

	cEventButton::cEventButton(Vector2 e_vPos,cGlyphFontRender*e_pGlyphFontRender,const WCHAR* e_strText,Vector4 e_vBGColor,cBaseImage*e_pConnectRadianImage,cBaseImage*e_pLineImage)
	{
		m_pstrPressedText = 0;
		m_fFontScale = 1.f;
		m_HoldTimeCounter.SetTargetTime(0.f);
		m_pReferenceImage = 0;
		m_pButton = new cTextButton(e_vPos,e_pGlyphFontRender,e_vBGColor,e_strText,e_pConnectRadianImage,e_pLineImage);
	}

	cEventButton::cEventButton(cPuzzleImageUnit*e_pImage,Vector3 e_vPos)
	{
		m_pstrPressedText = 0;
		m_fFontScale = 1.f;
		m_HoldTimeCounter.SetTargetTime(0.f);
		m_pReferenceImage = e_pImage;
		this->m_pButton = new cImageButton(e_pImage,e_vPos);
		this->m_vCollisionRange = m_pButton->GetCollisionRange();
	}

	cEventButton::cEventButton(cEventButton*e_pEventButton):cClickMouseBehavior(e_pEventButton)
	{
		m_pstrPressedText = 0;
		if( m_pstrPressedText )
		{
			m_pstrPressedText = new std::wstring;
			*m_pstrPressedText = e_pEventButton->m_pstrPressedText->c_str();
		}
		m_fFontScale = e_pEventButton->m_fFontScale;
		m_strResult = e_pEventButton->m_strResult;
		m_HoldTimeCounter = e_pEventButton->m_HoldTimeCounter;
		m_pReferenceImage = e_pEventButton->m_pReferenceImage;
		this->SetName(e_pEventButton->GetName());
		NamedTypedObject*l_pObject = e_pEventButton->m_pButton->Clone();		
		this->m_pButton = reinterpret_cast<cClickMouseBehavior*>(l_pObject);
		//InternalCollide is override so we dont need this acctually
		if(!m_pButton)
		{
			const WCHAR*l_str = e_pEventButton->m_pButton->Type();
			int a=0;
		}
		m_bRepeatEnable = e_pEventButton->m_bRepeatEnable;
		this->m_vCollisionRange = e_pEventButton->m_vCollisionRange;
	}

	cEventButton::~cEventButton()
	{
		SAFE_DELETE(m_pButton);
	}

	void	cEventButton::SetButton(cClickMouseBehavior*e_pButton,cBaseImage*e_pReferenceImage)
	{
		SAFE_DELETE(m_pButton);
		m_pReferenceImage = e_pReferenceImage;
		m_pButton = e_pButton;
		if( m_pButton && m_pButton->Type() == cTextButton::TypeID )
		{
			cTextButton*l_pTextButton = (cTextButton*)m_pButton;
			if( l_pTextButton->GetGlyphFontRender() )
				l_pTextButton->GetGlyphFontRender()->SetScale(this->m_fFontScale);
			
		}
		SetRepeatEnable(m_bRepeatEnable);
	}

	void		cEventButton::SetRepeatEnable(bool e_bRepeat)
	{
		m_bRepeatEnable = e_bRepeat;
		if( m_pButton->TypeID == cTextButton::TypeID )
		{
			cTextButton*l_pImageButton = (cTextButton*)m_pButton;
			m_pButton->SetName(this->GetName());
			l_pImageButton->SetPressedRepeatMouseUp(m_bRepeatEnable);
		}
		else
		if( m_pButton->TypeID == cImageButton::TypeID )
		{
			cImageButton*l_pImageButton = (cImageButton*)m_pButton;
			//this one doesn't work now...because I a lazy to do it?...
			//l_pImageButton->SetPressedRepeatMouseUp(l_bRepeat);
			this->m_vCollisionRange = m_pButton->GetCollisionRange();
			l_pImageButton->SetPressedRepeatMouseUp(m_bRepeatEnable);
		}
	}

	//bool		cEventButton::IsSatisfiedCondition()
	//{
	//	return m_pButton->IsSatisfiedCondition();
	//}

	void		cEventButton::SetColor(Vector4 e_vColor)
	{
		if(this->m_pButton->Type() == cTextButton::TypeID )
		{
			cTextButton*l_pTextButton = (cTextButton*)m_pButton;
			l_pTextButton->SetColor(e_vColor);
		}
		else
		{
			cImageButton*l_pImageButton = (cImageButton*)m_pButton;
			l_pImageButton->SetColor(e_vColor);
		}
	}


	void		cEventButton::SetBGColor(Vector4 e_vColor)
	{
		if(this->m_pButton->Type() == cTextButton::TypeID )
		{
			cTextButton*l_pTextButton = (cTextButton*)m_pButton;
			l_pTextButton->SetTextBGColor(e_vColor);
		}
		else
		{
			cImageButton*l_pImageButton = (cImageButton*)m_pButton;
			l_pImageButton->SetColor(e_vColor);		
		}
	}
	NamedTypedObject*   cEventButton::GetResourcePointer()
	{
		if(this->m_pButton->Type() != cTextButton::TypeID )
		{
			NamedTypedObject*l_pNamedTypedObject = m_pButton->GetOwner();
			if( l_pNamedTypedObject )
			{
				if( l_pNamedTypedObject->Type() == cPuzzleImage::TypeID )
				{
					return l_pNamedTypedObject;
				}
			}
			return m_pButton;
		}
		return 0;
	}

	bool		cEventButton::InternalCollide(int e_iPosX,int e_iPosY)
	{
		return m_pButton->Collide(e_iPosX,e_iPosY);
		return true;
	}

	void		cEventButton::InternalMouseLeave(int e_iPosX,int e_iPosY)
	{
		if( m_pButton )
			m_pButton->MouseLeave(e_iPosX,e_iPosY);
	}

    void		cEventButton::InternalMouseDown(int e_iPosX,int e_iPosY)
	{
		if( m_pButton )
			m_pButton->MouseDown(e_iPosX,e_iPosY);
		m_HoldTimeCounter.Start();
	}

	void		cEventButton::InternalMouseMove(int e_iPosX,int e_iPosY)
	{
		if( m_pButton )
			m_pButton->MouseMove(e_iPosX,e_iPosY);
	}

    void		cEventButton::InternalMouseUp(int e_iPosX,int e_iPosY)
	{
		if( m_pButton )
			m_pButton->MouseUp(e_iPosX,e_iPosY);
	}

	void		cEventButton::InternalInit()
	{
		if( m_pButton )
			m_pButton->Init();
		m_ButtonClickEvent.Init();
		m_HoldTimeCounter.Start();
	}

	void		cEventButton::InternalUpdate(float e_fElpasedTime)
	{
		if( !m_pButton )
			return;
		m_pButton->SingnalProcess();
		m_pButton->Update(e_fElpasedTime);
		bool	l_bSatisfied = false;
		if( m_bRepeatEnable )
		{
			bool	l_bTouched = cGameApp::m_sbTouched;
			if( l_bTouched )
			{
				if(!m_pButton->Collide(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y))
				{
					m_pButton->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
				}
			}
		}
		l_bSatisfied = m_pButton->IsSatisfiedCondition();
		if(m_pButton->m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO || eOMB_HORVER == m_pButton->m_eObjectMouseBehavior)
		{
			if( !m_HoldTimeCounter.bTragetTimrReached )
				m_HoldTimeCounter.Update(e_fElpasedTime);
		}
		//
		if( l_bSatisfied )
		{
			if( m_HoldTimeCounter.bTragetTimrReached )
			{
				this->m_bSatisfiedCondition = true;
				this->m_ButtonClickEvent.Update(e_fElpasedTime);
			}
			else
			{
				m_pButton->Init();
			}
		}
	}

	void		cEventButton::InternalRender()
	{
		if( m_pButton )
			m_pButton->Render();
		//if( m_pButton->IsSatisfiedCondition() )
		//{
		//	this->m_ButtonClickEvent.Render();
		//}
	}
	//<cEventButton Name="33" Text="79979 test" BGColor="1,0,0.5,1" Pos="400,500,0" />
	//<cEventButton Name="up.png" cBaseImage="Up" Pos="150,50,0" Repeat="1" />
	//<cEventButton Name="22" cPuzzleImage="Element" cPuzzleImageUnit="Gate" Pos="500,500,0" />
	TiXmlElement*		cEventButton::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		l_pEvent->SetAttribute(NAME,this->GetName());
		Vector3	l_vPos;
		if(cTextButton::TypeID == this->m_pButton->Type())
		{
			cTextButton*l_pTextButton = (cTextButton*)m_pButton;
			l_pEvent->SetAttribute(L"Text",l_pTextButton->GetTxt());
			if(l_pTextButton->GetTextBGColor())
				l_pEvent->SetAttribute(L"BGColor",ValueToStringW((*l_pTextButton->GetTextBGColor())));
			Vector4	l_vCollisionRange = l_pTextButton->GetCollisionRange();
			l_vPos.x = l_vCollisionRange.x;
			l_vPos.y = l_vCollisionRange.y;
			l_vPos.z = l_vCollisionRange.z;
		}
		else
		{
			if( !m_pReferenceImage )
			{
				delete l_pEvent;
				return 0;
			}
			cImageButton*l_pImageButton = (cImageButton*)this->m_pButton;
			bool	l_bRepeat = l_pImageButton->IsPressedRepeatMouseUp();
			l_pEvent->SetAttribute(EVENT_REPEAT,l_bRepeat?1:0);
			l_vPos = l_pImageButton->GetPos();
			if(cBaseImage::TypeID == m_pReferenceImage->Type())
			{
				cBaseImage*l_pBaseImage = (cBaseImage*)m_pReferenceImage;
				l_pEvent->SetAttribute(cBaseImage::TypeID,l_pBaseImage->GetName());
			}
			else
			if(cPuzzleImageUnit::TypeID == m_pReferenceImage->Type())
			{
				cPuzzleImageUnit*l_pPIUnit = (cPuzzleImageUnit*)m_pReferenceImage;
				l_pEvent->SetAttribute(cPuzzleImage::TypeID,l_pPIUnit->GetOwner()->GetName());
				l_pEvent->SetAttribute(cPuzzleImageUnit::TypeID,l_pPIUnit->GetName());
			}
		}
		l_pEvent->SetAttribute(L"Pos",ValueToStringW(l_vPos));
		if( this->m_pButton->GetKeyData() != (unsigned char)-1 )
		{
			l_pEvent->SetAttribute(EVENT_KEY_DATA,ValueToStringW(this->m_pButton->GetKeyData()));
		}
		if( this->m_strResult.length()>0 )
		{
			l_pEvent->SetAttribute(L"Result",m_strResult.c_str());
		}
		return l_pEvent;
	}

	eButtonSelectionType	StringToButtonSelectionType(const WCHAR*e_str)
	{
		if( !wcscmp(e_str,L"One") )
			return eBST_ONE;
		if( !wcscmp(e_str,L"AllClick") )
			return eBST_ALL_CLICK;
		if( !wcscmp(e_str,EVENT_REPEAT) )
			return eBST_REPEAT;
		if( !wcscmp(e_str,L"AllPressed") )
			return eBST_ALL_PRESSED;
		return eBST_MAX;
	}

	std::wstring			ValueToString(eButtonSelectionType e_eButtonSelectionType)
	{
		std::wstring	l_str;
		WCHAR	l_strTemp[TEMP_SIZE];
		switch(e_eButtonSelectionType)
		{
			case eBST_ONE:
				WCHAR_TO_WCHAR(L"One",l_strTemp);
				break;
			case eBST_ALL_CLICK:
				WCHAR_TO_WCHAR(L"AllClick",l_strTemp);
				break;
			case eBST_REPEAT:
				WCHAR_TO_WCHAR(EVENT_REPEAT,l_strTemp);
				break;
			case eBST_ALL_PRESSED:
				WCHAR_TO_WCHAR(L"AllPressed",l_strTemp);
				break;
			default:
				WCHAR_TO_WCHAR(L"unknow",l_strTemp);
				break;
		}
		l_str = l_strTemp;
		return l_str;
	}
	
	cEventMultiButton::cEventMultiButton(TiXmlElement*e_pTiXmlElement)
	{
		m_iClickCount = 0;
		m_pIndexVariable = 0;
		m_pPrefixButton = 0;
		m_bStayAtLastSelect = false;
		m_vSelectedColor = g_vImageButtonClickedColor;
		//m_vNoneSelectedColor = m_vSelectedColor/2.f;
		m_vNoneSelectedColor = Vector4::Zero;
		std::wstring	l_strPrefixButtonName;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventMultiButton::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				SetName(l_strValue);
			}
			else
			COMPARE_NAME("StayAtLastSelect")
			{
				m_bStayAtLastSelect = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME_WITH_DEFINE(BUTTON_SELECTION_TYPE)
			{
				m_eButtonSelectionType = StringToButtonSelectionType(l_strValue);	
				if( m_eButtonSelectionType == eBST_ALL_CLICK )
				{
					UT::ErrorMsg(L"FUCK",L"I am lazy to support cEventMultiButton AllClick now lah...FUCK");
				}
			}
			else
			COMPARE_NAME("PrefixButtonName")
			{
				l_strPrefixButtonName = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventVariable::TypeID)
			{
				m_pIndexVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
				if( !m_pIndexVariable )
				{
					UT::ErrorMsg(l_strValue,L"there is not this variable");
				}
			}
			else
			COMPARE_NAME("ClickCount")
			{
				m_iClickCount = VALUE_TO_INT;
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_SELECTED_COLOR)
			{
				m_vSelectedColor = VALUE_TO_VECTOR4;
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_NONE_SELECTED_COLOR)
			{
				m_vNoneSelectedColor = VALUE_TO_VECTOR4;
			}
		PARSE_NAME_VALUE_END
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while( e_pTiXmlElement )
		{
			cEventBase*l_pEventBase = g_pCurrentEventManager->GetTriggerEvent(e_pTiXmlElement);
			bool	l_bIsPrefix = false;
			if( l_pEventBase )
			{
				if( wcslen(l_strPrefixButtonName.c_str()) )
				{
					if( !wcscmp(l_strPrefixButtonName.c_str(),l_pEventBase->GetName()) )
						l_bIsPrefix = true;
				}
				if( l_bIsPrefix )
				{
					assert( m_pPrefixButton == 0 && "m_pPrefixButton is exists!" );
					m_pPrefixButton = l_pEventBase;
				}
				else
					this->AddObjectNeglectExist(l_pEventBase);
			}
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}
		m_bCheckOutOfWorking = false;
	}
	//if PrefixButton button attribute is exists,it won't add into children's list
	//<cEventMultiButton Name="" ButtonSelectionType="One" StayAtLastSelect="1" PrefixButtonName="11" ClickCount="4"  >
	//<cEventMultiButton Name="" ButtonSelectionType="Repeat" StayAtLastSelect="1" PrefixButtonName="11" >
	//	<cEventButton Name="11" BaseImage="Down" Pos="300,500,0" />
	//	<cEventButton Name="22" PI="Element" PIUnit="Gate" Pos="500,500,0" />
	//	<cEventButton Name="33" Text="79979 test" BGColor="1,0,0.5,1" Pos="400,500,0" />
	//	<cEventObjectMouseUpDectet cEventObjectInstance="GeneratorTest" />
	//</cEventMultiButton>
	TiXmlElement*		cEventMultiButton::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		l_pEvent->SetAttribute(NAME,this->GetName());
		l_pEvent->SetAttribute(BUTTON_SELECTION_TYPE,ValueToString(this->m_eButtonSelectionType));
		l_pEvent->SetAttribute(L"StayAtLastSelect",this->m_bStayAtLastSelect?L"1":L"0");
		if(this->m_iClickCount != 0 )
		{
			l_pEvent->SetAttribute(L"ClickCount",this->m_iClickCount);
		}
		if(m_pPrefixButton)
		{
			l_pEvent->SetAttribute(L"PrefixButtonName",m_pPrefixButton->GetName());
			cEventButton*l_pEventButton = dynamic_cast<cEventButton*>(m_pPrefixButton);
			l_pEvent->LinkEndChild(l_pEventButton->ToTiXmlElement());
		}

		l_pEvent->SetAttribute(EVENT_SELECTED_COLOR,ValueToStringW(this->m_vSelectedColor));
		l_pEvent->SetAttribute(EVENT_NONE_SELECTED_COLOR,ValueToStringW(this->m_vNoneSelectedColor));
		
		
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cEventButton*l_pEventButton = dynamic_cast<cEventButton*>(this->m_ObjectList[i]);
			l_pEvent->LinkEndChild(l_pEventButton->ToTiXmlElement());
		}
		return l_pEvent;
	}
	//cEventMultiButton::cEventMultiButton(eButtonSelectionType e_eButtonSelectionType):cTabPage(Vector4(-79979,-79979,79979,79979))
	//{
	//	m_pIndexVariable =0;
	//	m_pPrefixButton = 0;
	//	m_eButtonSelectionType = e_eButtonSelectionType;
	//	m_bCheckOutOfWorking = false;
	//	m_bStayAtLastSelect = false;
	//}

	cEventMultiButton::cEventMultiButton()
	{
		m_vSelectedColor = g_vImageButtonClickedColor;
		m_vNoneSelectedColor = m_vSelectedColor/2.f;
		this->m_eButtonSelectionType = eBST_MAX;
		m_pIndexVariable = 0;
		m_pPrefixButton = 0;
		m_bStayAtLastSelect = false;
		m_iClickCount = -1;
		m_pPrefixButton = 0;
	}

	cEventMultiButton::cEventMultiButton(cEventMultiButton*e_pEventMultiButton):cTabPage((cTabPage*)e_pEventMultiButton)
	{
		m_vSelectedColor = e_pEventMultiButton->m_vSelectedColor;
		m_vNoneSelectedColor = e_pEventMultiButton->m_vNoneSelectedColor;
		this->m_eButtonSelectionType = e_pEventMultiButton->m_eButtonSelectionType;
		m_pIndexVariable = e_pEventMultiButton->m_pIndexVariable;
		m_pPrefixButton = 0;
		m_bStayAtLastSelect = e_pEventMultiButton->m_bStayAtLastSelect;
		m_iClickCount = e_pEventMultiButton->m_iClickCount;
		if( e_pEventMultiButton->m_pPrefixButton )
		{
			NamedTypedObject*l_pObject = e_pEventMultiButton->m_pPrefixButton->Clone();
			m_pPrefixButton = reinterpret_cast<cClickMouseBehavior*>(l_pObject);
		}
	}

	cEventMultiButton::~cEventMultiButton()
	{
		SAFE_DELETE(m_pPrefixButton);
	}

	bool	cEventMultiButton::InternalCollide(int e_iPosX,int e_iPosY)
	{
		return cTabPage::InternalCollide(e_iPosX,e_iPosY);
	}

	void	cEventMultiButton::InternalInit()
	{
		if( m_pPrefixButton )
			m_pPrefixButton->Init();
		cTabPage::Init();
		m_PressedEventButtonList.clear();
	}

	bool	cEventMultiButton::IsAllClickBehaviorDone()
	{
		int l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			if(!this->m_ObjectList[i]->IsSatisfiedCondition())
			{
				return false;
			}
		}
		return true;
	}

	bool	cEventMultiButton::IsRepeatBehaviorDone()
	{
		if( m_iClickCount == (int)m_PressedEventButtonList.size() )
		{
			return true;
		}
		return false;
	}

	bool		cEventMultiButton::IsAllPressedBehaviorDone()
	{
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cEventButton*l_pEventButton = dynamic_cast<cEventButton*>(this->m_ObjectList[i]);
			if(l_pEventButton->GetHoldTimeCounter().fRestTime > 0.f )
				return false;
		}
		return true;
	}

	bool	cEventMultiButton::IsOneBehaviorDone()
	{
		if(this->m_pCurrentWorkingObject)
			return true;
		return false;
	}

	void	cEventMultiButton::InternalUpdate(float e_fElpasedTime)
	{
		if( this->m_bSatisfiedCondition )
			return;
		if( this->m_pCancelButton && m_pCancelButton->IsSatisfiedCondition() )
		{
			m_bSatisfiedCondition = true;
			return;
		}
		//if click all do not set new index's color
		if( m_pIndexVariable && eBST_ALL_CLICK != m_eButtonSelectionType )
		{
			std::wstring	l_str = m_pIndexVariable->ConvertToString().c_str();
			int	l_iIndex = this->GetObjectIndexByName(l_str.c_str());
			if( l_iIndex != -1 )
			{
				int	l_iCount = this->Count();
				for( int i=0;i<l_iCount;++i )
				{
					cEventButton*l_pEventButton	= dynamic_cast<cEventButton*>(this->GetObject(i));
					if( l_iIndex == i )
						l_pEventButton->SetBGColor(m_vSelectedColor);
					else
						l_pEventButton->SetBGColor(m_vNoneSelectedColor);
				}
			}
		}
		cTabPage::Update(e_fElpasedTime);
		if( eBST_ONE != m_eButtonSelectionType )
		{
			if(this->m_pCurrentWorkingObject)
			{
				m_PressedEventButtonList.push_back(this->GetObject(this->GetCurrentWorkingObjectIndex()));
				if( eBST_ALL_CLICK != m_eButtonSelectionType )
					m_pCurrentWorkingObject->Init();
				else
				{
					//m_pCurrentWorkingObject->SetSatisfiedCondition(false);					
				}
				m_pCurrentWorkingObject = 0;
			}
		}
		switch(m_eButtonSelectionType)
		{
			case eBST_ONE:
				this->m_bSatisfiedCondition = IsOneBehaviorDone();
				break;
			case eBST_ALL_CLICK:
				m_bSatisfiedCondition = IsAllClickBehaviorDone();
				break;
			case eBST_REPEAT:
				this->m_bSatisfiedCondition = IsRepeatBehaviorDone();				
				break;
			case eBST_ALL_PRESSED:
				this->m_bSatisfiedCondition = IsAllPressedBehaviorDone();
				break;
		}
		m_bClickMouseBehaviorSatisfied = m_bSatisfiedCondition;
	}

	void	cEventMultiButton::InternalRender()
	{
		if( m_pPrefixButton )
			m_pPrefixButton->Render();
		cTabPage::Render();
	}

	void	cEventMultiButton::Start(float e_fElpaseTime)
	{
	}

	bool    cEventMultiButton::IsSatisfiedCondition()
	{
		return m_bSatisfiedCondition;
	}

	void    cEventMultiButton::SetSatisfiedCondition(bool e_b)
	{
		this->m_bSatisfiedCondition = e_b;
	}

	void	cEventMultiButton::Init()
	{
		cEventButton*l_pSelectedEventButton = 0;
		//if( eBST_MULTI == m_eButtonSelectionType )
		if( m_bStayAtLastSelect)
		{
			if( this->m_pCurrentWorkingObject )
				l_pSelectedEventButton = dynamic_cast<cEventButton*>(this->m_pCurrentWorkingObject);
		}
		cEventBase::Init();
		//if( l_pSelectedEventButton )
		//	l_pSelectedEventButton->SetColor(m_vSelectedColor);
		cTabPage::Init();
	}

	void	cEventMultiButton::Destroy()
	{
		cEventBase::Destroy();
		cTabPage::Destroy();
	}

	void	cEventMultiButton::Update(float e_fElpastTime)
	{
		cEventBase::Update(e_fElpastTime);
	}

	void	cEventMultiButton::Render()
	{
		cEventBase::Render();
	}

	void	cEventMultiButton::SingnalProcess(unsigned char e_usKey)
	{
		cTabPage::SingnalProcess(e_usKey);
	}

	void	cEventMultiButton::SingnalProcess()
	{
		cTabPage::SingnalProcess();
	}

	void    cEventMultiButton::SetRelativePosition( Vector3 e_vPos )
	{
		cTabPage::SetRelativePosition(e_vPos);
	}

	void    cEventMultiButton::DebugRender()
	{
		cTabPage::DebugRender();
	}

	void	cEventMultiButton::MouseDown(int e_iPosX,int e_iPosY)
	{
		cTabPage::MouseDown(e_iPosX,e_iPosY);
	}

	void	cEventMultiButton::MouseMove(int e_iPosX,int e_iPosY)
	{
		cTabPage::MouseMove(e_iPosX,e_iPosY);
	}

	void	cEventMultiButton::MouseUp(int e_iPosX,int e_iPosY)
	{
		cClickMouseBehavior*l_pcClickMouseBehavior = this->m_pCurrentWorkingObject;
		cTabPage::MouseUp(e_iPosX,e_iPosY);
	}

	void	cEventMultiButton::AddData(Vector2 e_vPos,cGlyphFontRender*e_pGlyphFontRender,const WCHAR* e_strText,Vector4 e_vBGColor,cBaseImage*e_pConnectRadianImage,cBaseImage*e_pLineImage)
	{
		cClickMouseBehavior*l_pButton = new cTextButton(e_vPos,e_pGlyphFontRender,e_vBGColor,e_strText,e_pConnectRadianImage,e_pLineImage);
		this->AddObjectNeglectExist(l_pButton);
	}

	void	cEventMultiButton::AddData(cPuzzleImageUnit*e_pImage,Vector3 e_vPos)
	{
		cImageButton*l_pImageButton = new cImageButton(e_pImage,e_vPos);	
		this->AddObjectNeglectExist(l_pImageButton);
	}

	void	cEventMultiButton::CollectResourceList(cNamedTypedObjectVector<NamedTypedObject>*e_pObjectListByName)
	{
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cEventBase*l_pEventBase = dynamic_cast<cEventBase*>(this->GetObject(i));
			l_pEventBase->CollectResourceList(e_pObjectListByName);
		}
	}
//end namespace FATMING_CORE
}