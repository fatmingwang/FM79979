#include "stdafx.h"
#include "GameData.h"


sGameData::sMonsterInfo::sMonsterInfo()
{
	m_pHPData = new cLinerDataProcessor<Vector3>();
	m_pSTRData = new cLinerDataProcessor<Vector3>();
	m_pStaminaData = new cLinerDataProcessor<Vector3>();
}

sGameData::sMonsterInfo::~sMonsterInfo()
{
	SAFE_DELETE(m_pHPData);
	SAFE_DELETE(m_pSTRData);
	SAFE_DELETE(m_pStaminaData);
}


sGameData::sMonsterShop::sMonsterShop()
{
	this->m_iPriceType = -1;
	this->m_iMonsterID = -1;
	this->m_fExtraCoin = -1.f;
	std::function<float()> l_Fun1 = std::bind(&sGameData::sMonsterShop::GetExtraCoin,this);
	std::function<float()> l_Fun2 = std::bind(&sGameData::sMonsterShop::GetExtraEXP,this);
	std::function<int()> l_Fun3 = std::bind(&sGameData::sMonsterShop::GetHP,this);
	std::function<int()> l_Fun4 = std::bind(&sGameData::sMonsterShop::GetPrice,this);
	std::function<int()> l_Fun5 = std::bind(&sGameData::sMonsterShop::GetSTR,this);
	m_NameAndValueMap.insert(std::make_pair(this->GetExtraCoinName(),cSimpleFunctionPointerHelp(l_Fun1)));
	this->m_fExtraEXP = -1.f;
	m_NameAndValueMap.insert(std::make_pair(this->GetExtraEXPName(),cSimpleFunctionPointerHelp(l_Fun2)));
	this->m_iHP = -1;
	cSimpleFunctionPointerHelp l_Fun3Fun(l_Fun3);
	m_NameAndValueMap.insert(std::make_pair(this->GetHPName(),cSimpleFunctionPointerHelp(l_Fun3Fun)));
	this->m_iPrice = -1;
	m_NameAndValueMap.insert(std::make_pair(this->GetPriceName(),cSimpleFunctionPointerHelp(l_Fun4)));
	this->m_iSTR = -1;
	m_NameAndValueMap.insert(std::make_pair(this->GetSTRName(),cSimpleFunctionPointerHelp(l_Fun5)));
}
sGameData::sMonsterShop::~sMonsterShop()
{

}

