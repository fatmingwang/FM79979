#pragma once
#include <vector>
class cBullet;
class cPlayer;
class cMonster;

struct sToProbabilityHittedFishData
{
	int iToProbabilityID;
	int iFishID;
	int iBetRate;
	int iPlayerID;
};

struct sFishDiedData
{
	int iToProbabilityID;
	int iFishID;
	int iPayRate;//0 fish not die or already dead.
	int	iGivePrizeReturnType;
	union PrizeReturnType
	{
		struct sPrizeDouble
		{
			int iAddConstScoreTime;
			int iScoreDoubleTime;
		};
		sPrizeDouble	PrizeDouble;
		int	iMiniGameID;
	};
	PrizeReturnType PrizeReturnTypeData;
};

struct sHittedFishData
{
	int iToProbabilityID;//once probability return result compare with this to check is this same
	cBullet*pBullet;
	cPlayer*pPlayer;
	cMonster*pMonster;
	sHittedFishData()
	{
		pBullet = nullptr;
		pPlayer = nullptr;
		pMonster = nullptr;
		static int l_siSN = 0;
		iToProbabilityID = l_siSN;
		++l_siSN;
	}
	~sHittedFishData(){}
	sToProbabilityHittedFishData GetToProbabilityHittedFishData();
};


//
class cProbability
{
	bool											m_bLeaveThread;
	bool											m_bThreadFinish;
	void											CreateThread();
	std::vector<sToProbabilityHittedFishData>		m_WaitForProcessToProbabilityHittedFishData;//the data from game play
	std::vector<sFishDiedData>						m_WaitForFetchFishDiedData;//the data wait for fetch
public:
	//
	cProbability();
	~cProbability();
	void											Init();
	void											Update( float e_fElpaseTime );
	std::vector<sFishDiedData>						GetResult();
	void											AddToProbabilityHittedFishData(std::vector<sToProbabilityHittedFishData> e_ToProbabilityHittedFishDataVector);
};