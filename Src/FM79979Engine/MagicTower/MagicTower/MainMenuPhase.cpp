#include "stdafx.h"
#include "MainMenuPhase.h"
cMainMenuPhase::cMainMenuPhase(const WCHAR*e_strMPDIListFileName,int e_iOptionsCount,int e_iGCMPDIIndex)
{
	m_iCurrentSelectedIndex = -1;
	m_pBGMPDI = 0;
	for(int i=1;i<e_iOptionsCount;++i)
		m_ppSelectionMPDI[i] = 0;
	m_strMainMenuMPDIFileName = cMPDIList::FileToMPDIListName(e_strMPDIListFileName);
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(e_strMPDIListFileName);
	m_iOptionsCount = e_iOptionsCount;
	m_ppSelectionMPDI = new cMPDI*[e_iOptionsCount];
	m_pBGMPDI = l_pMPDIList->GetObject(0);;
	m_pBGMPDI->Init();
	for(int i=1;i<m_iOptionsCount;++i)
	{
		m_ppSelectionMPDI[i] = l_pMPDIList->GetObject(i);
		m_ppSelectionMPDI[i]->Init();
	}
	this->m_bKillAfterSatisfiedCondition = true;
}

cMainMenuPhase::~cMainMenuPhase()
{
	cGameApp::m_spAnimationParser->RemoveObject(m_strMainMenuMPDIFileName.c_str());
}

void	cMainMenuPhase::Init()
{
	m_iCurrentSelectedIndex = -1;
}

void	cMainMenuPhase::Update(float e_fElpaseTime)
{
}
//it would be called if the stage is changed.
void	cMainMenuPhase::Render()
{
	if( m_iCurrentSelectedIndex != -1 )
	{
		m_ppSelectionMPDI[m_iCurrentSelectedIndex]->SetCurrentColor(Vector4(1.5f,1.5f,1.5f,1.f));
	}
}

void	cMainMenuPhase::DebugRender()
{

}

void	cMainMenuPhase::MouseDown(int e_iX,int e_iY)
{
	for(int i=1;i<m_iOptionsCount;++i)
	{
		if(m_ppSelectionMPDI[i]->Collide(e_iX,e_iY))
		{
			m_iCurrentSelectedIndex = i;
		}
	}
}

void	cMainMenuPhase::MouseUp(int e_iX,int e_iY)
{
	for(int i=1;i<m_iOptionsCount;++i)
	{
		if(m_ppSelectionMPDI[i]->Collide(e_iX,e_iY))
		{
			if( m_iCurrentSelectedIndex == i )
			{
				break;
			}
		}
	}
	m_iCurrentSelectedIndex = -1;
}

void	cMainMenuPhase::MouseHover(int e_iX,int e_iY)
{

}

const   WCHAR*	cMainMenuPhase::GetNextPhaseName()
{
	return 0;
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