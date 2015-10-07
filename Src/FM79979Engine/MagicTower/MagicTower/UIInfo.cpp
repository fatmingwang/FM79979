#include "stdafx.h"
#include "UIInfo.h"

WCHAR*	UI_INFO_MPDILIST_FILE_NAME = L"MagicTower/Image/UIInfo.mpdi";

cUIInfo::cUIInfo()
{
	m_pUIInfoMPDI = 0;
	cMPDIList*l_pMPDILIst = cGameApp::GetMPDIListByFileName(UI_INFO_MPDILIST_FILE_NAME);
	m_pUIInfoMPDI = l_pMPDILIst->GetObject(L"UIInfo");
}

cUIInfo::~cUIInfo()
{
	//SAFE_DELETE(m_pUIInfoMPDI);
}

void	cUIInfo::Update(float e_fElpaseTime)
{
	if(m_pUIInfoMPDI)
		m_pUIInfoMPDI->Update(e_fElpaseTime);
}

void	cUIInfo::Render()
{
	if(m_pUIInfoMPDI)
		m_pUIInfoMPDI->Render();
}