#include "stdafx.h"
#include "FishShowPaobability.h"

#include "ProbabilityMain.h"
//#include "FishProbabilityInterface.h"
#include "../../FishGameProbability\FishProbabilityInterface.h"

cProbabilityMain::cProbabilityMain()
{

}

cProbabilityMain::~cProbabilityMain()
{

}

void	cProbabilityMain::Init()
{
	
	//std::vector<sGameToProbabilityData> l_vsGameToProbabilityData;
	//l_vsGameToProbabilityData.clear();
	//sGameToProbabilityData l_sGameToProbabilityData;
	//memset( &l_sGameToProbabilityData, -1, sizeof(sGameToProbabilityData) );

	//l_sGameToProbabilityData.BulletType = eBT_COMMON_BULLET;
	//l_sGameToProbabilityData.iBetRate = 100;
	////l_sGameToProbabilityData.iFishIDVector
	//l_sGameToProbabilityData.iPlayerID = 2;
	//l_sGameToProbabilityData.fPower = 100.0f;
	//l_sGameToProbabilityData.iUniqueID = 38;

	//l_vsGameToProbabilityData.push_back(l_sGameToProbabilityData);
	//

	//ProbInit();
	//GiveProbabilityHittedFishesData(l_vsGameToProbabilityData);
}

void	cProbabilityMain::Update( float  e_fElpaseTime )
{
	//ProbUpdate(e_fElpaseTime);
}

std::vector<sFishDiedData>	cProbabilityMain::GetResult()
{
	std::vector<sFishDiedData> l_sFishDiedData;

	return l_sFishDiedData;
}

void	cProbabilityMain::AddToProbabilityHittedFishData(std::vector<sToProbabilityHittedFishData> e_ToProbabilityHittedFishDataVector)
{

}
