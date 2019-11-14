#include "stdafx.h"
#include "MainCharacterBehavior.h"

char*MAIN_ROLE_SETUP_DATA = "MagicTower/Battle/MainRole/MainRole.xml";
const wchar_t*g_strMainRoleStatusName[] = {L"Idle",L"Attack",L"AttackFailed",L"Hurt",L"Died"};
//<Battle Pos="710,200,0"/>

cMainRoleData::cMainRoleData(TiXmlElement*e_pTiXmlElement):cCharacterBase(e_pTiXmlElement)
{	
	assert(this->Count() == eMTMRS_MAX&&"animation count is not match");
	m_bBattleMode = false;
	m_eVirtualGamePadDirection = eVGPD_NONE;
	m_vRenderPos = Vector2(0,0);
	m_eBattleAttackType = eBT_ATTATK_MAX;
	m_eBattleDefenceType = eBT_DEFENCE_MAX;

	m_eBattleAttackType = eBT_ATTATK_MOVE_OBJECT;
	m_eBattleDefenceType = eBT_DEFENCE_1;

	//main character setup
	cPuzzleImage*l_pPuzzleImage = cGameApp::GetPuzzleImageByFileName(L"MagicTower/Image/MainCharacter.pi");
	POINT	l_MapPosIndex = {2,1};
	m_pImage[eVGPD_LEFT] = l_pPuzzleImage->GetObject(L"Character_Main_L");
	m_pImage[eVGPD_UP] = l_pPuzzleImage->GetObject(L"Character_Main_Bk");
	m_pImage[eVGPD_RIGHT] = l_pPuzzleImage->GetObject(L"Character_Main_R");
	m_pImage[eVGPD_DOWN] = l_pPuzzleImage->GetObject(L"Character_Main_F");
	m_pImage[eVGPD_NONE] = l_pPuzzleImage->GetObject(L"Character_Main_Idle");
	assert(m_pImage[eVGPD_NONE]);
	assert(m_pImage[eVGPD_UP]);
	assert(m_pImage[eVGPD_LEFT]);
	assert(m_pImage[eVGPD_RIGHT]);
	assert(m_pImage[eVGPD_DOWN]);
	//TiXmlElement*l_pElement = ISAXCallback::GetXmlElementByNameFromElement(L"Battle",e_pTiXmlElement);
	TiXmlElement*l_pElement = GetXmlElementByNameFromElement(L"Battle",e_pTiXmlElement);
	if( l_pElement )
	{
		const WCHAR*l_strPos = l_pElement->Attribute(L"Pos");
		if( l_strPos )
		{
			this->SetWorldPosition(GetVector3(l_strPos));
		}
	}
}

cMainRoleData::~cMainRoleData()
{

}

void		cMainRoleData::SetBattleMode(bool e_bBattleMode)
{
	m_bBattleMode = e_bBattleMode;
	if( m_bBattleMode )
	{
		this->SetCurrentWorkingObject(g_strMainRoleStatusName[eMTMRS_IDLE]);
		assert(this->GetCurrentWorkingObject());
	}
}

void		cMainRoleData::MouseDown(int e_iPosX,int e_iPosY)
{

}

void		cMainRoleData::MouseHover(int e_iPosX,int e_iPosY)
{

}

void		cMainRoleData::MouseUp(int e_iPosX,int e_iPosY)
{

}

void	cMainRoleData::Update(float e_fElpaseTime)
{
	if( !m_bBattleMode )
		m_pImage[m_eVirtualGamePadDirection]->SetPos(m_vRenderPos);
	else
		cFAIAnimationCharacter::Update(e_fElpaseTime);
}

void	cMainRoleData::Render()
{
	if( !m_bBattleMode )
		m_pImage[m_eVirtualGamePadDirection]->Render();
	else
	{
		cFAIAnimationCharacter::Render();
		RenderStatusInfo();
	}
}