#pragma once
#include <string>
#include "StageParser.h"
#include "ChartWithName.h"


class cSimpleFunctionPointerHelp
{
	bool					m_bIntFunction;
	std::function<int()>	m_IntFunction;
	std::function<float()>	m_FloatFunction;
public:
	cSimpleFunctionPointerHelp()
	{
	}
	cSimpleFunctionPointerHelp(std::function<int()> e_Function)
	{
		if( e_Function )
			SetFunction(e_Function);
	}
	cSimpleFunctionPointerHelp(std::function<float()> e_Function)
	{
		if( e_Function )
			SetFunction(e_Function);
	}
	void SetFunction(std::function<int()> e_Function)
	{
		m_IntFunction = e_Function;
		m_bIntFunction = true; 
	}
	void SetFunction(std::function<float()> e_Function)
	{
		m_FloatFunction = e_Function ;
		m_bIntFunction = false;	
	}
	~cSimpleFunctionPointerHelp(){}

	float					GetValue()
	{
		if( m_bIntFunction )
			return (float)m_IntFunction();
		return m_FloatFunction();
	}
};


//struct sGetFunctionVector:public std::vector<cSimpleFunctionPointerHelp*>
//{
//	sGetFunctionVector(){}
//	~sGetFunctionVector(){ DELETE_VECTOR((*this),cSimpleFunctionPointerHelp*); }
//};

struct sGameData:public sXmlNode
{
	friend class cGameData;
	//
	struct sMonsterInfo:public sXmlNode,public cChartWithName
	{
		enum eMonsterStatus
		{
			eMS_HP = 0,
			eMS_STR,
			eMS_STAMINIA,
			eMS_EXP,
			eMS_COIN_COST,
			eMS_TIME_COST,
			eMS_CRYSTAL_COST,
			eES_SKILL_CD,
			eMS_MAX,
		};
		cChartWithName								m_AllRenderData;
		//std::vector<std::wstring>					m_AllRenderData;
		void							SetRenderData();
		struct sLevelInfo:public cChartWithName::sNameAndData
		{
			GET_INT_DATA_FROM_ATTRIBUTE(HP,L"Health");
			GET_INT_DATA_FROM_ATTRIBUTE(STR,L"Strength");
			GET_INT_DATA_FROM_ATTRIBUTE(Stamina,L"Stamina");
			GET_INT_DATA_FROM_ATTRIBUTE(Exp,L"exp");
			GET_INT_DATA_FROM_ATTRIBUTE(CoinCost,L"coinCost");
			GET_INT_DATA_FROM_ATTRIBUTE(TimeCost,L"timeCost");
			GET_INT_DATA_FROM_ATTRIBUTE(CrystalCost,L"crystalCost");
			GET_INT_DATA_FROM_ATTRIBUTE(SkillCD,L"SkillCD");


			GET_INT_DATA_FROM_ATTRIBUTE(ID,L"ID");
			GET_INT_DATA_FROM_ATTRIBUTE(Level,L"level");
			sLevelInfo();
			~sLevelInfo();
		};
		GET_INT_DATA_FROM_ATTRIBUTE(ID,L"ID");
		sMonsterInfo();
		//sMonsterInfo(const sMonsterInfo&e_MonsterInfo);
		~sMonsterInfo();
		//std::vector<sLevelInfo>	m_LevelInfoVector;
		std::wstring	GetMonsterName();
		int				GetHitBearCount(int e_iDamage,int e_iLevel);
	};

	struct sMonsterShop:public sXmlNode,public cChartWithName
	{
		std::map<std::wstring,cSimpleFunctionPointerHelp*>	m_NameAndValueMap;
		GET_INT_DATA_FROM_ATTRIBUTE(MonsterID,L"monster");
		GET_INT_DATA_FROM_ATTRIBUTE(STR,L"strengthpoint");
		GET_INT_DATA_FROM_ATTRIBUTE(HP,L"lifepoint");
		GET_FLOAT_DATA_FROM_ATTRIBUTE(ExtraEXP,L"exppercent");
		GET_FLOAT_DATA_FROM_ATTRIBUTE(ExtraCoin,L"coinpercent");
		GET_INT_DATA_FROM_ATTRIBUTE(Price,L"price");
		GET_INT_DATA_FROM_ATTRIBUTE(PriceType,L"pricetype");
		std::map<std::wstring,float>				GetAttributeNameAndValue();
		void		Init();
		sMonsterShop(const sMonsterShop&e_MonsterShop);
		sMonsterShop();
		~sMonsterShop();
	};

