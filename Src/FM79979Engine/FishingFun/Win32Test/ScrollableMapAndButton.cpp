#include "stdafx.h"
#include "ScrollableMapAndButton.h"

//<ScrollableMapAndButton>
//	<BG>
//		<cMPDI cMPDIList="" cMPDI="">
//	</BG>
//	<Buttons>
//		<cButtonDialog  AllElementIsButton="true">
//			<cMPDI MPDIList="" MPDI=""/>
//		</cButtonDialog>
//	</Buttons>
//</ScrollableMapAndButton>

cScrollableMapAndButton::cScrollableMapAndButton(TiXmlElement*e_pElement)
{
	m_pOrthogonalCamera = new cOrthogonalCamera(cGameApp::m_spOpenGLRender->m_vGameResolution);
	m_pMapBGMPDI = 0;
	m_pStagesButton = 0;
	m_vViewableRect = cGameApp::m_spOpenGLRender->m_vViewPortSize;
	const WCHAR*l_strAttribute = e_pElement->Attribute(L"ViewRect");
	m_vViewableRect = GetVector4(l_strAttribute);
	e_pElement = e_pElement->FirstChildElement();
	while(e_pElement)
	{
		const WCHAR*l_strValue = e_pElement->Value();
		COMPARE_VALUE("BG")
		{
			cMPDI*l_pMPDI = cMPDI::GetMe(e_pElement->FirstChildElement());
			//Vector3	l_vPos = Vector3::Zero;
			//Vector2	l_vSize = l_pMPDI->GetDrawSize();
			//Vector4	l_vRect1(l_vPos.x,l_vPos.y,l_vPos.x+l_vSize.x,l_vPos.y+l_vSize.y);
			//m_vViewableRect = l_vRect1;
			m_pMapBGMPDI = l_pMPDI;
			m_pMapBGMPDI->Init();
		}
		else
		COMPARE_VALUE("cButtonDialog")
		{
			m_pStagesButton = dynamic_cast<cClickMouseBehaviorVector<cClickMouseBehavior>*>(GetClickMouseBehavior(e_pElement));
			if( m_pStagesButton )
				m_pStagesButton->Init();
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
}

cScrollableMapAndButton::~cScrollableMapAndButton()
{
	SAFE_DELETE(m_pMapBGMPDI);
	SAFE_DELETE(m_pStagesButton);
	SAFE_DELETE(m_pOrthogonalCamera);
}


const WCHAR*cScrollableMapAndButton::GetSelectStageName()
{
	if( m_pStagesButton )
	{
		return m_pStagesButton->GetCurrentWorkingObjectName();
	}
	return 0;
}

void	cScrollableMapAndButton::Init()
{
	if( m_pStagesButton )
		m_pStagesButton->Init();
}

void	cScrollableMapAndButton::Update(float e_fElpaseTime)
{
	if( m_pMapBGMPDI )
		m_pMapBGMPDI->Update(e_fElpaseTime);
	if( m_pStagesButton )
	{
		m_pStagesButton->Update(e_fElpaseTime);
		if(m_pStagesButton->IsSatisfiedCondition())
		{
			//m_pStagesButton->Init();
		}
	}
}

void	cScrollableMapAndButton::Render()
{
	if( m_pOrthogonalCamera )
		m_pOrthogonalCamera->Render();
	if( m_pMapBGMPDI )
		m_pMapBGMPDI->Render();
	if( m_pStagesButton )
	{
		m_pStagesButton->Render();
	}
}

void	cScrollableMapAndButton::MouseDown(int e_iPosX,int e_iPosY)
{
	m_MouseDownPos.x = e_iPosX;
	m_MouseDownPos.y = e_iPosY;
	Vector4	l_vViewRect = m_pOrthogonalCamera->GetViewRect();
	e_iPosX += (int)l_vViewRect.x;
	e_iPosY += (int)l_vViewRect.y;
	if( m_pStagesButton )
		m_pStagesButton->MouseDown(e_iPosX,e_iPosY);
}

void	cScrollableMapAndButton::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pMapBGMPDI )
	{
		int	l_iPosX = e_iPosX-m_MouseDownPos.x;
		int	l_iPosY = e_iPosY-m_MouseDownPos.y;
		l_iPosX *= -1;
		l_iPosY *= -1;
		Vector4	l_vViewRect = m_pOrthogonalCamera->GetViewRect();
		Vector2	l_vSize(l_vViewRect.Width(),l_vViewRect.Height());
		l_vViewRect.Move(l_iPosX,l_iPosY);
		if( l_vViewRect.x < 0 ){	l_vViewRect.x = 0;l_vViewRect.z = l_vSize.x;}
		if( l_vViewRect.z > m_vViewableRect.z ){	l_vViewRect.z = m_vViewableRect.z;l_vViewRect.x = l_vViewRect.z-l_vSize.x;}
		if( l_vViewRect.y < 0 ){	l_vViewRect.y = 0;l_vViewRect.w = l_vSize.y;}
		if( l_vViewRect.w > m_vViewableRect.w ){	l_vViewRect.w = m_vViewableRect.w;l_vViewRect.y = l_vViewRect.w-l_vSize.y;}
		m_pOrthogonalCamera->SetViewRect(l_vViewRect);
		m_MouseDownPos.x = e_iPosX;
		m_MouseDownPos.y = e_iPosY;
		//m_pMapBGMPDI->Update(e_fElpaseTime);
	}
	Vector4	l_vViewRect = m_pOrthogonalCamera->GetViewRect();
	e_iPosX += (int)l_vViewRect.x;
	e_iPosY += (int)l_vViewRect.y;
	if( m_pStagesButton )
	{
		m_pStagesButton->MouseMove(e_iPosX,e_iPosY);
	}
}

void	cScrollableMapAndButton::MouseUp(int e_iPosX,int e_iPosY)
{
	Vector4	l_vViewRect = m_pOrthogonalCamera->GetViewRect();
	e_iPosX += (int)l_vViewRect.x;
	e_iPosY += (int)l_vViewRect.y;
	if( m_pStagesButton )
		m_pStagesButton->MouseUp(e_iPosX,e_iPosY);
}