std::map<std::wstring,float>	sGameData::sMonsterShop::GetAttributeNameAndValue()
{
	std::map<std::wstring,float> l_ResultMap;
	for(std::map<std::wstring,cSimpleFunctionPointerHelp>::iterator l_Iterator = m_NameAndValueMap.begin();
		l_Iterator != m_NameAndValueMap.end();l_Iterator++)
	{
		if( l_Iterator->second.GetValue() != -1 )
		{
			l_ResultMap.insert(std::make_pair(l_Iterator->first,l_Iterator->second.GetValue()));
		}
	}
	return l_ResultMap;
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

std::wstring	sGameData::sMonsterInfo::GetMonsterName()
{
	if(this->m_LineDataVector.size())
	{
		auto l_pData = this->m_LineDataVector[0];
		std::wstring l_strName = l_pData->getValue(L"name_CN");
		l_strName += L",";
		l_strName += ValueToStringW(l_pData->getValue(L"name"));
		return l_strName;
	}
	return L"";
}

int		sGameData::sMonsterInfo::GetHitBearCount(int e_iDamage,int e_iLevel)
{
	if(e_iLevel >= (int)m_LineDataVector.size())
	{
		return 0;
	}
	auto l_pLevelInfo = (sLevelInfo*)m_LineDataVector[e_iLevel];
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
	if(this->m_LineDataVector.size())
	{
		return ((sLevelInfo*)(this->m_LineDataVector[0]))->GetID();
	}
	return -1;
}

std::wstring	sGameData::sEnemyStatus::GetMonsterName()
{
	if(this->m_LineDataVector.size())
	{
		auto l_pLevelInfo = (sLevelInfo*)m_LineDataVector[0];
		std::wstring l_strName = l_pLevelInfo->getValue(L"description");
		l_strName += L",";
		l_strName += ValueToStringW(l_pLevelInfo->GetID());
	}
	return L"";	
}

int				sGameData::sEnemyStatus::GetHitBearCount(int e_iDamage,int e_iLevel)
{
	if(e_iLevel >= (int)m_LineDataVector.size())
	{
		return 0;
	}
	auto l_pLevelInfo = (sLevelInfo*)m_LineDataVector[e_iLevel];
	int	l_iHP = l_pLevelInfo->GetHP();
	if( l_iHP > 0 )
	{
		float l_fBearCount = (float)l_iHP/e_iDamage;
		return (int)l_fBearCount;
	}
	return 0;
}



sGameData::sGameData()
{
	m_RenderFunctionPointerVector.push_back(std::bind(&sGameData::MonsterRender,this));
	m_RenderFunctionPointerVector.push_back(std::bind(&sGameData::ShopRender,this));
	m_RenderFunctionPointerVector.push_back(std::bind(&sGameData::EnemyRender,this));

	m_UpdateFunctionPointerVector.push_back(std::bind(&sGameData::MonsterUpdate,this,std::placeholders::_1));
	m_UpdateFunctionPointerVector.push_back(std::bind(&sGameData::ShopUpdate,this,std::placeholders::_1));
	m_UpdateFunctionPointerVector.push_back(std::bind(&sGameData::EnemyUpdate,this,std::placeholders::_1));
}

sGameData::~sGameData()
{
}

sGameData::sEnemyStatus*		sGameData::GetEnemyStatus(int e_iID)
{
	size_t l_uiSize = 0;this->m_EnemyStatusVector.size();
	for(size_t i=0;i<l_uiSize;++i)
	{
		if(e_iID == m_EnemyStatusVector[i].GetMonsterID())
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


void	sGameData::Init()
{
	m_fMonsterInfoStartPosX = 0.f;
	m_fMonsterInfoStartPosY = 0.f;
	m_fMonsterInfoGapX = 90.f;
	m_fMonsterStatusGapX = 30;
	m_fShopStartPosX = 0.f;
	m_fShopStartPosY = 0.f;
	m_fShopGapX = 30.f;
	m_fEnemyStartPosX = 0.f;
	m_fEnemyStartPosY = 0.f;
	m_fEnemyGapX = 30.f;
	//========================================================================
	//
	//
	float	l_fStartPosX = m_fMonsterInfoStartPosX;
	float	l_fStartPosY = m_fMonsterInfoStartPosY;
	float	l_fGapX = m_fMonsterInfoGapX;

	for(size_t i=0;m_MonsterInfoVector.size();++i)
	{
		sMonsterInfo*l_pMonsterInfo = &m_MonsterInfoVector[i];
		for(size_t j=0;j<l_pMonsterInfo->m_LineDataVector.size();++j)
		{
			auto l_pLevelInfo = (sMonsterInfo::sLevelInfo*)l_pMonsterInfo->m_LineDataVector[j];
			l_pMonsterInfo->m_pHPData->AddData(Vector3(l_fStartPosX,(float)l_pLevelInfo->GetHP(),0),0);
			l_pMonsterInfo->m_pSTRData->AddData(Vector3(l_fStartPosX+m_fMonsterStatusGapX,(float)l_pLevelInfo->GetSTR(),0),0);
			l_pMonsterInfo->m_pStaminaData->AddData(Vector3(l_fStartPosX+(m_fMonsterStatusGapX*2),(float)l_pLevelInfo->GetStamina(),0),0);
		}
		l_fStartPosX += l_fGapX;
	}
	l_fStartPosX = m_fShopStartPosX;
	l_fStartPosY = m_fShopStartPosY;
	l_fGapX = m_fShopGapX;
	for(std::map<int,std::vector<sMonsterShop>>::iterator l_Iterator=m_MonsterShopDataMap.begin();
		l_Iterator != m_MonsterShopDataMap.end();l_Iterator++)
	{
		auto l_pVector = &l_Iterator->second;
		for(size_t i=0;i<l_pVector->size();++i)
		{
			sMonsterShop*l_pMonsterShop = (sMonsterShop*)&l_pVector[i];
			std::map<std::wstring,float> l_Result = l_pMonsterShop->GetAttributeNameAndValue();
			for(auto l_Iterator : l_Result)
			{
				cChartWithName::sNameAndData*l_pNameAndData = new cChartWithName::sNameAndData();
				l_pNameAndData->m_Name = l_Iterator.first;
				l_fStartPosX += l_fGapX;
				l_pNameAndData->m_pLineData->AddData(Vector3(l_fStartPosX,0,0),0);
				l_pNameAndData->m_pLineData->AddData(Vector3(l_fStartPosX,l_Iterator.second,0),0);
				l_pMonsterShop->m_LineDataVector.push_back(l_pNameAndData);
			}
		}
	}
	l_fStartPosX = m_fEnemyStartPosX;
	l_fStartPosY = m_fEnemyStartPosY;
	l_fGapX = m_fEnemyGapX;
	for(size_t i=0;m_EnemyStatusVector.size();++i)
	{
	
	}
}

void	sGameData::MonsterRender()
{

}

void	sGameData::MonsterUpdate(float e_fElpaseTime)
{

}
void	sGameData::ShopRender()
{

}
void	sGameData::ShopUpdate(float e_fElpaseTime)
{

}
void	sGameData::EnemyRender()
{

}
void	sGameData::EnemyUpdate(float e_fElpaseTime)
{

}


void	sGameData::Update(int e_iTargetFlag,float e_fElpaseTime)
{
	int l_iCount = (int)m_UpdateFunctionPointerVector.size();
	for(int i=0;i<l_iCount;++i)
	{
		if( i << 1  & e_iTargetFlag)
		{
			m_UpdateFunctionPointerVector[i](e_fElpaseTime);
		}
	}
}

void	sGameData::Render(int e_iTargetFlag)
{
	int l_iCount = (int)m_UpdateFunctionPointerVector.size();
	for(int i=0;i<l_iCount;++i)
	{
		if( i << 1  & e_iTargetFlag)
		{
			m_RenderFunctionPointerVector[i]();
		}
	}
}

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
			sGameData::sMonsterInfo::sLevelInfo*l_pLevelInfo = new sGameData::sMonsterInfo::sLevelInfo();
			l_pLevelInfo->StroeAllAttribute(l_Element);
			int l_iLevel = l_pLevelInfo->GetLevel()-1;
			if( l_iStartIndex < l_iLevel)
				l_iStartIndex = l_iLevel;
			else
			{
				UT::ErrorMsg(L"Level should from 1 to 15", ValueToStringW(l_iLevel) );
			}
			if( !l_pMonsterInfo || l_pMonsterInfo->GetID() != l_pLevelInfo->GetID())
			{
				l_pMonsterInfo = this->m_pGameData->GetMonsterInfo(l_pLevelInfo->GetID());
			}
			if( i == 0 )
			{
				l_pMonsterInfo->m_LineDataVector.push_back(l_pLevelInfo);
			}
			else
			{
				if( l_iLevel < (int)l_pMonsterInfo->m_LineDataVector.size() )
				{
					auto l_pLevelInfo = l_pMonsterInfo->m_LineDataVector[l_iLevel];
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
		sGameData::sEnemyStatus::sLevelInfo*l_pLevelInfo = new sGameData::sEnemyStatus::sLevelInfo();
		l_pLevelInfo->StroeAllAttribute(l_Element);
		if( !l_pEnemyStatus || l_pEnemyStatus->GetMonsterID() != l_pLevelInfo->GetID() )
		{
			l_pEnemyStatus  = this->m_pGameData->GetEnemyStatus(l_pLevelInfo->GetID());
			if( !l_pEnemyStatus  )
			{
				sGameData::sEnemyStatus l_EnemyStatus;				
				this->m_pGameData->m_EnemyStatusVector.push_back(l_EnemyStatus);
				l_pEnemyStatus  = this->m_pGameData->GetEnemyStatus(l_pLevelInfo->GetID());
			}
		}
		l_pEnemyStatus->m_LineDataVector.push_back(l_pLevelInfo);
		l_pLevelInfo->StroeAllAttribute(l_Element);
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