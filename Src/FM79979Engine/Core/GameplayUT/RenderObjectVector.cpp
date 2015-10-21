#include "../stdafx.h"
#include "../AllImageInclude.h"
#include "Mouse_Touch.h"
#include "FontFrame.h"
#include "RenderObjectVector.h"
#include "ObjectXMLParser.h"
#include "GameApp.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cRenderObjectVector);

	cRenderObjectVector::cRenderObjectVector()
	{

	}

	cRenderObjectVector::~cRenderObjectVector()
	{
	
	}

	cRenderObjectVector::cRenderObjectVector(cRenderObjectVector*e_pFontFrameVector):cBehaviorObjectList<cRenderObject>(e_pFontFrameVector)
	{
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			this->GetObject(i)->SetParent(this,false);
		}
	}
//<cRenderObjectVector>
//	<cFontWithFrame GlyphRenderName="" TextBGColor="" Text="" Pos=""/>
//		<SubMPDI cMPDIList="" cMPDI="" cSubMPDI=""/>
//		<ConnectRadianImage>
//			base image
//		</ConnectRadianImage>
//		<LineImage>
//			base image
//		</LineImage>
//	</cFontWithFrame/>
//<cRenderObjectVector>
	cRenderObjectVector*cRenderObjectVector::GetMe(TiXmlElement*e_pElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cRenderObjectVector::TypeID);
		cRenderObjectVector*l_pRenderObjectVector = new cRenderObjectVector();
		e_pElement = e_pElement->FirstChildElement();
		while( e_pElement )
		{
			const wchar_t*l_strValue = e_pElement->Value();
			cRenderObject*l_pObject = GetRenderObjectBehavior(e_pElement);
			if( l_pObject )
				l_pRenderObjectVector->AddObjectNeglectExist(l_pObject);
			e_pElement = e_pElement->NextSiblingElement();
		}
		return l_pRenderObjectVector;
	}

	void	cRenderObjectVector::AddData(wchar_t*e_strText,cGlyphFontRender*e_pGlyphFontRender,TiXmlElement*e_pSubMPDIElement)
	{
		cSubMPDI*l_pSubMPDI = cSubMPDI::GetMe(e_pSubMPDIElement);
		AddData(e_strText,e_pGlyphFontRender,l_pSubMPDI);
	}

	void	cRenderObjectVector::AddData(wchar_t*e_strText,cGlyphFontRender*e_pGlyphFontRender,cSubMPDI*e_pPos)
	{
		cFontWithFrame*l_pFontWithFrame = new cFontWithFrame(cGameApp::m_spGlyphFontRender,e_strText);
		if(e_pPos->GetOriginalPointList().size() > 0)
			l_pFontWithFrame->SetLocalPosition(e_pPos->GetOriginalPointList()[0]);
		this->AddObjectNeglectExist(l_pFontWithFrame);
		//l_pFontWithFrame->SetDestroyConnectionWhileDestoruction(false);
		AddChild(l_pFontWithFrame);
	}

	void	cRenderObjectVector::ChangeData(int e_iIndex,const wchar_t*e_strText)
	{
		cRenderObject*l_pRenderObject = this->GetObject(e_iIndex);
		if( l_pRenderObject->Type() == cFontWithFrame::TypeID )
		{
			cFontWithFrame*l_pFontWithFrame = (cFontWithFrame*)this->GetObject(e_iIndex);
			l_pFontWithFrame->SetText(e_strText);
		}
	}

	//void	cRenderObjectVector::Init()
	//{
	//
	//}

	//void	cRenderObjectVector::Update(float e_fElpaseTime)
	//{
	//
	//}

	//void	cRenderObjectVector::Render()
	//{
	//	int	l_iCount = this->Count();
	//	for(int i=0;i<l_iCount;++i )
	//	{
	//		this->GetObject(i)->Render();
	//	}
	//}
//end namespace FATMING_CORE
}