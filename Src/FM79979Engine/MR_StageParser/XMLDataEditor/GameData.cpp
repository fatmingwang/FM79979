#include "stdafx.h"
#include "GameData.h"


sGameData::sMonsterInfo::sMonsterInfo()
{

}

sGameData::sMonsterInfo::~sMonsterInfo()
{

}


sGameData::sMonsterShop::sMonsterShop()
{
	this->m_fExtraCoin = 0.f;
	this->m_fExtraEXP = 0.f;
	this->m_iHP = -1;
	this->m_iMonsterID = -1;
	this->m_iPrice = -1;
	this->m_iSTR = -1;
}
sGameData::sMonsterShop::~sMonsterShop()
{

}

sGameData::sMonsterInfo::sLevelInfo::sLevelInfo()
{
	this->m_iHP = -1;
	this->m_iSTR = -1;
	this->m_iStamina = -1;
	this->m_iID = -1;
	this->m_iLevel = -1;
}


sGameData::sMonsterInfo::sLevelInfo::~sLevelInfo()
{

}

sGameData::sEnemyStatus::sLevelInfo::sLevelInfo()
{
	this->m_iHP = -1;
	this->m_iSTR = -1;
	this->m_fTimeAdd = -1.f;
	this->m_fTimeMinus = -1.f;
	this->m_iSpeed = -1;
	this->m_iCloseSpeed = -1;
	this->m_fSkillTimeReduce = -1.f;
	this->m_iLevel = -1;
	this->m_iID = -1;
}

sGameData::sEnemyStatus::sLevelInfo::~sLevelInfo()
{

}


sGameData::sGameData()
{
	m_pHPLine = new cLinerDataProcessor<Vector3>();
	m_pSTRLine = new cLinerDataProcessor<Vector3>();
	m_pStaminaLine = new cLinerDataProcessor<Vector3>();
}

sGameData::~sGameData()
{
	SAFE_DELETE(m_pHPLine);
	SAFE_DELETE(m_pSTRLine);
	SAFE_DELETE(m_pStaminaLine);
}


sGameData::sEnemyStatus*		sGameData::GetEnemyStatus(int e_iID)
{
	size_t l_uiSize = 0;this->m_EnemyStatusVector.size();
	for(size_t i=0;i<l_uiSize;++i)
	{
		if(e_iID == m_EnemyStatusVector[i].GetID())
		{
			return &m_EnemyStatusVector[i];
		}
	}
	return nullptr;
}

sGameData::sMonsterInfo*	sGameData::GetMonsterInfo(int e_iID)
{
	size_t l_uiSize = 0;this->m_MonsterInfoVector.size();
	for(size_t i=0;i<l_uiSize;++i)
	{
		if(e_iID == m_MonsterInfoVector[i].GetID())
		{
			return &m_MonsterInfoVector[i];
		}
	}
	return nullptr;
}

std::wstring	sGameData::sMonsterInfo::GetMonsterName()
{
	if(this->m_LevelInfoVector.size())
	{
		std::wstring l_strName = this->m_LevelInfoVector[0].getValue(L"name_CN");
		l_strName += L",";
		l_strName += ValueToStringW(this->m_LevelInfoVector[0].getValue(L"name"));
		return l_strName;
	}
	return L"";
}

int		sGameData::sMonsterInfo::GetHitBearCount(int e_iDamage,int e_iLevel)
{
	if(e_iLevel >= (int)m_LevelInfoVector.size())
	{
		return 0;
	}
	auto*l_pLevelInfo = &m_LevelInfoVector[e_iLevel];
	int	l_iHP = l_pLevelInfo->GetHP();
	if( l_iHP > 0 )
	{
		float l_fBearCount = (float)l_iHP/e_iDamage;
		return (int)l_fBearCount;
	}
	return 0;
	//l_pLevelInfo->
}

int	sGameData::sEnemyStatus::GetMonsterID()
{
	if(this->m_LevelInfoVector.size())
	{
		return this->m_LevelInfoVector[0].GetID();
	}
	return -1;
}

std::wstring	sGameData::sEnemyStatus::GetMonsterName()
{
	if(this->m_LevelInfoVector.size())
	{
		std::wstring l_strName = this->m_LevelInfoVector[0].getValue(L"description");
		l_strName += L",";
		l_strName += ValueToStringW(this->m_LevelInfoVector[0].GetID());
	}
	return L"";	
}

