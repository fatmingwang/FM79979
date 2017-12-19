#include "stdafx.h"
#include "GameRulePhase.h"

cGameRulePhase::cGameRulePhase()
{
	this->SetName(RULE_PHASE_NAME);
	m_pScroller = 0;
	m_pBackButton = 0;
	m_pBGMPDI = 0;	
	PARSE_FAILED_MESSAGE_BOX(this,"BluffingGirl/PhaseData/Rule.xml");
}

cGameRulePhase::~cGameRulePhase()
{
	SAFE_DELETE(m_pScroller);
	SAFE_DELETE(m_pBackButton);
}

void	cGameRulePhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("cScroller")
	{
		m_pScroller = cScroller::GetMe(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("BackButton")
	{
		if( e_pTiXmlElement->FirstChildElement() )
		{
			m_pBackButton = cImageButton::GetMe(e_pTiXmlElement->FirstChildElement());
		}
	}
	else
	COMPARE_VALUE("BGAnimation")
	{
		m_pBGMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		if( m_pBGMPDI )
			m_pBGMPDI->Init();
	}
}

//set data to correct
void	cGameRulePhase::Init()
{
	if( m_pBackButton )
		m_pBackButton->Init();
	if( m_pScroller )
		m_pScroller->Init();
}

void	cGameRulePhase::Update(float e_fElpaseTime)
{
	if( m_pBGMPDI )
		m_pBGMPDI->Update(e_fElpaseTime);
	if( m_pBackButton )
	{
		m_pBackButton->Update(e_fElpaseTime);
		if(m_pBackButton->IsSatisfiedCondition())
		{
			this->m_bSatisfiedCondition = true;
		}
	}
	if( m_pScroller )
	{
		m_pScroller->Update(e_fElpaseTime);
		if( m_pScroller->IsSatisfiedCondition() )
		{
			m_pScroller->SetSatisfiedCondition(false);
		}
	}
}

void	cGameRulePhase::Render()
{
	glClearColor( 1,1.0,1.0,1 );
	if( m_pBGMPDI )
		m_pBGMPDI->Render();
	if( m_pBackButton )
		m_pBackButton->Render();
	if( m_pScroller )
		m_pScroller->Render();
}

void	cGameRulePhase::DebugRender()
{

}

//
void	cGameRulePhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pBackButton )
		m_pBackButton->MouseMove(e_iPosX,e_iPosY);
	if( m_pScroller )
		m_pScroller->MouseMove(e_iPosX,e_iPosY);
}

void	cGameRulePhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pBackButton )
		m_pBackButton->MouseDown(e_iPosX,e_iPosY);
	if( m_pScroller )
		m_pScroller->MouseDown(e_iPosX,e_iPosY);
}

void	cGameRulePhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pBackButton )
		m_pBackButton->MouseUp(e_iPosX,e_iPosY);
	if( m_pScroller )
		m_pScroller->MouseUp(e_iPosX,e_iPosY);
}

const   WCHAR*	cGameRulePhase::GetNextPhaseName()
{
	return m_strInPhaseName.c_str();
}

void	cGameRulePhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	m_strInPhaseName = e_strPhaseName;
}