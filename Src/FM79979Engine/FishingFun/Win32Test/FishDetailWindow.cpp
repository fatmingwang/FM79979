#include "stdafx.h"
#include "FishDetailWindow.h"

cFishDetailWindow::cFishDetailWindow(TiXmlElement*e_pElement)
{
	m_pRenderObjectVector = 0;
	m_pBGMPDI = 0;
	m_pCloseButton = 0;
	m_pFishDetailButton = 0;
	e_pElement = e_pElement->FirstChildElement();
	while( e_pElement )
	{
		const WCHAR*l_strValue = e_pElement->Value();
		COMPARE_VALUE_WITH_DEFINE(cImageButton::TypeID)
		{
			cImageButton*l_pImageButton = cImageButton::GetMe(e_pElement);
			if(l_pImageButton->IsSameName(L"CloseDetail"))
			{
				m_pCloseButton = l_pImageButton;
			}
			else
			if(l_pImageButton->IsSameName(L"Detail"))
			{
				m_pFishDetailButton = l_pImageButton;
			}
			else
			{
				SAFE_DELETE(l_pImageButton);
			}
		}
		else
		COMPARE_VALUE("BG")
		{
			m_pBGMPDI = cMPDI::GetMe(e_pElement->FirstChildElement(),false);
			if( m_pBGMPDI )
				m_pBGMPDI->Init();
		}
		else
		COMPARE_VALUE_WITH_DEFINE(cRenderObjectVector::TypeID)
		{
			m_pRenderObjectVector = cRenderObjectVector::GetMe(e_pElement);
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
}

cFishDetailWindow::~cFishDetailWindow()
{
	SAFE_DELETE(m_pRenderObjectVector);
	SAFE_DELETE(m_pCloseButton);
	SAFE_DELETE(m_pFishDetailButton);
}
void    cFishDetailWindow::Update(float e_fElpaseTime)
{
	if( m_pBGMPDI )
		m_pBGMPDI->Update(e_fElpaseTime);
	if( m_pCloseButton )
	{
		m_pCloseButton->Update(e_fElpaseTime);
		if( m_pCloseButton->IsSatisfiedCondition() )
		{
			m_pFishDetailButton->Init();
			m_pCloseButton->Init();
		}
	}
	if( m_pFishDetailButton )
		m_pFishDetailButton->Update(e_fElpaseTime);
}
void    cFishDetailWindow::Render()
{
	if( m_pBGMPDI )
		m_pBGMPDI->Render();
	if( m_pFishDetailButton )
	{
		m_pFishDetailButton->Render();
		if( m_pFishDetailButton->IsSatisfiedCondition() )
		{
			if( m_pCloseButton )
				m_pCloseButton->Render();
		}
	}
}
void	cFishDetailWindow::SetupData(const WCHAR*e_strFishName)
{

}

void    cFishDetailWindow::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pFishDetailButton )
	{
		m_pFishDetailButton->MouseDown(e_iPosX,e_iPosY);
		if( m_pFishDetailButton->IsSatisfiedCondition() )
		{
			if( m_pCloseButton )
				m_pCloseButton->MouseDown(e_iPosX,e_iPosY);
		}
	}
}

void    cFishDetailWindow::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pFishDetailButton )
	{
		m_pFishDetailButton->MouseMove(e_iPosX,e_iPosY);
		if( m_pFishDetailButton->IsSatisfiedCondition() )
		{
			if( m_pCloseButton )
				m_pCloseButton->MouseMove(e_iPosX,e_iPosY);
		}
	}
}

void    cFishDetailWindow::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pFishDetailButton )
	{
		m_pFishDetailButton->MouseUp(e_iPosX,e_iPosY);
		if( m_pFishDetailButton->IsSatisfiedCondition() )
		{
			if( m_pCloseButton )
				m_pCloseButton->MouseUp(e_iPosX,e_iPosY);
		}
	}
}