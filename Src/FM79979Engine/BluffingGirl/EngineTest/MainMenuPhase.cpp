#include "stdafx.h"
//#include <jni.h>
#include "MainMenuPhase.h"
#include "AllPhaseDefine.h"
#include "IABSetup.h"
#include "BluffingGirlApp.h"
#include "PlayerData.h"

cMainMenuPhase::cMainMenuPhase(const char*e_strMainMenuFileName)
{
	this->SetName(MAIN_PHASE_NAME);
	m_pBGMPDI = 0;
	m_strFileName = e_strMainMenuFileName;
	m_SelectionButton.SetAllObjectsDoneToSatisfied(false);
}

cMainMenuPhase::~cMainMenuPhase()
{
	Destroy();
}

void	cMainMenuPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("PhaseData")
	{
		ProcessPhaseData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("Button")
	{
		m_MainfunctionButton.AddButton(e_pTiXmlElement->FirstChildElement());
	}
	else
	COMPARE_VALUE("GoToSelectGirl")
	{
		TiXmlElement*l_pButtonList = e_pTiXmlElement->FirstChildElement();
		while( l_pButtonList )
		{
			cImageButton*l_pImageButton = cImageButton::GetMe(l_pButtonList);
			if( l_pImageButton )
				m_SelectionButton.AddObjectNeglectExist(l_pImageButton);
			l_pButtonList = l_pButtonList->NextSiblingElement();
		}
	}
	else
	COMPARE_VALUE("BG")
	{
		m_pBGMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		if( m_pBGMPDI )
			m_pBGMPDI->Init();
	}
}

void	cMainMenuPhase::ProcessPhaseData(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("ClickSound")
		{
			m_pClickSound = cGameApp::GetSoundByFileName(l_strValue);
		}
	PARSE_NAME_VALUE_END
}

void	cMainMenuPhase::Init()
{
	m_bSatisfiedCondition = false;
	Destroy();
	m_ResourceStamp.StampResource1();
	if(!this->Parse(m_strFileName.c_str()))
	{
		UT::ErrorMsg(m_strFileName.c_str(),"parse failed!!");
	}
	m_SelectionButton.Init();
	m_MainfunctionButton.Init();
	m_ResourceStamp.StampResource2();
	//m_ResourceStamp.ReleaseNewResource();
}
void	cMainMenuPhase::Destroy()
{
	m_MainfunctionButton.Destroy();
	m_SelectionButton.Destroy();
	m_ResourceStamp.ReleaseNewResource();
	m_pBGMPDI = 0;
}

void	cMainMenuPhase::Update(float e_fElpaseTime)
{
	m_MainfunctionButton.Update(e_fElpaseTime);
	if( m_pBGMPDI )
		m_pBGMPDI->Update(e_fElpaseTime);
	m_SelectionButton.Update(e_fElpaseTime);
	if( m_SelectionButton.IsSatisfiedCondition() )
	{
		if( !m_SelectionButton.GetCurrentWorkingObject() )
		{
			return;
		}
		bool	l_bIntoStore = false;
		bool	l_bSatisfied = true;
		const WCHAR*l_strName = m_SelectionButton.GetCurrentWorkingObject()->GetName();
		if( !wcscmp(l_strName,CHOICE_GIRL_PHASE_NAME) )
		{
			m_strNextPhaseName = CHOICE_GIRL_PHASE_NAME;
		}
		else
		if( !wcscmp(l_strName,HEART_RECOVER_SELECTGIRL_PHASE_NAME) )
		{
			m_strNextPhaseName = HEART_RECOVER_SELECTGIRL_PHASE_NAME;
		}
		else
		if( !wcscmp(l_strName,FB_PHASE_NAME) )
		{
			m_strNextPhaseName = FB_PHASE_NAME;
			//m_strNextPhaseName = HEART_RECOVER_SELECTGIRL_PHASE_NAME;
		}
		else
		if( !wcscmp(l_strName,SETUP_PHASE_NAME) )
		{
			m_strNextPhaseName = SETUP_PHASE_NAME;
			//m_strNextPhaseName = WIN_PRIZE_PHASE;
		}
		else
		if( !wcscmp(l_strName,STORE_PHASE_NAME) )
		{
			l_bIntoStore = true;
			m_strNextPhaseName = STORE_PHASE_NAME;
		}
		else
		{
			l_bSatisfied = false;
		}
		if( l_bSatisfied )
			cGameApp::SoundPlay(BUTTON_CLICK_SOUND,true);
		m_SelectionButton.Init();
		this->m_bSatisfiedCondition = l_bSatisfied;
		//if( l_bSatification )
		{//play sound
			if( m_pClickSound )m_pClickSound->Play(true);
		}
	}
	if( m_MainfunctionButton.IsSatisfiedCondition() )
	{
		m_strNextPhaseName = m_MainfunctionButton.GetNextPhaseName();
		m_MainfunctionButton.Init();
		this->m_bSatisfiedCondition = true;
	}
	if(this->m_bSatisfiedCondition && 
		wcscmp(STORE_PHASE_NAME,m_strNextPhaseName.c_str()) &&
		wcscmp(SETUP_PHASE_NAME,m_strNextPhaseName.c_str()))
	{
		Destroy();
	}
}
//it would be called if the stage is changed.
void	cMainMenuPhase::Render()
{
	glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	if( m_pBGMPDI )
		m_pBGMPDI->Render();
	m_SelectionButton.Render();
	m_MainfunctionButton.Render();
}

void	cMainMenuPhase::DebugRender()
{
	m_SelectionButton.DebugRender();
	m_MainfunctionButton.DebugRender();
}

void	cMainMenuPhase::MouseDown(int e_iX,int e_iY)
{
	m_SelectionButton.MouseDown(e_iX,e_iY);
	m_MainfunctionButton.MouseDown(e_iX,e_iY);
}

void	cMainMenuPhase::MouseUp(int e_iX,int e_iY)
{
	m_SelectionButton.MouseUp(e_iX,e_iY);
	m_MainfunctionButton.MouseUp(e_iX,e_iY);
}

void	cMainMenuPhase::MouseMove(int e_iX,int e_iY)
{
	m_SelectionButton.MouseMove(e_iX,e_iY);
	m_MainfunctionButton.MouseMove(e_iX,e_iY);
}

const   WCHAR*	cMainMenuPhase::GetNextPhaseName()
{
	return m_strNextPhaseName.c_str();
}

void*	cMainMenuPhase::GetData()
{
	return 0;
}

void	cMainMenuPhase::KeyDown(char e_char)
{
	switch(e_char)
	{
		case 37://left

			break;
		case 38://up

			break;
		case 39://right

			break;
		case 40://down

			break;
	}

}