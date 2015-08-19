#include "stdafx.h"
#include "GirlIntroducingPhase.h"
#include "BluffingGirlApp.h"
#include "ChoiceGirlStagePhase.h"
cGirlIntroducingPhase::cGirlIntroducingPhase()
{
	m_strVideoFileName = L"Video/123.fmv";
	this->SetName(GIRL_INTRODUCING_PHASE_NAME);
	m_pFMPlayer = 0;
	m_TimeToHideButton.SetTargetTime(3.f);
	m_bFirstMouseUpForShowLeaveButton = false;
	m_pLeaveButton = 0;
	this->Parse("PhaseData/GirlIntroducingPhase.xml");
}

cGirlIntroducingPhase::~cGirlIntroducingPhase()
{
	SAFE_DELETE(m_pFMPlayer);
	SAFE_DELETE(m_pLeaveButton);
}
//<cGirlIntroducingPhase>
//	<LeaveButton HideTime="2">
//		<cImageButton Name="TestButton" MPDIList="Image/Dice/Dice.mpdi" MPDI="aabb"/>
//	<LeaveButton>
//</cGirlIntroducingPhase>
void	cGirlIntroducingPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
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

void	cGirlIntroducingPhase::Destroy()
{
	SAFE_DELETE(m_pFMPlayer);
	cGameApp::m_spSoundParser->RemoveAllBGM();
}

void	cGirlIntroducingPhase::Init()
{
	Destroy();
	m_bFirstMouseUpForShowLeaveButton = false;
	m_pFMPlayer = new cFMVVideo();
	if(!m_pFMPlayer->OpenFile(UT::WcharToChar(m_strVideoFileName.c_str()).c_str()))
	{
		this->m_bSatisfiedCondition = true;
		UT::ErrorMsg(L"file is not exist",m_strVideoFileName.c_str());
		SAFE_DELETE(m_pFMPlayer);
		//file is not exists
		return;
	}
}

void	cGirlIntroducingPhase::Update(float e_fElpaseTime)
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
			if( m_pFMPlayer )
			{
				m_pFMPlayer->Destroy();
			}
			this->m_bSatisfiedCondition = true;
		}
	}
	if( m_bSatisfiedCondition )
	{
		Destroy();
	}
}
//it would be called if the stage is changed.
void	cGirlIntroducingPhase::Render()
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

void	cGirlIntroducingPhase::DebugRender()
{

}

void	cGirlIntroducingPhase::MouseDown(int e_iX,int e_iY)
{
	if( m_bFirstMouseUpForShowLeaveButton && m_pLeaveButton )
		m_pLeaveButton->MouseDown(e_iX,e_iY);
	else
	{
		if( m_pLeaveButton )
			m_pLeaveButton->Init();
	}
}

void	cGirlIntroducingPhase::MouseMove(int e_iX,int e_iY)
{
	if( m_bFirstMouseUpForShowLeaveButton && m_pLeaveButton )
		m_pLeaveButton->MouseMove(e_iX,e_iY);
}

void	cGirlIntroducingPhase::MouseUp(int e_iX,int e_iY)
{
	if( m_bFirstMouseUpForShowLeaveButton && m_pLeaveButton )
		m_pLeaveButton->MouseUp(e_iX,e_iY);
	else
	{
		m_bFirstMouseUpForShowLeaveButton = true;
		m_TimeToHideButton.Start();
	}
}

const	WCHAR*	cGirlIntroducingPhase::GetNextPhaseName()
{
	return BLUFFING_DICEGAME_PHASE_NAME;
}

void	cGirlIntroducingPhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	cChoiceGirlStagePhase*l_pPhase = (cChoiceGirlStagePhase*)cBluffingGirlApp::GetPhase(e_strPhaseName);
	if( l_pPhase )
	{	
		cChoiceGirlStagePhase::cStageData*l_pStageData = l_pPhase->GetCurrentStageData();
		m_strVideoFileName = l_pStageData->m_strGirlIntroducingVideoFileName;
	}
	//if( e_pData )
	//	m_strVideoFileName = (WCHAR*)e_pData;
}