#pragma once
#include <vector>

#define	FISH_PAY_RATE_FILE_NAME		"Fish/PayRate.xml"
#define	FISH_PROBABILITY_FILE_NAME	"Fish/Probability.xml"

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
#define MAX_PLAYER	10

enum eBulletType
{
	eBT_COMMON_BULLET = 0,
	eBT_MINI_GAME_TYPE1,//DeepSeaMusicBox
	eBT_MINI_GAME_TYPE2,//NeptuneFork
	eBT_MINI_GAME_TYPE3,//KillOrder
	eBT_MINI_GAME_TYPE4,//SprayFlame
	eBT_MAX
};

enum eFishHitResult
{
	eFHR_Dead,
	eFHR_Alive,
	eFHR_TransformStatus,//Weapon transform use
	eFHR_MAX
};

enum eProbFieldType//(�@40��ICON)
{
	ePFT_Small,//�p��(15)
	ePFT_Middle,//����(8)
	ePFT_Big,//�j��(6)
	ePFT_Giant,//������(4)
	ePFT_SuperGiant,//�W����(2)ColorfulTreasure�C�m�_�c+�C�m���s
	ePFT_SpecialWeapon,//�|�دS��Z��+�w�ߪw�w��(4+1)
	ePFT_MAX
};

struct sProbInit
{
	int		iProbLevel;
	float	fDrawRate;
	int		iCoinRate;
	int		iMaxBet;
	int		iMinBet;
	float	RevenueAmplitude;

	bool	bPromotion;
	//FieldType?

};

struct sGameToProbabilityData
{	
	int iUniqueID;
	int iFishID;
	int iPlayerID;
	int iBet;
	int iCoinRate;
	eBulletType			BulletType;
	float fPrize;
	float fExtraOdds;
	float fPower;
	
	float fExtraProb;//for Kill Mode.

	int iBetRate;
	std::vector<int> iFishIDVector;
	
	eFishHitResult	FishHitResult;
	//eProbFieldType		FishType;
};

struct sProbabilityToGameData
{
	struct sPrizeReturnType
	{
		int iAddConstScoreTime;//
		int iScoreDoubleTime;//
	};
	struct sFishPayRate
	{
		int		iFishID;
		float	fPrize;
		float	fExtraOdds;
	};
	int		iUniqueID;//NameTypeObject Id ,GetUniqueID()
	int		iPlayerID;
	int		iBet;
	int		iCoinRate;
	

	eBulletType		BulletType;
	eFishHitResult	FishHitResult;
	std::vector<sFishPayRate> FishPayRateVector;//??

	int iGetMiniGameID;
	float fPower;
	sPrizeReturnType PrizeReturnTypeData;
};

PROBABILITY_EXPORT	void										ProbInit();

PROBABILITY_EXPORT	void										GiveProbabilityHittedFishesData(std::vector<sGameToProbabilityData> e_GameToProbabilityDataVector);
PROBABILITY_EXPORT	std::vector<sProbabilityToGameData>			GetHittedFishesResult();


PROBABILITY_EXPORT	void										ProbDestroy();