int				sGameData::sEnemyStatus::GetHitBearCount(int e_iDamage,int e_iLevel)
{
	if(e_iLevel >= (int)m_LevelInfoVector.size())
	{
		return 0;
	}
	auto*l_pLevelInfo = &m_LevelInfoVector[e_iLevel];
	int	l_iHP = l_pLevelInfo->GetHP();
	if( l_iHP > 0 )
	{
		float l_fBearCount = (float)l_iHP/e_iDamage;
		return (int)l_fBearCount;
	}
	return 0;
}

void	sGameData::Init()
{
	m_pHPLine->Clear();
	m_pSTRLine->Clear();
	m_pStaminaLine->Clear();
	for(size_t i=0;m_MonsterInfoVector.size();++i)
	{
//		sMonsterInfo*l_pLevelInfo = &m_MonsterInfoVector[i];
	//	m_pHPLine->AddData(Vector3(0.f,(float)l_pLevelInfo->GetHP(),0.f),(float)i);
		//m_pStaminaLine->AddData(Vector3(0.f,(float)l_pLevelInfo->GetStamina(),0),(float)i);
		//m_pSTRLine->AddData(Vector3(0.f,(float)l_pLevelInfo->GetSTR(),0),(float)i);
	}
}

//struct sMonsterInfo:public sXmlNode
//{
//	
//}

cGameData::cGameData()
{
	m_strFirectory = "./";
	m_strShopFileName = "shop.xml";
	m_strNPCInfoFileName = "npcinfo.xml";
	m_strMonsterLevelFileName = "monsterlevel.xml";
	m_strMonsterHealthFileName = "health.xml";
	m_strMonsterStregthFileName = "stregth.xml";
	m_strMonsterStaminaFileName = "stamina.xml";
	m_pGameData = new sGameData();
}

cGameData::~cGameData()
{
	SAFE_DELETE(m_pGameData);
}

void	cGameData::Parse(const char*e_strDirectory)
{
	SAFE_DELETE(m_pGameData);
	m_pGameData = new sGameData();
	m_strFirectory = e_strDirectory;
	std::string l_strShopFileName = m_strFirectory+l_strShopFileName;
	std::string l_strNPCInfoFileName = m_strFirectory+l_strNPCInfoFileName;
	std::string l_strMonsterLevelFileName = m_strFirectory+l_strMonsterLevelFileName;
	std::string l_strMonsterHealthFileName = m_strFirectory+l_strMonsterHealthFileName;
	std::string l_strMonsterStregthFileName = m_strFirectory+l_strMonsterStregthFileName;
	std::string l_strMonsterStaminaFileName = m_strFirectory+l_strMonsterStaminaFileName;
	ParseShopFile(l_strShopFileName.c_str());
	ParseMonsterFile(l_strMonsterLevelFileName.c_str(),
		l_strMonsterStaminaFileName.c_str(),
		l_strMonsterStregthFileName.c_str(),
		m_strMonsterHealthFileName.c_str());
	ParseNPCInfoFile(l_strShopFileName.c_str());
	m_pGameData->Init();
}

bool	cGameData::ParseShopFile(const char*e_strFileName)
{
	cNodeISAX l_NodeISAX;
	if(!l_NodeISAX.ParseDataIntoXMLNode(e_strFileName))
		return false;
	auto l_Element = l_NodeISAX.GetRootElement();
	l_Element = l_Element->FirstChildElement();
	while(l_Element)
	{
		sGameData::sMonsterShop l_ShopXmlNode;
		l_ShopXmlNode.StroeAllAttribute(l_Element);
		int	l_iID = l_ShopXmlNode.GetMonsterID();
		std::vector<sGameData::sMonsterShop>*l_pMonsterShopVector = nullptr;
		if(!IsMapContainKey<int,std::vector<sGameData::sMonsterShop>>(&m_pGameData->m_MonsterShopDataMap,l_iID))
		{
			std::vector<sGameData::sMonsterShop> l_MonsterShopVector;
			m_pGameData->m_MonsterShopDataMap[l_iID] = l_MonsterShopVector;
		}
		l_pMonsterShopVector = &m_pGameData->m_MonsterShopDataMap[l_iID];
		l_pMonsterShopVector->push_back(l_ShopXmlNode);
		l_Element = l_Element->NextSiblingElement();
		
	}
	return true;
}

