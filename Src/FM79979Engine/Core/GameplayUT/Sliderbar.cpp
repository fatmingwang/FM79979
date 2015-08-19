#include "../stdafx.h"
#include "Sliderbar.h"
#include "GameApp.h"
#include "../Image/SimplePrimitive.h"
#include "../Image/SubMPDI.h"
#include "ObjectXMLParser.h"
namespace FATMING_CORE
{
	const wchar_t*         cSliderBar::TypeID( L"cSliderBar" );

	cSliderBar::cSliderBar(Vector3	e_vPos,cBaseImage*e_pBar,cBaseImage*e_pSlider,eOrientation e_eOrientation)
	{
		this->m_eOrientation = e_eOrientation;
		m_pBar = 0;
		m_pSlider = 0;
		if( e_pBar )
		{
			m_pBar = dynamic_cast<cBaseImage*>(e_pBar->Clone());
			this->m_vCollisionRange = Vector4(0,0,(float)m_pBar->GetWidth(),(float)m_pBar->GetHeight());
		}
		else
		{
			m_vCollisionRange = Vector4(0,0,100,20);
		}
		if( e_pSlider )
			m_pSlider = dynamic_cast<cBaseImage*>(e_pSlider->Clone());
		m_fMaxValue = 100.f;
		m_fMinValue = 0.f;
		//m_fBarLength = 100.f;
		m_fCurrentValue = 0.f;
		if( m_eOrientation == eO_HORIZONTAL )
			this->m_fCurrentBarValue = this->m_vCollisionRange.Width();
		else
			this->m_fCurrentBarValue = this->m_vCollisionRange.Height();
		this->SetPos(e_vPos);
	}

	cSliderBar::cSliderBar(cCueToStartCurveWithTime*e_pBar,cCueToStartCurveWithTime*e_pSlider,eOrientation e_eOrientation)
	{
		this->m_eOrientation = e_eOrientation;
		m_pBar = 0;
		m_pSlider = 0;
		if( e_pBar )
		{
			m_pBar = e_pBar->PointDataToBaseImage(0);
			this->m_vCollisionRange = Vector4(0,0,(float)m_pBar->GetWidth(),(float)m_pBar->GetHeight());
		}
		else
		{
			m_vCollisionRange = Vector4(0,0,100,20);
		}
		if( e_pSlider )
		{
			m_pSlider = e_pSlider->PointDataToBaseImage(0);
			POINT	l_Offset = {0,0};
			m_pSlider->SetOffsetPos(l_Offset);
		}
		m_fMaxValue = 100.f;
		m_fMinValue = 0.f;
		//m_fBarLength = 100.f;
		m_fCurrentValue = 0.f;
		if( m_eOrientation == eO_HORIZONTAL )
			this->m_fCurrentBarValue = this->m_vCollisionRange.Width();
		else
			this->m_fCurrentBarValue = this->m_vCollisionRange.Height();
	}

