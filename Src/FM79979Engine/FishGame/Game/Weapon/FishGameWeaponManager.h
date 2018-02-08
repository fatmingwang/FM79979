#pragma once

#include "FishGameWeaponBase.h"

class cWeaponManager:public cRenderObject,public cFishGameCommonRegisterManager,public cNodeISAX
{
	virtual	bool									MyParse(TiXmlElement*e_pRoot)override;
	virtual void									AddCloneRegisterFunction()override;
	//
	cNamedTypedObjectVector<cFishGameWeaponBase>	m_CommonWeaponVector;
	cNamedTypedObjectVector<cFishGameWeaponBase>	m_MiniGameWeaponVector;
	cFishGameWeaponBase*							m_pCurrentWeapon;
	cFishGameWeaponBase*							m_pLastSelectedCommonWeapon;//from mini game to common weapon
	std::list<eWeaponType>							m_WaitToPlayMiniGamWeapon;
	//
	void											SwitchWeapon();
	//
	bool											DeviceNotExistAssignPlayerIndexByKey0123456789KeyUpdate();
	bool											DeviceNotExistAssignPlayerIndexByKey0123456789KeyUpdateAfter();
	unsigned char									m_ucKey[eFPK_MAX];
	void											SetupKey();
	//to check how many bullet remain on the scene.
	GET_SET_DEC(int, m_iShootNumBuleet, GetShootNumBuleet, SetShootNumBuleet);
public:
	cWeaponManager();
	virtual ~cWeaponManager();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
};