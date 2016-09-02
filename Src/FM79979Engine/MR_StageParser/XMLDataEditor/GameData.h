#pragma once
#include <string>
#include "StageParser.h"


struct sGameData:public sXmlNode
{
	friend class cGameData;
	//
	struct sMonsterInfo:public sXmlNode
	{
		struct sLevelInfo:public sXmlNode
		{
			GET_INT_DATA_FROM_ATTRIBUTE(HP,L"Health");
			GET_INT_DATA_FROM_ATTRIBUTE(STR,L"Strength");
			GET_INT_DATA_FROM_ATTRIBUTE(Stamina,L"Stamina");
			GET_INT_DATA_FROM_ATTRIBUTE(ID,L"ID");
			GET_INT_DATA_FROM_ATTRIBUTE(Level,L"Level");
			sLevelInfo();
			~sLevelInfo();
		};
		sMonsterInfo();
		~sMonsterInfo();
		cLinerDataProcessor<Vector3>*	m_pLine;
		GET_INT_DATA_FROM_ATTRIBUTE(ID,L"ID");
		std::vector<sLevelInfo>	m_LevelInfoVector;
		std::wstring	GetMonsterName();
		int				GetHitBearCount(int e_iDamage,int e_iLevel);
	};

	struct sMonsterShop:public sXmlNode
	{
		GET_INT_DATA_FROM_ATTRIBUTE(MonsterID,L"monster");
		GET_INT_DATA_FROM_ATTRIBUTE(STR,L"strengthpoint");
		GET_INT_DATA_FROM_ATTRIBUTE(HP,L"lifepoint");
		GET_FLOAT_DATA_FROM_ATTRIBUTE(ExtraEXP,L"exppercent");
		GET_FLOAT_DATA_FROM_ATTRIBUTE(ExtraCoin,L"coinpercent");
		GET_INT_DATA_FROM_ATTRIBUTE(Price,L"price");
		sMonsterShop();
		~sMonsterShop();
	};

	struct sEnemyStatus
	{
		struct sLevelInfo:public sXmlNode
		{
			GET_INT_DATA_FROM_ATTRIBUTE(HP,L"Health");
			GET_INT_DATA_FROM_ATTRIBUTE(STR,L"attack");
			GET_FLOAT_DATA_FROM_ATTRIBUTE(TimeAdd,L"rewardTime");
			GET_FLOAT_DATA_FROM_ATTRIBUTE(TimeMinus,L"reduceTime");
			GET_INT_DATA_FROM_ATTRIBUTE(Speed,L"maxSpeed");
			GET_INT_DATA_FROM_ATTRIBUTE(CloseSpeed,L"closeSpeed");
			GET_FLOAT_DATA_FROM_ATTRIBUTE(SkillTimeReduce,L"recoverMonsterSkillTime");
			GET_INT_DATA_FROM_ATTRIBUTE(Level,L"Level");
			GET_INT_DATA_FROM_ATTRIBUTE(ID,L"ID");
			sLevelInfo();
			~sLevelInfo();
		};
		std::vector<sLevelInfo>		m_LevelInfoVector;
		//
		std::wstring	GetMonsterName();
		int				GetMonsterID();
		int				GetHitBearCount(int e_iDamage,int e_iLevel);
		
	};

	//<node ID="104" level="9" HP="72" attack="21" rewardTime="2" reduceTime="1" maxSpeed="19" closeSpeed="12" prefab="A_Npc_Boar_fly" recoverMonsterSkillTime="0.5"/>

public:
			//std::vector<sXmlNode> ShopDataVector;
	sGameData();
	~sGameData();
	//bind data to lines
	void							Init();
	std::vector<sMonsterInfo>				m_MonsterInfoVector;
	std::map<int,std::vector<sMonsterShop>>	m_MonsterShopDataMap;
	std::vector<sEnemyStatus>				m_EnemyStatusVector;


	sMonsterInfo*		GetMonsterInfo(int e_iID);
	sEnemyStatus*		GetEnemyStatus(int e_iID);
};


class cGameData
{
	sGameData*	m_pGameData;
	//
	std::string m_strFirectory;
	//
	std::string	m_strShopFileName;
	bool		ParseShopFile(const char*e_strFileName);
	//
	std::string	m_strMonsterLevelFileName;
	std::string	m_strMonsterHealthFileName;
	std::string	m_strMonsterStregthFileName;
	std::string	m_strMonsterStaminaFileName;
	bool		ParseMonsterFile(const char*e_strFileName,
		const char*e_strStaminaFileName,
		const char*e_strStregthFileName,
		const char*e_strHealthFileName);
	//
	std::string	m_strNPCInfoFileName;
	bool		ParseNPCInfoFile(const char*e_strFileName);
	//
public:
	cGameData();
	~cGameData();
	void	Parse(const char*e_strDirectory);
	void	Render();
	void	Update(float e_fElpaseTime);
};



//每日任務
//dailychallenge.xml
//
//find a proper stamina let player play over 3 hours
//
//成就
//mission.xml
//same as daily mission
//
//
//stage_ob.xml
//all objects
//
//
//每日簽到
//dailyevent.xml
//要記住總共登入多少天嗎
//５０天１００天１５０天格５０天特別給獎勵?
//
//
//ＭＩＫＡ生及太慢
//一下就被加分關的飛彈打死
//而且第五關開始怪物要打兩下