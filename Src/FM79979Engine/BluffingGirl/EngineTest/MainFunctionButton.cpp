#include "stdafx.h"
//#include <jni.h>
#include "MainFunctionButton.h"
#include "AllPhaseDefine.h"
#include "IABSetup.h"
#include "BluffingGirlApp.h"
#include "PlayerData.h"

cMainfunctionButton::cMainfunctionButton()
{
	this->SetName(L"MainFunctionButton");
	m_pClickSound = 0;
}

cMainfunctionButton::~cMainfunctionButton()
{
	Destroy();
}

void	cMainfunctionButton::AddButton(TiXmlElement*e_pTiXmlElement)
{
	cImageButton*l_pImageButton = cImageButton::GetMe(e_pTiXmlElement);
	if( l_pImageButton )
		m_SelectionButton.AddObjectNeglectExist(l_pImageButton);
	//else
	//COMPARE_VALUE("MainfunctionButton")
	//{
	//	m_pClickSound = cGameApp::GetSoundByFileName(l_strValue);
	//}
}

void	cMainfunctionButton::Init()
{
	m_bSatisfiedCondition = false;
	m_SelectionButton.Init();
}
void	cMainfunctionButton::Destroy()
{
	m_SelectionButton.Destroy();
}
bool g_bFristTimeInto = true;
extern bool		FBPost();
void	cMainfunctionButton::Update(float e_fElpaseTime)
{
	m_SelectionButton.Update(e_fElpaseTime);
	if( m_SelectionButton.IsSatisfiedCondition() )
	{
		bool	l_bSatification = true;
		if( !m_SelectionButton.GetCurrentWorkingObject() )
		{
			return;
		}
		if( m_pClickSound )m_pClickSound->Play(true);
		const WCHAR*l_strName = m_SelectionButton.GetCurrentWorkingObject()->GetName();
#ifdef ANDROID
		if( !wcscmp(l_strName,L"FB") )
		{
			//cGameApp::OpenKeyBoard();
			FBPost();
		}
		else
#endif
		{
			m_strNextPhaseName = l_strName;
			this->m_bSatisfiedCondition = true;
		}
		m_SelectionButton.Init();
	}
}
//it would be called if the stage is changed.
void	cMainfunctionButton::Render()
{
	m_SelectionButton.Render();
}

void	cMainfunctionButton::DebugRender()
{
	m_SelectionButton.DebugRender();
}

void	cMainfunctionButton::MouseDown(int e_iX,int e_iY)
{
	m_SelectionButton.MouseDown(e_iX,e_iY);
}

void	cMainfunctionButton::MouseUp(int e_iX,int e_iY)
{
	m_SelectionButton.MouseUp(e_iX,e_iY);
}

void	cMainfunctionButton::MouseMove(int e_iX,int e_iY)
{
	m_SelectionButton.MouseMove(e_iX,e_iY);
}

const   WCHAR*	cMainfunctionButton::GetNextPhaseName()
{
	return m_strNextPhaseName.c_str();
}

bool	cMainfunctionButton::IsCallDestroy()
{
	if(!wcscmp(m_strNextPhaseName.c_str(),STORE_PHASE_NAME) ||
		!wcscmp(m_strNextPhaseName.c_str(),SETUP_PHASE_NAME))
		return false;
	return true;
}