bool	cGameData::ParseMonsterFile(const char*e_strFileName,
		const char*e_strStaminaFileName,
		const char*e_strStregthFileName,
		const char*e_strHealthFileName)
{
	cNodeISAX l_NodeISAX1;
	cNodeISAX l_StaminaNodeISAX1;
	cNodeISAX l_StregthNodeISAX1;
	cNodeISAX l_HealthNodeISAX1;
	if(!l_NodeISAX1.ParseDataIntoXMLNode(e_strFileName)||
		!l_StaminaNodeISAX1.ParseDataIntoXMLNode(e_strStaminaFileName)||
		!l_StregthNodeISAX1.ParseDataIntoXMLNode(e_strStregthFileName)||
		!l_HealthNodeISAX1.ParseDataIntoXMLNode(e_strHealthFileName))
		return false;
	const int l_iArraySize = 3;
	cNodeISAX*l_pNodeISAXArray[l_iArraySize] = {&l_StaminaNodeISAX1,&l_StregthNodeISAX1,&l_HealthNodeISAX1};
	auto l_Element = l_NodeISAX1.GetRootElement();
	l_Element = l_Element->FirstChildElement();
	while(l_Element)
	{
		sGameData::sMonsterInfo l_MonsterLevelInfo;
		l_MonsterLevelInfo.StroeAllAttribute(l_Element);
		this->m_pGameData->m_MonsterInfoVector.push_back(l_MonsterLevelInfo);
		l_Element = l_Element->NextSiblingElement();
	}
	sGameData::sMonsterInfo*l_pMonsterInfo = nullptr;
	for( int i=0;i<l_iArraySize;++i )
	{
		auto l_Element = l_pNodeISAXArray[i]->GetRootElement();
		l_Element = l_Element->FirstChildElement();
		//avoid Level is not from 0 to  15
		int l_iStartIndex = -1;
		while(l_Element)
		{
			sGameData::sMonsterInfo::sLevelInfo l_LevelInfo;
			l_LevelInfo.StroeAllAttribute(l_Element);
			int l_iLevel = l_LevelInfo.GetLevel()-1;
			if( l_iStartIndex < l_iLevel)
				l_iStartIndex = l_iLevel;
			else
			{
				UT::ErrorMsg(L"Level should from 1 to 15", ValueToStringW(l_iLevel) );
			}
			if( !l_pMonsterInfo || l_pMonsterInfo->GetID() != l_LevelInfo.GetID())
			{
				l_pMonsterInfo = this->m_pGameData->GetMonsterInfo(l_LevelInfo.GetID());
			}
			if( i == 0 )
			{
				l_pMonsterInfo->m_LevelInfoVector.push_back(l_LevelInfo);
			}
			else
			{
				if( l_iLevel < (int)l_pMonsterInfo->m_LevelInfoVector.size() )
				{
					auto l_pLevelInfo = &l_pMonsterInfo->m_LevelInfoVector[l_iLevel];
					l_pLevelInfo->Merge(l_Element);
				}
				else
				{
					UT::ErrorMsg(ValueToStringW(l_iLevel),L"Level out of range");
				}
			}
			
			l_Element = l_Element->NextSiblingElement();
		}
	}
	return true;
}

bool	cGameData::ParseNPCInfoFile(const char*e_strFileName)
{
	cNodeISAX l_NodeISAX;
	if(!l_NodeISAX.ParseDataIntoXMLNode(e_strFileName))
		return false;
	auto l_Element = l_NodeISAX.GetRootElement();
	l_Element = l_Element->FirstChildElement();
	sGameData::sEnemyStatus*l_pEnemyStatus = nullptr;
	while(l_Element)
	{
		sGameData::sEnemyStatus::sLevelInfo l_LevelInfo;
		l_LevelInfo.StroeAllAttribute(l_Element);
		if( !l_pEnemyStatus || l_pEnemyStatus->GetMonsterID() != l_LevelInfo.GetID() )
		{
			l_pEnemyStatus  = this->m_pGameData->GetEnemyStatus(l_LevelInfo.GetID());
			if( !l_pEnemyStatus  )
			{
				sGameData::sEnemyStatus l_EnemyStatus;				
				this->m_pGameData->m_EnemyStatusVector.push_back(l_EnemyStatus);
				l_pEnemyStatus  = this->m_pGameData->GetEnemyStatus(l_LevelInfo.GetID());
			}
		}
		l_pEnemyStatus->m_LevelInfoVector.push_back(l_LevelInfo);
		l_LevelInfo.StroeAllAttribute(l_Element);
		l_Element = l_Element->NextSiblingElement();
	}
	return true;
}

void	cGameData::Render()
{

}
void	cGameData::Update(float e_fElpaseTime)
{

}