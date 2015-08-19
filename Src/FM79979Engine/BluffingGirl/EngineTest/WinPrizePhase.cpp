#include "stdafx.h"
#include "WinPrizePhase.h"

cWinPrizePhase::cWinPrizePhase()
{
	m_strVideoFileName = L"Video/123.fmv";
	this->SetName(WIN_PRIZE_PHASE);
	m_pFMPlayer = 0;
	m_TimeToHideButton.SetTargetTime(3.f);
	m_bFirstMouseUpForShowLeaveButton = false;
	m_pLeaveButton = 0;
	this->Parse("PhaseData/WinPrizePhase.xml");
}

cWinPrizePhase::~cWinPrizePhase()
{
	SAFE_DELETE(m_pFMPlayer);
	SAFE_DELETE(m_pLeaveButton);
}
//<cWinPrizePhase>
//	<LeaveButton HideTime="2">
//		<cImageButton Name="TestButton" MPDIList="Image/Dice/Dice.mpdi" MPDI="aabb"/>
//	<LeaveButton>
//</cWinPrizePhase>
void	cWinPrizePhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("LeaveButton")
	{
		const WCHAR*l_strHideTime = e_pTiXmlElement->Attribute(L"HideTime");
		m_TimeToHideButton.SetTargetTime(GetFloat(l_strHideTime));
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		if( e_pTiXmlElement )
		{
			m_pLeaveButton = cImageButton::GetMe(e_pTiXmlElement);
		}
	}
}

void	cWinPrizePhase::Destroy()
{
	SAFE_DELETE(m_pFMPlayer);
}

void	cWinPrizePhase::Init()
{
	m_bFirstMouseUpForShowLeaveButton = false;
	SAFE_DELETE(m_pFMPlayer);
	m_pFMPlayer = new cFMVVideo();
	if(!m_pFMPlayer->OpenFile(UT::WcharToChar(m_strVideoFileName.c_str()).c_str()))
	{
		this->m_bSatisfiedCondition = true;
		UT::ErrorMsg(L"file is not exist",m_strVideoFileName.c_str());
		//file is not exists
		return;
	}
}

void	cWinPrizePhase::Update(float e_fElpaseTime)
{
	if( m_bFirstMouseUpForShowLeaveButton )
	{
		m_TimeToHideButton.Update(e_fElpaseTime);
		if( m_TimeToHideButton.bTragetTimrReached )
		{
			m_pLeaveButton->Init();
			m_bFirstMouseUpForShowLeaveButton = false;
		}
		else
		{
			if( m_pLeaveButton )
			{
				m_pLeaveButton->Update(e_fElpaseTime);
				if( m_pLeaveButton->IsSatisfiedCondition() )
				{
					this->m_bSatisfiedCondition = true;
				}
			}
		}
	}
	if( m_pFMPlayer )
	{
		if( !m_pFMPlayer->m_bUsingMultiThread )
			m_pFMPlayer->Update(e_fElpaseTime);
		if(m_pFMPlayer->IsPlayDone())
		{
			this->m_bSatisfiedCondition = true;
		}
	}
	if(this->m_bSatisfiedCondition && 
		wcscmp(STORE_PHASE_NAME,m_strNextPhaseName.c_str()) &&
		wcscmp(SETUP_PHASE_NAME,m_strNextPhaseName.c_str()))
	{
		Destroy();
		cBasicSound*l_pSound = cGameApp::GetSoundByFileName(L"Sound/MainBG.ogg");
		if(l_pSound)
		{
			l_pSound->Play(true);
			l_pSound->SetLoop(true);
		}
	}
}
//it would be called if the stage is changed.
void	cWinPrizePhase::Render()
{
	if( m_pFMPlayer )
	{
		m_pFMPlayer->Render();
	}
	if( m_bFirstMouseUpForShowLeaveButton )
	{
		if( m_pLeaveButton )
			m_pLeaveButton->Render();
	}
}

void	cWinPrizePhase::DebugRender()
{

}

void	cWinPrizePhase::MouseDown(int e_iX,int e_iY)
{
	if( m_bFirstMouseUpForShowLeaveButton && m_pLeaveButton )
		m_pLeaveButton->MouseDown(e_iX,e_iY);
	else
	{
		if( m_pLeaveButton )
			m_pLeaveButton->Init();
	}
}

void	cWinPrizePhase::MouseMove(int e_iX,int e_iY)
{
	if( m_bFirstMouseUpForShowLeaveButton && m_pLeaveButton )
		m_pLeaveButton->MouseMove(e_iX,e_iY);
}

void	cWinPrizePhase::MouseUp(int e_iX,int e_iY)
{
	if( m_bFirstMouseUpForShowLeaveButton && m_pLeaveButton )
		m_pLeaveButton->MouseUp(e_iX,e_iY);
	else
	{
		m_bFirstMouseUpForShowLeaveButton = true;
		m_TimeToHideButton.Start();
	}
}

const	WCHAR*	cWinPrizePhase::GetNextPhaseName()
{
	return CHOICE_GIRL_STAGE_PHASE_NAME;
	return CHOICE_GIRL_PHASE_NAME;
}

void			cWinPrizePhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	if( e_pData )
		m_strVideoFileName = (WCHAR*)e_pData;
}