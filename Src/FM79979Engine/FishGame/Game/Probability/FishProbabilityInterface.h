#pragma once
#include <vector>;

#define	FISH_PAY_RATE_FILE_NAME	"Fish/PayRate.xml"

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


class cProbabilityInterface:public NamedTypedObject
{
	cProbabilityInterface();
	virtual ~cProbabilityInterface();
public:
	static cProbabilityInterface * GetInstance();
	virtual void									GiveProbabilityHittedFishesData(std::vector<sGameToProbabilityData> e_GameToProbabilityDataVector) = 0;
	virtual std::vector<sProbabilityToGameData>		GetHittedFishesResult() = 0;
};