	struct sEnemyStatus:public cChartWithName
	{
		enum eEnemyStatus
		{
			eES_HP = 0,
			eES_STR,
			eES_TIME_ADD,
			eES_TIME_MINUS,
			eES_SPEED,
			eES_CLOSE_SPEED,
			eES_SKILL_TIME_REDUCE,
			eES_MAX,
		};
		//cChartWithName::sNameAndData*	m_pHPData;
		//cChartWithName::sNameAndData*	m_pSTRData;
		//cChartWithName::sNameAndData*	m_pTimeAdd;
		//cChartWithName::sNameAndData*	m_pTimeMinus;
		//cChartWithName::sNameAndData*	m_pSpeed;
		//cChartWithName::sNameAndData*	m_pCloseSpeed;
		//cChartWithName::sNameAndData*	m_pSkillTimeReduce;
		cChartWithName					m_AllRenderData;
		struct sLevelInfo:public cChartWithName::sNameAndData
		{
			GET_INT_DATA_FROM_ATTRIBUTE(HP,L"HP");
			GET_INT_DATA_FROM_ATTRIBUTE(STR,L"attack");
			GET_FLOAT_DATA_FROM_ATTRIBUTE(TimeAdd,L"rewardTime");
			GET_FLOAT_DATA_FROM_ATTRIBUTE(TimeMinus,L"reduceTime");
			GET_INT_DATA_FROM_ATTRIBUTE(Speed,L"maxSpeed");
			GET_INT_DATA_FROM_ATTRIBUTE(CloseSpeed,L"closeSpeed");
			GET_FLOAT_DATA_FROM_ATTRIBUTE(SkillTimeReduce,L"recoverMonsterSkillTime");
			GET_INT_DATA_FROM_ATTRIBUTE(Level,L"Level");
			GET_INT_DATA_FROM_ATTRIBUTE(ID,L"ID");
			sLevelInfo();
			sLevelInfo(const sLevelInfo& e_LevelInfo);
			~sLevelInfo();
		};
		sEnemyStatus();
		//sEnemyStatus(const sEnemyStatus& e_EnemyStatus);
		~sEnemyStatus();
		//std::vector<sLevelInfo>		m_LevelInfoVector;
		//
		std::wstring	GetMonsterName();
		int				GetMonsterID();
		int				GetHitBearCount(int e_iDamage,int e_iLevel);
		
	};
	void									EnemyDataInit();
	void									MonsterDataInit();
	void									ShopDataInit();
	//bind data to lines
	void									Init();
	float									m_fShopStartPosX;
	float									m_fShopStartPosY;
	float									m_fShopGapX;
	float									m_fMonsterInfoStartPosX;
	float									m_fMonsterInfoStartPosY;
	float									m_fMonsterInfoGapX;
	float									m_fMonsterStatusGapX;
	float									m_fEnemyStartPosX;
	float									m_fEnemyStartPosY;
	float									m_fEnemyGapX;
	float									m_fEnemyStatusGapX;
	//
	Vector2									m_vMonsterRect;
	float									m_fMonsterScale;
	float									m_fShopScale;
	float									m_fEnemyScale;
	//

	std::vector<std::function<void()>>		m_RenderFunctionPointerVector;
	std::vector<std::function<void(float)>>	m_UpdateFunctionPointerVector;
	void									MonsterRender();
	void									MonsterUpdate(float e_fElpaseTime);
	void									ShopRender();
	void									ShopUpdate(float e_fElpaseTime);
	void									EnemyRender();
	void									EnemyUpdate(float e_fElpaseTime);
	//<node ID="104" level="9" HP="72" attack="21" rewardTime="2" reduceTime="1" maxSpeed="19" closeSpeed="12" prefab="A_Npc_Boar_fly" recoverMonsterSkillTime="0.5"/>
	struct sRenderFlagControl
	{
		bool						m_bShowValue;
		int							m_iRenderFlag;
		std::vector<int>			m_RenderIndexVector;
		std::vector<Vector2>		m_MaxVector;
		sRenderFlagControl(){m_iRenderFlag = 0;m_bShowValue = false;}
		void	Init(int e_iNumStatus){ m_iRenderFlag = 0;m_RenderIndexVector.clear();m_MaxVector.clear(); for(int i=0;i<e_iNumStatus;++i)m_MaxVector.push_back(Vector2(0,0));m_bShowValue = false; }
	};
public:
			//std::vector<sXmlNode> ShopDataVector;
	sGameData();
	~sGameData();
	//
	sRenderFlagControl							m_MonsterRenderFlagControl;
	std::vector<sMonsterInfo*>					m_MonsterInfoVector;
	//monster ID.
	sRenderFlagControl							m_ShopRenderFlagControl;
	std::map<int,std::vector<sMonsterShop*>*>	m_MonsterShopDataMap;
	//
	std::vector<sEnemyStatus*>					m_EnemyStatusVector;
	sRenderFlagControl							m_EnemyRenderFlagControl;


	sMonsterInfo*		GetMonsterInfo(int e_iID);
	sEnemyStatus*		GetEnemyStatus(int e_iID);
	std::wstring		GetMonsterName(int e_iID);
	//1<11 monster,1<<2enemy,1<<3 shop
	void									Render(int e_iTargetFlag);
	void									Update(int e_iTargetFlag,float e_fElpaseTime);
};


class cGameData
{
	//
	std::string m_strFirectory;
	//
	std::string	m_strShopFileName;
	bool		ParseShopFile(const char*e_strFileName);
	//
	std::string	m_strMonsterFileName;
	std::string	m_strMonsterLevelFileName;
	std::string	m_strMonsterHealthFileName;
	std::string	m_strMonsterStregthFileName;
	std::string	m_strMonsterStaminaFileName;
	bool		ParseMonsterFile(const char*e_strMonsterFileName,
		const char*e_strMonsterLevelFileName,
		const char*e_strStaminaFileName,
		const char*e_strStregthFileName,
		const char*e_strHealthFileName);
	//
	std::string	m_strNPCInfoFileName;
	bool		ParseNPCInfoFile(const char*e_strFileName);
	//
public:
	sGameData*	m_pGameData;
	cGameData();
	~cGameData();
	bool	Parse(const char*e_strDirectory);
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