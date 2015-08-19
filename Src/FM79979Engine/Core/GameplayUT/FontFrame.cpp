#include "../stdafx.h"
#include "FontFrame.h"
#include "ImageButton.h"
#include "GameApp.h"
#include "ObjectXMLParser.h"
namespace FATMING_CORE
{
	const wchar_t*         cTextButton::TypeID( L"cTextButton" );
	const wchar_t*         cFontWithFrame::TypeID( L"cFontWithFrame");
	cFontWithFrame::cFontWithFrame(cGlyphFontRender*e_pGlyphFontRender,const wchar_t*e_strText,cBaseImage*e_pConnectRadianImage,cBaseImage*e_pLineImage)
	{
		m_pstrCurrentText = 0;
		m_pvTextBGColor = 0;
		m_pstrPressedText = 0;
		m_pGlyphFontRender = e_pGlyphFontRender;
		m_pConnectRadianImage = e_pConnectRadianImage;
		m_pLineImage = e_pLineImage;
		SetText(e_strText);
	}

	cFontWithFrame::cFontWithFrame(cFontWithFrame*e_pFontWithFrame):cRenderObject(e_pFontWithFrame)
	{
		m_pstrCurrentText = 0;
		m_pvTextBGColor = 0;
		m_pstrPressedText = 0;	
		m_pGlyphFontRender = e_pFontWithFrame->m_pGlyphFontRender;
		if( e_pFontWithFrame->m_pConnectRadianImage )
			m_pConnectRadianImage = dynamic_cast<cBaseImage*>(e_pFontWithFrame->m_pConnectRadianImage->Clone());
		if( e_pFontWithFrame->m_pConnectRadianImage )
			m_pLineImage = dynamic_cast<cBaseImage*>(e_pFontWithFrame->m_pConnectRadianImage->Clone());
		if( e_pFontWithFrame->m_pvTextBGColor )
			SetTextBGColor(*e_pFontWithFrame->m_pvTextBGColor);
		SetText(e_pFontWithFrame->m_strText.c_str());
	}

	cFontWithFrame::~cFontWithFrame()
	{
		SAFE_DELETE(m_pstrPressedText);
		SAFE_DELETE(m_pvTextBGColor);
	}

	void	cFontWithFrame::SetPressedText(const wchar_t*e_strText)
	{
		SAFE_DELETE(m_pstrPressedText);
		m_pstrPressedText = new std::wstring;
		*m_pstrPressedText = e_strText;
	}

	void	cFontWithFrame::SetText(const wchar_t*e_strText)
	{
		m_strText = e_strText;
		m_pstrCurrentText = m_strText.c_str();
		m_vRenderSize = Vector2::Zero;
		if( m_pGlyphFontRender )
			m_vRenderSize = this->m_pGlyphFontRender->GetRenderSize(e_strText);
	}

