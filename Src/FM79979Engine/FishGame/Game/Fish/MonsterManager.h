#pragma once

#include "MonsterBase.h"

class	cMonsterManager:public cNamedTypedObjectVector<cMonsterBase>,public cNodeISAX, public cFishGameCommonRegisterManager
{
	virtual void																AddCloneRegisterFunction() override;
	//
	virtual	bool																MyParse(TiXmlElement*e_pRoot) override;
	cMonsterBase*																ProcessMonster(TiXmlElement*e_pTiXmlElement,const WCHAR*l_strTypeID);
	void																		ProcessFishBodyTypeAmount(TiXmlElement* e_pTiXmlElement);
	//
	cMonsterBase**																m_ppWorkingMonster;
	int																			m_iWorkingMonsterSize;
	std::list< cWaitForFetchFunctionObjectList<cMonsterBase>* >	m_AllFish;
//	cFAIBehaviorParser*															m_pFAIBehaviorParser;
public:
	cMonsterManager();
	~cMonsterManager();
	void			MonsterLeaveScene();
	void			Init();
	void			Update(float e_fElpaseTime);
	void			Render();
	void			RenderMonsterDiedShow();
	void			DebugRender();
	void			Destroy();
	cMonsterBase*	FishRequire(int e_iFishIndex,cFAIMachine* e_pAI = NULL);
	cMonsterBase*	GetMonsterByID(int e_iID);
	int				GetFishIDByName(const wchar_t* e_pString);
	eBodyType		GetFishSizeByID(int e_iFishIdx);
	eBodyType		GetFishSizeByName(const wchar_t* e_pString);
};