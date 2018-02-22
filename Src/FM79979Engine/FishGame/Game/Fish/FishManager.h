#pragma once

#include "FishBase.h"
#include "../Common/SingltonTemplate.h"
class	cFishManager:public cNamedTypedObjectVector<cFishBase>,public cNodeISAX, public cFishGameCommonRegisterManager,public cSingltonTemplate<cFishManager>
{
	virtual void																AddCloneRegisterFunction() override;
	//
	virtual	bool																MyParse(TiXmlElement*e_pRoot) override;
	cFishBase*																	ProcessMonster(TiXmlElement*e_pTiXmlElement,const WCHAR*l_strTypeID);
	void																		ProcessFishBodyTypeAmount(TiXmlElement* e_pTiXmlElement);
	//
	cFishBase**																	m_ppWorkingMonster;
	int																			m_iWorkingMonsterSize;
	std::list< cWaitForFetchFunctionObjectList<cFishBase>* >					m_AllFish;
	cFAIBehaviorParser*															m_pFAIBehaviorParser;
	//std::map<std::wstring, int>													m_FishNameAndIDMap;
public:
	cFishManager();
	~cFishManager();
	void				MonsterLeaveScene();
	void				Init();
	void				Update(float e_fElpaseTime);
	void				Render();
	void				RenderMonsterDiedShow();
	void				DebugRender();
	void				Destroy();
	cFishBase*			FishRequire(int e_iFishIndex,cFAIMachine* e_pAI = NULL);
	//Fish Group must has fish
	cFishBase*			ForceFishRequire(int e_iFishIndex, cFAIMachine* e_pAI = NULL);
	cFishBase*			GetMonsterByID(int e_iID);
	int					GetFishIDByName(const wchar_t* e_pString);
	eFishBodyType		GetFishSizeByID(int e_iFishIdx);
	eFishBodyType		GetFishSizeByName(const wchar_t* e_pString);
	cFAIBehaviorParser*	GetFAIBehaviorParser() { return m_pFAIBehaviorParser; }
};