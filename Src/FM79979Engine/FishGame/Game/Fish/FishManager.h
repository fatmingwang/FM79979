#pragma once

#include "FishBase.h"

class cFishShowProbability;
//#define SINGLETON_BASIC_FUNCTION(TYPE)TYPE(TYPE const&) = delete;TYPE(TYPE*) = delete;

class	cFishManager:public cNamedTypedObjectVector<cFishBase>,public cNodeISAX, public cFishGameCommonRegisterManager<cFishBase>,public cSingltonTemplate<cFishManager>
{
	//
	virtual	bool																MyParse(TiXmlElement*e_pRoot) override;
	void																		GenerateReservedFishAndRenderOrder();
	//
	cFishShowProbability*														m_pFishShowProbability;
	cTempContainer<cFishBase>													m_WorkingMonster;
	std::vector<cTempContainer<cFishBase>*>										m_RenderWorkingMonsterVector;//sort by render Order,(get local position.z)
	//for quick find index of vector(cNamedTypedObjectVector<cFishBase>) by fish ID
	std::map<int, int>															m_FishIDAndIndexOfVector;
	std::map<int, int>															m_FishIDAndRenderOrderIndex;
	std::vector< std::vector<cFishBase*>* >										m_AllFish;
	cFAIBehaviorParser*															m_pFAIBehaviorParser;
	cFishBase*																	GetWaitForFetchFishFromVector(std::vector<cFishBase*>*e_pFishVector);
	//
	cFishManager();
	virtual ~cFishManager();
public:
	SINGLETON_BASIC_FUNCTION(cFishManager);
	void				MonsterLeaveScene();
	void				Init();
	void				Update(float e_fElpaseTime);
	void				Render();
	void				RenderMonsterDiedShow();
	void				DebugRender();
	void				Destroy();
	cFishBase*			RequireWaitForFetchFish(int e_iFishID);
	//Fish Group must has fish
	cFishBase*			ForceFishRequire(int e_iFishID);
	//a reference to get info
	const cFishBase*	GetFishByID(int e_iID);
	cFAIBehaviorParser*	GetFAIBehaviorParser() { return m_pFAIBehaviorParser; }
};