	cSliderBar::~cSliderBar()
	{
		SAFE_DELETE(m_pBar);
		SAFE_DELETE(m_pSlider);
	}
	//<cSliderBar>
	//	<SliderImage>
	//		<cPuzzleImageUnit PI="Image/Girl/ChoiceGirl.pi" PIUnit="1">
	//	</SliderImage>
	//	<BarImage>
	//		<cPuzzleImageUnit PI="Image/Girl/ChoiceGirl.pi" PIUnit="1">
	//	</BarImage>
	//	<ShowPos Pos="0,0,0">
	//</cSliderBar>
	//<cSliderBar>
	//	<SliderSubMPDI>
	//		<cSubMPDI cMPDIList="" cMPDI="" cSubMPDI=""/>
	//	</SliderSubMPDI>
	//	<BarSubMPDI>
	//		<cSubMPDI cMPDIList="" cMPDI="" cSubMPDI=""/>
	//	</BarSubMPDI>
	//</cSliderBar>
	//<cSliderBar>
	//	<ShowPos Pos="0,0,0">
	//	<CollideRange Rect="0,0,200,50"/>
	//</cSliderBar>
	cSliderBar*		cSliderBar::GetMe(TiXmlElement*e_pElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cSliderBar::TypeID);
		const wchar_t*l_strSliderName = 0;
		cSliderBar*l_pSliderBar = 0;
		cBaseImage*l_pBarImage = 0;
		cBaseImage*l_pSliderImage = 0;
		cSubMPDI*l_pSliderSubMPD = 0;
		cSubMPDI*l_pBarSubMPD = 0;
		eOrientation	l_eOrientation = eO_HORIZONTAL;
		Vector4	l_vCollideRange = Vector4::Zero;
		Vector3	l_vPos;
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("Name")
			{
				l_strSliderName = l_strValue;
			}
			else
			COMPARE_NAME("ShowPos")
			{
				l_vPos = VALUE_TO_VECTOR3;
			}
			else
			COMPARE_NAME("CollideRange")
			{
				l_vCollideRange = VALUE_TO_VECTOR4;
			}
			else
			COMPARE_NAME("Orientation")
			{
				l_eOrientation = ::GetOrientation(l_strValue);
			}
		PARSE_NAME_VALUE_END
		e_pElement = e_pElement->FirstChildElement();
		while( e_pElement )
		{
			l_strValue = e_pElement->Value();
			COMPARE_VALUE("SliderImage")
			{
				l_pSliderImage = dynamic_cast<cBaseImage*>(GetRenderObjectBehavior(e_pElement->FirstChildElement()));
			}
			else
			COMPARE_VALUE("BarImage")
			{
				l_pBarImage = dynamic_cast<cBaseImage*>(GetRenderObjectBehavior(e_pElement->FirstChildElement()));
			}
			else
			COMPARE_VALUE("SliderSubMPDI")
			{
				l_pSliderSubMPD = dynamic_cast<cSubMPDI*>(GetRenderObjectBehavior(e_pElement->FirstChildElement()));
			}
			else
			COMPARE_VALUE("BarSubMPDI")
			{
				l_pBarSubMPD = dynamic_cast<cSubMPDI*>(GetRenderObjectBehavior(e_pElement->FirstChildElement()));
			}
			e_pElement = e_pElement->NextSiblingElement();
		}
		if( l_pSliderSubMPD || l_pBarSubMPD )
		{
			l_pSliderBar = new cSliderBar(l_pBarSubMPD,l_pSliderSubMPD,l_eOrientation);
		}
		else
		if( l_pSliderImage || l_pBarImage )
		{
			l_pSliderBar = new cSliderBar(l_vPos,l_pBarImage,l_pSliderImage,l_eOrientation);
		}
		else
		{
			l_pSliderBar = new cSliderBar(l_vPos,0,0,l_eOrientation);
			
		}
		if( l_vCollideRange.x != 0 ||
			l_vCollideRange.y != 0 ||
			l_vCollideRange.z != 0 ||
			l_vCollideRange.w != 0)
		{
			l_pSliderBar->SetCollisionRange(l_vCollideRange);
		}
		if( l_strSliderName )
			l_pSliderBar->SetName(l_strSliderName);
		SAFE_DELETE(l_pBarSubMPD);
		SAFE_DELETE(l_pSliderSubMPD);
		SAFE_DELETE(l_pSliderImage);
		SAFE_DELETE(l_pBarImage);
		return l_pSliderBar;
	}

	void	cSliderBar::Update(float e_fElpaseTime)
	{
		if(this->m_eObjectMouseBehavior == eOMB_UP || m_eObjectMouseBehavior == eOMB_DOUBLU_CLICK_UP )
			m_eObjectMouseBehavior = eOMB_NONE;
	}

	void	cSliderBar::Render()
	{
		if( this->m_pBar )
		{
			m_pBar->Render();
		}
		if( m_pSlider )
			this->m_pSlider->Render();
		float	l_fHeight = cGameApp::m_spGlyphFontRender->GetGlyphReader()->GetCharInfo((int)0).fHeight;
		cGameApp::m_spGlyphFontRender->RenderFont(this->m_vCollisionRange.x,
			this->m_vCollisionRange.y-l_fHeight-10.f,ValueToStringW((int)this->m_fCurrentValue));
	}

	void	cSliderBar::DebugRender()
	{
		Vector4	l_vNewCollisionRange(m_vCollisionRange);
		if( this->m_pSlider )
		{
			l_vNewCollisionRange.z -= m_pSlider->GetWidth();
		}
		GLRender::RenderRectangle(Vector2(m_vCollisionRange.x,m_vCollisionRange.y),m_vCollisionRange.Width(),m_vCollisionRange.Height(),Vector4::One);
		//if( m_pSlider )
		{
			Vector3	l_vPos(l_vNewCollisionRange.x,l_vNewCollisionRange.y,0.f);
			float	l_fWidth = 100.f;
			float	l_fHeight = 100.f;
			if( this->m_eOrientation == eO_HORIZONTAL )
			{
				l_vPos.x = l_vNewCollisionRange.z-m_fCurrentBarValue;
				l_fWidth = l_vNewCollisionRange.Width()/(this->m_fMaxValue-this->m_fMinValue);
				l_fHeight = l_vNewCollisionRange.Height();
			}
			else
			{
				l_vPos.y = l_vNewCollisionRange.w-m_fCurrentBarValue;
				l_fWidth = l_vNewCollisionRange.Width();
				l_fHeight = l_vNewCollisionRange.Height()/(this->m_fMaxValue-this->m_fMinValue);
			}
			GLRender::RenderRectangle(Vector2(l_vPos.x,l_vPos.y),l_fWidth,l_fHeight,Vector4::One);
		}
	}

	void	cSliderBar::SetValue(float e_fValue)
	{
		Vector4	l_vNewCollisionRange(m_vCollisionRange);
		if( this->m_pSlider )
		{
			l_vNewCollisionRange.z -= m_pSlider->GetWidth();
		}
		if(e_fValue>this->m_fMaxValue)
		{
			e_fValue = m_fMaxValue;
		}
		else
		if(e_fValue<this->m_fMinValue)
		{
			e_fValue = m_fMinValue;
		}
		this->m_fCurrentValue = e_fValue;
		assert(m_fMaxValue>m_fMinValue);
		float	l_fPercentage = e_fValue/(m_fMaxValue-m_fMinValue);
		l_fPercentage = 1-l_fPercentage;
		if( this->m_eOrientation == eO_HORIZONTAL )
		{
			this->m_fCurrentBarValue = UT::LERP<float>(0,l_vNewCollisionRange.z-l_vNewCollisionRange.x,l_fPercentage);
			//if in debug render comment this.......?
			m_fCurrentBarValue = l_vNewCollisionRange.z -m_fCurrentBarValue;
			if(this->m_pSlider)
				m_pSlider->SetPos(Vector2(m_fCurrentBarValue,m_pSlider->GetLocalPositionPointer()->y));
		}
		else
		{
			this->m_fCurrentBarValue = UT::LERP<float>(0,l_vNewCollisionRange.w-l_vNewCollisionRange.y,l_fPercentage);
			//if in debug render comment this.......?
			m_fCurrentBarValue = l_vNewCollisionRange.w -m_fCurrentBarValue;
			if(this->m_pSlider)
				m_pSlider->SetPos(Vector2(m_pSlider->GetLocalPositionPointer()->x,m_fCurrentBarValue));
		}
	}
	void	cSliderBar::Slide(int e_iPosX,int e_iPosY)
	{
		Vector4	l_vNewCollisionRange(m_vCollisionRange);
		if( this->m_pSlider )
		{
			l_vNewCollisionRange.z -= m_pSlider->GetWidth();
		}
		e_iPosX = l_vNewCollisionRange.ClampWidth(e_iPosX);
		e_iPosY = l_vNewCollisionRange.ClampHeight(e_iPosY);
		if( this->m_eOrientation == eO_HORIZONTAL )
		{
			this->m_fCurrentBarValue = l_vNewCollisionRange.z-e_iPosX;
		}
		else
		{
			this->m_fCurrentBarValue = l_vNewCollisionRange.w-e_iPosY;
		}
		float	l_fPercentage;
		if( this->m_eOrientation == eO_HORIZONTAL )
			l_fPercentage = m_fCurrentBarValue/(l_vNewCollisionRange.Width());
		else
			l_fPercentage = m_fCurrentBarValue/(l_vNewCollisionRange.Height());
		l_fPercentage = 1.f-l_fPercentage;
		this->m_fCurrentValue = UT::LERP<float>(this->m_fMinValue,this->m_fMaxValue,l_fPercentage);	
		if(this->m_pSlider)
		{
			if( this->m_eOrientation == eO_HORIZONTAL )
				m_pSlider->SetPos(Vector2((float)e_iPosX,m_pSlider->GetLocalPositionPointer()->y));
			else
				m_pSlider->SetPos(Vector2(m_pSlider->GetLocalPositionPointer()->x,(float)e_iPosY));
		}
	}

	void    cSliderBar::InternalMouseDown(int e_iPosX,int e_iPosY)
	{
		Slide(e_iPosX,e_iPosY);
	}

	void    cSliderBar::InternalMouseMove(int e_iPosX,int e_iPosY)
	{
		Slide(e_iPosX,e_iPosY);
	}

	void    cSliderBar::InternalMouseUp(int e_iPosX,int e_iPosY)
	{
		Slide(e_iPosX,e_iPosY);
	}

	void	cSliderBar::SetPos(Vector3 e_vPos)
	{
		if( m_pBar )
		{
			Vector3	l_vOffsetPos = *m_pBar->GetLocalPositionPointer()-*m_pSlider->GetLocalPositionPointer();
			this->m_pBar->SetPos(e_vPos);
			if(this->m_eOrientation == eO_HORIZONTAL)
				l_vOffsetPos.y-=(m_pBar->GetHeight())/2.f;
			else
				l_vOffsetPos.x-=(m_pBar->GetWidth())/2.f;
			this->m_pSlider->SetPos(l_vOffsetPos+e_vPos);
		}
		this->m_vCollisionRange.x+= e_vPos.x;
		this->m_vCollisionRange.y+= e_vPos.y;
		this->m_vCollisionRange.z+= e_vPos.x;
		this->m_vCollisionRange.w+= e_vPos.y;
	}
//end namespace
}