	void	cFontWithFrame::SetTextBGColor(Vector4 e_vColor)
	{
		if( !m_pvTextBGColor )
			m_pvTextBGColor = new Vector4(e_vColor);
		else
			*m_pvTextBGColor = e_vColor;
	}
	//<cFontWithFrame GlyphRenderName="" TextBGColor="" Text="" Pos=""/>
	//<SubMPDI cMPDIList="" cMPDI="" cSubMPDI=""/>
	//<ConnectRadianImage>
	//	base image
	//</ConnectRadianImage>
	//<LineImage>
	//	base image
	//</LineImage>
	//</cFontWithFrame/>
	cFontWithFrame*	cFontWithFrame::GetMe(TiXmlElement*e_pTiXmlElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cFontWithFrame::TypeID);
		cGlyphFontRender*	l_pGlyphFontRender = cGameApp::m_spGlyphFontRender;
		cBaseImage*			l_pConnectRadianImage = 0;
		cBaseImage*			l_pLineImage = 0;
		Vector4				l_vTextBGColor;
		Vector3				l_vPos = Vector3::Zero;
		bool				l_bTextBGColor = false;
		std::wstring		l_strText;
		const wchar_t*		l_strSetName = 0;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cFontWithFrame::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("GlyphRenderName")
			{
				l_pGlyphFontRender = cGameApp::GetGlyphFontRenderByFileName(l_strValue);
			}
			else
			COMPARE_NAME("TextBGColor")
			{
				l_vTextBGColor = VALUE_TO_VECTOR4;
				l_bTextBGColor = true;
			}
			else
			COMPARE_NAME("Text")
			{
				l_strText = l_strValue;
			}
			else
			COMPARE_NAME("Pos")
			{
				l_vPos = VALUE_TO_VECTOR3;
			}
			else
			COMPARE_NAME("Name")
			{
				l_strSetName = l_strValue;
			}
		PARSE_NAME_VALUE_END
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while( e_pTiXmlElement )
		{
			l_strValue = e_pTiXmlElement->Value();
			COMPARE_VALUE("ConnectRadianImage")
			{
				cRenderObject*l_pRenderObjectBehavior = GetRenderObjectBehavior(e_pTiXmlElement->FirstChildElement());
				if( l_pRenderObjectBehavior )
					l_pConnectRadianImage = dynamic_cast<cBaseImage*>(l_pRenderObjectBehavior);
			}
			else
			COMPARE_VALUE("LineImage")
			{
				cRenderObject*l_pRenderObjectBehavior = GetRenderObjectBehavior(e_pTiXmlElement->FirstChildElement());
				if( l_pRenderObjectBehavior )
					l_pLineImage = dynamic_cast<cBaseImage*>(l_pRenderObjectBehavior);
			}
			else//for position
			COMPARE_VALUE_WITH_DEFINE(cSubMPDI::TypeID)
			{
				cSubMPDI*l_pSubMPDI = cSubMPDI::GetMe(e_pTiXmlElement,false);
				if( l_pSubMPDI )
				{
					if( l_pSubMPDI->GetOriginalPointList().size() > 0)
						l_vPos = l_pSubMPDI->GetOriginalPointList()[0];
				}
			}
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}
		cFontWithFrame*l_pFontWithFrame = 0;
		if( l_pGlyphFontRender )
		{
			l_pFontWithFrame = new cFontWithFrame(l_pGlyphFontRender,l_strText.c_str(),l_pConnectRadianImage,l_pLineImage);
			if(l_strSetName)
				l_pFontWithFrame->SetName(l_strSetName);
			if( l_bTextBGColor )
				l_pFontWithFrame->SetTextBGColor(l_vTextBGColor);
			l_pFontWithFrame->SetPos(l_vPos);
		}
		return l_pFontWithFrame;
	}

	void	cFontWithFrame::Render()
	{
		Vector3	l_vPos = this->GetWorldPosition();
		Render((int)l_vPos.x,(int)l_vPos.y);
	}

	void	cFontWithFrame::Render(int e_iX,int e_iY)
	{
		if( !m_pvTextBGColor )
		{
			//Vector2	l_vLeltUp((float)e_iX,(float)e_iY);
			//Vector2	l_vRightUp(e_iX+m_vRenderSize.x,(float)e_iY);
			//Vector2	l_vLeltDown((float)e_iX,e_iY+m_vRenderSize.y);
			//Vector2	l_vRightDown(e_iX+m_vRenderSize.x,e_iY+m_vRenderSize.y);
		}
		else
		{
			float	l_fScale = 1.f;
			RenderFilledRectangle(Vector2((float)e_iX,(float)e_iY),m_vRenderSize.x,m_vRenderSize.y,*m_pvTextBGColor,0.f);
		}
		if( m_pGlyphFontRender  && m_pstrCurrentText )
		{
			m_pGlyphFontRender->RenderFont(e_iX,e_iY,m_pstrCurrentText);
		}
	}

	cTextButton::cTextButton(Vector2 e_vRenderPos,cGlyphFontRender*e_pGlyphFontRender,Vector4 e_vPressedColor,const wchar_t* e_strText,cBaseImage*e_pConnectRadianImage,cBaseImage*e_pLineImage)
		:cFontWithFrame(e_pGlyphFontRender,e_strText,e_pConnectRadianImage,e_pLineImage)
	{
		m_fScale = 1.f;
		m_vFontColor = Vector4::One;
		m_bPressedRepeatMouseUp = false;
		this->SetLocalPosition(Vector3(e_vRenderPos.x,e_vRenderPos.y,0.f));
		m_vCollisionRange = Vector4(0,0,m_vRenderSize.x,m_vRenderSize.y);
		m_TC.SetTargetTime(MOUSE_UP_TIME_OFFSET);
		m_TCForRepeat.SetTargetTime(cImageButton::m_sfRepeatTime);
		this->m_pvTextBGColor = new Vector4(e_vPressedColor);
		m_vPressedColor = g_vImageButtonClickedColor;
	}

	cTextButton::cTextButton(cTextButton*e_pTextButton):cFontWithFrame(e_pTextButton->m_pGlyphFontRender,e_pTextButton->m_strText.c_str(),e_pTextButton->m_pConnectRadianImage,e_pTextButton->m_pLineImage),
		cClickMouseBehavior(e_pTextButton)
	{
		m_fScale = 1.f;
		SetLocalPosition(e_pTextButton->GetLocalPosition());
		if( e_pTextButton->m_pstrPressedText )
			this->SetPressedText(e_pTextButton->m_pstrPressedText->c_str());
		m_bPressedRepeatMouseUp = e_pTextButton->m_bPressedRepeatMouseUp;
		m_TC.SetTargetTime(MOUSE_UP_TIME_OFFSET);
		m_TCForRepeat.SetTargetTime(cImageButton::m_sfRepeatTime);
		this->m_pvTextBGColor = new Vector4(*e_pTextButton->m_pvTextBGColor);
		m_vPressedColor = e_pTextButton->m_vPressedColor;
		m_vFontColor = e_pTextButton->m_vFontColor;
	}


	cTextButton::~cTextButton()
	{
	
	}

	void	cTextButton::Destroy()
	{
	}

	void	cTextButton::SetScale(float e_fScale)
	{
		m_fScale = e_fScale;
		m_vCollisionRange = Vector4(0,0,m_vRenderSize.x*e_fScale,m_vRenderSize.y*e_fScale);
	}

	void	cTextButton::SetColor(Vector4 e_vColor)
	{
		if(m_pConnectRadianImage)
			m_pConnectRadianImage->SetColor(e_vColor);
		if(m_pLineImage)
			m_pLineImage->SetColor(e_vColor);
		m_vPressedColor = e_vColor;
	}

	void    cTextButton::InternalMouseDown(int e_iPosX,int e_iPosY)
	{
		SetColor(*this->m_pvTextBGColor);
		if( this->m_pstrPressedText )
		{
			m_pstrCurrentText = this->m_pstrPressedText->c_str();
		}
	}

	void    cTextButton::InternalMouseLeave(int e_iPosX,int e_iPosY)
	{
		SetColor(Vector4::One);
		m_pstrCurrentText = this->m_strText.c_str();
	}

	void    cTextButton::InternalMouseUp(int e_iPosX,int e_iPosY)
	{
		SetColor(this->m_vFontColor);
		m_pstrCurrentText = this->m_strText.c_str();
		if( m_TC.fRestTime >0.00001f )
			m_TC.Start();
	}

	bool    cTextButton::IsSatisfiedCondition()
	{
		if( this->m_bPressedRepeatMouseUp && this->m_eObjectMouseBehavior == eOMB_HORVER )
		{
			return this->m_TCForRepeat.bTragetTimrReached;
		}
		if( cClickMouseBehavior::IsSatisfiedCondition() )
		{
			return this->m_TC.bTragetTimrReached;
		}
		return false;
	}

	void    cTextButton::Init()
	{
		eObjectMouseBehavior	l_eObjectMouseBehavior = m_eObjectMouseBehavior;
		m_TC.Start();
		cClickMouseBehavior::Init();
		InternalMouseUp(0,0);
		if( l_eObjectMouseBehavior == eOMB_HORVER )
			m_eObjectMouseBehavior = eOMB_HORVER;
	}

	void    cTextButton::InitRepeat()
	{

	}

	void    cTextButton::Update(float e_fElpaseTime)
	{
		if( this->m_bPressedRepeatMouseUp && (this->m_eObjectMouseBehavior == eOMB_HORVER || m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ))
		{
			if( m_TCForRepeat.bTragetTimrReached )
			{
				m_TCForRepeat.Start();
				m_eObjectMouseBehavior = eOMB_HORVER;
			}
			else
				this->m_TCForRepeat.Update(e_fElpaseTime);
		}
		if( cClickMouseBehavior::IsSatisfiedCondition() )
		{
			if( !m_TC.bTragetTimrReached )
			{
				m_TC.Update(e_fElpaseTime);
				if( m_TC.bTragetTimrReached )
				{
					InternalMouseUp(0,0);
					CallExternalFunction(this);
				}
			}
		}
	}

	void	cTextButton::Render()
	{
		if( m_pGlyphFontRender )
		{
			if( this->m_bCollided )
				this->m_pGlyphFontRender->SetFontColor(m_vPressedColor);
		}
		Vector3	l_vPos = this->GetWorldPosition();
		if( m_pGlyphFontRender )
		{
			m_pGlyphFontRender->SetScale(this->m_fScale);
		}
		Vector2	l_vRenderSize = this->m_vRenderSize;
		m_vRenderSize.x *= m_fScale;
		m_vRenderSize.y *= m_fScale;
		cFontWithFrame::Render((int)l_vPos.x,(int)l_vPos.y);
		m_vRenderSize = l_vRenderSize;
		if( m_pGlyphFontRender )
			m_pGlyphFontRender->SetFontColor(Vector4::One);
	}
	//<cTextButton RenderPos="" PressedColor="" Text="" >
	//	<ConnectRadianImage>
	//		<cPuzzleImageUnit.....>
	//	</ConnectRadianImage>
	//	<LineImage>
	//		<cPuzzleImageUnit.....>
	//	</LineImage>
	//<cTextButton RenderPos="" PressedColor="" Text="" >
	cTextButton*	cTextButton::GetMe(TiXmlElement*e_pElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cTextButton::TypeID);
		Vector2 l_vRenderPos;
		cGlyphFontRender*l_pGlyphFontRender = cGameApp::m_spGlyphFontRender;
		Vector4 l_vPressedColor = Vector4::HalfDark;
		std::wstring	l_strText;
		cBaseImage*l_pConnectRadianImage = 0;
		cBaseImage*l_pLineImage = 0;
		const wchar_t*l_strSetName = 0;
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("RenderPos")
			{
				l_vRenderPos = VALUE_TO_VECTOR3;
			}
			else
			COMPARE_NAME("PressedColor")
			{
				l_vPressedColor = VALUE_TO_VECTOR4;
			}
			else
			COMPARE_NAME("Text")
			{
				l_strText = l_strValue;
			}
			else
			COMPARE_NAME("Name")
			{
				l_strSetName = l_strValue;
			}
		PARSE_NAME_VALUE_END
		e_pElement = e_pElement->FirstChildElement();
		while( e_pElement )
		{
			const wchar_t*l_strValue = e_pElement->Value();
			COMPARE_VALUE("ConnectRadianImage")
			{
				l_pConnectRadianImage = dynamic_cast<cBaseImage*>(GetRenderObjectBehavior(e_pElement));
			}
			else
			COMPARE_VALUE("LineImage")
			{
				l_pLineImage = dynamic_cast<cBaseImage*>(GetRenderObjectBehavior(e_pElement));
			}
			COMPARE_NOT_SUPPORT(L"cTextButton::GetMe",l_strValue);
			e_pElement = e_pElement->NextSiblingElement();
		}
		cTextButton*l_pTextButton = new cTextButton(l_vRenderPos,l_pGlyphFontRender,l_vPressedColor,l_strText.c_str(),l_pConnectRadianImage,l_pLineImage);
			if(l_strSetName)
				l_pTextButton->SetName(l_strSetName);
		return l_pTextButton;
	}
}