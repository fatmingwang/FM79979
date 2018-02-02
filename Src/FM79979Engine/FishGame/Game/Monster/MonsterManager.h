#pragma once
//
#include "Monster.h"
#include "HittedFishData.h"
//


class	cMonsterManager:public cNamedTypedObjectVector<cMonster>,public cNodeISAX
{
	virtual	bool													MyParse(TiXmlElement*e_pRoot) override;
	cMonster*														ProcessMonster(TiXmlElement*e_pTiXmlElement,const WCHAR*l_strTypeID);
	void															ProcessFishBodyTypeAmount(TiXmlElement* e_pTiXmlElement);
	//
	cMonster**														m_ppWorkingMonster;
	int																m_iWorkingMonsterSize;
	cNamedTypedObjectVector< cNamedTypedObjectVector<cMonster> >	m_AllFish;
	cFAIBehaviorParser*												m_pFAIBehaviorParser;
public:
	cMonsterManager();
	~cMonsterManager();
	void			MonsterLeaveScene();
	void			Init();
	void			Update(float e_fElpaseTime);
	void			Render();
	void			MonsterDiedRender();
	void			DebugRender();
	void			Destroy();
	cMonster*		FishRequire (int e_iFishIndex, cFAIMachine* e_pAI = NULL);
	cMonster*		GetMonsterByID(int e_iID);
	int				GetFishIDByName (const WCHAR* e_pString);
	eBodyType		GetFishSizeByID (int e_iFishIdx);
	eBodyType		GetFishSizeByName (const WCHAR* e_pString);
};