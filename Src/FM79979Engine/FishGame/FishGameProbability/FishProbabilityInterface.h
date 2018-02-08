#pragma once
#include <vector>

#define	FISH_PAY_RATE_FILE_NAME	"Fish/PayRate.xml"

#define PROBABILITY_DLL
#ifdef  PROBABILITY_DLL
	#define PROBABILITY_EXPORT __declspec(dllexport)
#endif
//https://msdn.microsoft.com/en-us/library/a90k134d.aspx
//__declspec(dllexport) void __cdecl Function1(void);
//class __declspec(dllexport) CExampleExport : public CObject
//{
//	... class definition ...
//};

enum eBulletType
{
	eBT_COMMON_BULLET = 0,
	eBT_MINI_GAME_TYPE1,
	eBT_MINI_GAME_TYPE2,
	eBT_MINI_GAME_TYPE3,
	eBT_MINI_GAME_TYPE4,
	eBT_MAX
};


struct sGameToProbabilityData
{
	int iUniqueID;
	eBulletType BulletType;
	int iBetRate;
	int iPlayerID;
	std::vector<int> iFishIDVector;
};

struct sProbabilityToGameData
{
	struct sPrizeReturnType
	{
		int iAddConstScoreTime;
		int iScoreDoubleTime;
	};
	struct sFishPayRate
	{
		int iFishID;
		int iPayRate;
	};
	int iUniqueID;
	std::vector<sFishPayRate> FishPayRateVector;
	int iGetMiniGameID;
	sPrizeReturnType PrizeReturnTypeData;
};

PROBABILITY_EXPORT	void									GiveProbabilityHittedFishesData(std::vector<sGameToProbabilityData> e_GameToProbabilityDataVector);
PROBABILITY_EXPORT	std::vector<sProbabilityToGameData>		GetHittedFishesResult();