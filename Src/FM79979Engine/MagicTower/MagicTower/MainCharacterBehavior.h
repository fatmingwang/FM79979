#ifndef _MAGIC_TOWER_MAIN_CHARACTER_BEHAVIOR_H
#define _MAGIC_TOWER_MAIN_CHARACTER_BEHAVIOR_H

#include "LevelData.h"
#include "VirtualGamePad.h"
#include "BattlAttackBase.h"
#include "CharacterBase.h"

enum	eMagicTowerMainRoleStatus
{
	eMTMRS_IDLE = 0,
	eMTMRS_ATTACK,
	eMTMRS_ATTACK_FAILED,
	eMTMRS_HURT,
	eMTMRS_DIED,
	eMTMRS_MAX
};

extern WCHAR*g_strMainRoleStatusName[];


class	cMainRoleData:public cCharacterBase
{
	GET_SET_DEC(eBattleType,m_eBattleAttackType,GetBattleAttackType,SetBattleAttackType);
	GET_SET_DEC(eBattleType,m_eBattleDefenceType,GetBattleDefenceType,SetBattleDefenceType);
	//
	GET_SET_DEC(eVirtualGamePadDirection,m_eVirtualGamePadDirection,GetDirection,SetDirection);
	GET_SET_DEC(Vector2,m_vRenderPos,GetRenderPos,SetRenderPos);
	GET_SET_DEC(POINT,m_MapPosIndex,GetMapPosIndex,SetMapPosIndex);
	bool			m_bBattleMode;
public:
	cMainRoleData(TiXmlElement*e_pTiXmlElement);
	~cMainRoleData();
	cBaseImage* m_pImage[eVGPD_NONE];
	bool			IsBattleMode(){return m_bBattleMode;}
	void			SetBattleMode(bool e_bBattleMode);
	int	m_iCurrentExp;
	int	m_iAllLevelExpNeed[99];//aftern 99 each level need as many as 99's.
	int	m_iCurrentLV;
	cItemData	*m_psWeapon;
	cItemData	*m_psShield;
	cItemData	*m_psDecoration;
	void		MouseDown(int e_iPosX,int e_iPosY);
	void		MouseHover(int e_iPosX,int e_iPosY);
	void		MouseUp(int e_iPosX,int e_iPosY);
	virtual	void	Update(float e_fElpase);
	virtual	void	Render();
	virtual	void	RenderAttack(){}
};

#endif