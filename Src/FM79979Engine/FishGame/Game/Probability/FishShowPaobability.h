#pragma once

#include "../Fish/FishBase.h"
#include "../GameDefine/GameParameterDefine.h"

class cFishShowProbability
{
private:
	int								m_FishBodyTypeAllowToShowCountByPlayerCount[MAX_PLAYER][eFBT_Total];
	sProbabilityWithValue<int, int> m_FishShowProbabilityAndID[eFBT_Total];
	//
protected:
	int								m_FishBodyTypeAllowToShowCountWithCurrentPlayerCountSetup[eFBT_Total];
	//this should in fish manager
	struct sCurrentFishBodyTypeFishCount
	{
		int					iFishCountByFishBodyType[eFBT_Total];
		std::vector<int>	iGenerateFishIDVector;
		sCurrentFishBodyTypeFishCount()
		{
			for (int i = 0; i < eFBT_Total; ++i)iFishCountByFishBodyType[i] = 0;
		}
		~sCurrentFishBodyTypeFishCount(){}
	};
	UT::sTimeCounter				m_GenerateFishTC;//0.5 sceond
	sCurrentFishBodyTypeFishCount	m_CurrentFishBodyTypeFishCount;
public:
	cFishShowProbability(TiXmlElement*e_pTiXmlElement);
	~cFishShowProbability();
	void													Init();
	void													Update(float e_fElpaseTime);
	void													FishDired(eFishBodyType e_eFishBodyType);
	//current fish count by fish body type
	cFishShowProbability::sCurrentFishBodyTypeFishCount*	GetCurrentFishBodyTypeFishCount();
	//
	const int*												GetFishBodyTypeAllowToShowCountWithCurrentPlayerCountSetup();

};