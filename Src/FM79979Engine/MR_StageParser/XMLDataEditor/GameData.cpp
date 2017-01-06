#include "stdafx.h"
#include "GameData.h"
#include "StageParser.h"
extern cStageParser*g_pStageParser ;
cNodeISAX*g_pCurrentNodeISAX = nullptr;
extern Vector4 g_vLineIndexAndColor[6];

bool FUCKFunction(const char*e_strFileName)
{
	return g_pCurrentNodeISAX->ParseDataIntoXMLNode(e_strFileName);
}

bool LazyTempFileParse(cNodeISAX*e_pNodeISAX,const char*e_strFileName)
{
	g_pCurrentNodeISAX = e_pNodeISAX;
	std::function<bool(const char*)> l_Fun1 = std::bind(&FUCKFunction,std::placeholders::_1);
	return DNCT::TempFileToUnicodeParseFunction(gcnew String(e_strFileName),l_Fun1,"utf-8",false);
}

extern cOrthogonalCamera*	g_pOrthogonalCamera;
void	CameraOffset(Vector2 e_vMove,float e_fScale)
{
//	cOrthogonalCamera	l_OrthogonalCamera;
	Vector4 l_vRect = g_pOrthogonalCamera->GetViewRect();
	l_vRect.Move(e_vMove.x,e_vMove.y);
	//l_OrthogonalCamera.SetViewRect(l_vRect);
	//l_OrthogonalCamera.Render();
		//push attribute
	cMatrix44 l_Mat;
	glhOrthof2(l_Mat,l_vRect.x,l_vRect.z,l_vRect.w,l_vRect.y, -10000, 10000);
	//because I invert Y.
	//cMatrix44	l_mat = this->GetWorldTransform();
	//l_mat.SetTranslation(-l_mat.GetTranslation());
	l_Mat *= cMatrix44::ScaleMatrix(Vector3(e_fScale,e_fScale,1));
	FATMING_CORE::SetupShaderViewProjectionMatrix(l_Mat,true);
}

sGameData::sMonsterInfo::sMonsterInfo()
{
	this->m_iUnlockType = -1;
	this->m_iID = -1;
	this->m_iPoolIndex = -1;
	SetRenderData();
}

//sGameData::sMonsterInfo::sMonsterInfo(const sMonsterInfo&e_MonsterInfo):sXmlNode(e_MonsterInfo)
//{
//	assert(0&&"fuck");
//}

sGameData::sMonsterInfo::~sMonsterInfo()
{
	m_AllRenderData.ClearLineDataVector();
}

void	sGameData::sMonsterInfo::SetRenderData()
{
	m_AllRenderData.ClearLineDataVector();
	wchar_t*l_strAttributeName[eMS_MAX] = {
		L"HP",
		L"STR",
		L"Stamania",
		L"Exp",
		L"CoinCost",
		L"TimeCost",
		L"CrystalCost",
		L"Skill CD"
	};
	for(int i=0;i<eMS_MAX;++i)
	{
		cChartWithName::sNameAndData*l_pData = new cChartWithName::sNameAndData();
		l_pData->m_Name = l_strAttributeName[i];
		m_AllRenderData.m_LineDataVector.push_back(l_pData);
	}
}


sGameData::sMonsterShop::sMonsterShop()
{
	Init();
}

sGameData::sMonsterShop::sMonsterShop(const sMonsterShop&e_MonsterShop):sXmlNode(e_MonsterShop)
{
	Init();
}

void		sGameData::sMonsterShop::Init()
{
	this->m_iPoolIndex = -1;
	this->m_iID = -1;
	this->m_iType = -1;
	this->m_iMonsterID = -1;
	this->m_iSTR = -1;
	this->m_iHP = -1;
	this->m_fExtraEXP = -1.f;
	this->m_fExtraCoin = -1.f;
	this->m_fExtraSTR = -1.f;
	this->m_fExtraHP = -1.f;
	this->m_iPrice = -1;
	this->m_iPriceType = -1;
	std::function<float()> l_Fun1 = std::bind(&sGameData::sMonsterShop::GetExtraCoin,this);
	std::function<float()> l_Fun2 = std::bind(&sGameData::sMonsterShop::GetExtraEXP,this);
	std::function<float()> l_Fun6 = std::bind(&sGameData::sMonsterShop::GetExtraSTR,this);
	std::function<int()> l_Fun3 = std::bind(&sGameData::sMonsterShop::GetHP,this);
	std::function<int()> l_Fun4 = std::bind(&sGameData::sMonsterShop::GetPrice,this);
	std::function<int()> l_Fun5 = std::bind(&sGameData::sMonsterShop::GetSTR,this);
	std::function<float()> l_Fun7 = std::bind(&sGameData::sMonsterShop::GetExtraHP,this);

	m_NameAndValueMap.insert(std::make_pair(this->GetPriceName(),new cSimpleFunctionPointerHelp(l_Fun4)));
	m_NameAndValueMap.insert(std::make_pair(this->GetHPName(),new cSimpleFunctionPointerHelp(l_Fun3)));
	m_NameAndValueMap.insert(std::make_pair(this->GetSTRName(),new cSimpleFunctionPointerHelp(l_Fun5)));
	m_NameAndValueMap.insert(std::make_pair(this->GetExtraSTRName(),new cSimpleFunctionPointerHelp(l_Fun6)));
	m_NameAndValueMap.insert(std::make_pair(this->GetExtraCoinName(),new cSimpleFunctionPointerHelp(l_Fun1)));
	m_NameAndValueMap.insert(std::make_pair(this->GetExtraEXPName(),new cSimpleFunctionPointerHelp(l_Fun2)));
	m_NameAndValueMap.insert(std::make_pair(this->GetExtraHPName(),new cSimpleFunctionPointerHelp(l_Fun7)));
}

sGameData::sMonsterShop::~sMonsterShop()
{
	DEL_MAP(std::wstring,cSimpleFunctionPointerHelp*,m_NameAndValueMap);
}

std::wstring	sGameData::sMonsterShop::GetDescription()
{
	std::wstring l_strDescription;
	//l_strDescription += this->getValue(L"name_CN");
	l_strDescription += this->getValue(L"name");
	return l_strDescription;
}

std::map<std::wstring,float>	sGameData::sMonsterShop::GetAttributeNameAndValue()
{
	if( m_AttributeNameAndValue.size() == 0 )
	{
		for(std::map<std::wstring,cSimpleFunctionPointerHelp*>::iterator l_Iterator = m_NameAndValueMap.begin();
			l_Iterator != m_NameAndValueMap.end();l_Iterator++)
		{
			float l_fValue = l_Iterator->second->GetValue();
			std::wstring l_str = l_Iterator->first;
			if(  l_fValue != -1 )
			{
				m_AttributeNameAndValue.insert(std::make_pair(l_Iterator->first,l_fValue));
			}
		}
	}
	return m_AttributeNameAndValue;
}

sGameData::sMonsterInfo::sLevelInfo::sLevelInfo()
{
	this->m_iHP = -1;
	this->m_iSTR = -1;
	this->m_iStamina = -1;
	this->m_iExp = -1;
	this->m_iCoinCost = -1;
	this->m_iTimeCost = -1;
	this->m_iCrystalCost = -1;
	this->m_iSkillCD = -1;
	this->m_iID = -1;
	this->m_iLevel = -1;
}


sGameData::sMonsterInfo::sLevelInfo::~sLevelInfo()
{

}

sGameData::sEnemyStatus::sLevelInfo::sLevelInfo(const sLevelInfo& e_LevelInfo)
{
	this->m_iHP = e_LevelInfo.m_iHP;
	this->m_iSTR = e_LevelInfo.m_iSTR;
	this->m_fTimeAdd = e_LevelInfo.m_fTimeAdd;
	this->m_fTimeMinus = e_LevelInfo.m_fTimeMinus;
	this->m_iSpeed = e_LevelInfo.m_iSpeed;
	this->m_iCloseSpeed = e_LevelInfo.m_iCloseSpeed;
	this->m_fSkillTimeReduce = e_LevelInfo.m_fSkillTimeReduce;
	this->m_iLevel = e_LevelInfo.m_iLevel;
	this->m_iID = e_LevelInfo.m_iID;
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

int	sGameData::sEnemyStatus::GetSpeedByLevel(int e_iLevel)
{
	size_t l_uiSize = this->m_LineDataVector.size();
	if( e_iLevel >= (int)l_uiSize  )
		return -1;
	sLevelInfo*l_pLevelInfo = (sLevelInfo*)m_LineDataVector[e_iLevel];
	return l_pLevelInfo->GetSpeed();
}

std::wstring	sGameData::sMonsterInfo::GetMonsterName()
{
	std::wstring l_strName;
	if(this->m_LineDataVector.size())
	{
		auto l_pLevelInfo = (sLevelInfo*)m_LineDataVector[0];
		l_strName += ValueToStringW(l_pLevelInfo->GetID());
		l_strName += L":";
	}
	l_strName += getValue(L"name_CN");
	l_strName += L",";
	l_strName += ValueToStringW(getValue(L"name"));
	return l_strName;
	//if(this->m_LineDataVector.size())
	//{
	//	auto l_pData = this->m_LineDataVector[0];
	//	std::wstring l_strName = l_pData->getValue(L"name_CN");
	//	l_strName += L",";
	//	l_strName += ValueToStringW(l_pData->getValue(L"name"));
	//	return l_strName;
	//}
	//return L"";
}

std::wstring    sGameData::sMonsterInfo::GetInfo()
{
	std::wstring l_strInfo = GetUnlockInfo();
	l_strInfo += L"\n";
	std::wstring l_strStartFatigue = getValue(L"startfatigue");
	std::wstring l_strFatiguelevel = getValue(L"fatiguelevel");
	std::wstring l_strMaxFatigue = getValue(L"fatiguelimit");
	l_strInfo += L",StartEnergy:";
	l_strInfo += l_strStartFatigue;
	l_strInfo += L"\n";
	l_strInfo += L",RiseLevel:";
	l_strInfo += l_strFatiguelevel;
	l_strInfo += L"\n";
	l_strInfo += L",MaxEnergy:";
	l_strInfo += l_strMaxFatigue;
	return l_strInfo;
}

int	sGameData::sMonsterInfo::GetUnlockValue()
{
	return GetInt(getValue(L"unlockValue_CN"));
}


std::wstring	sGameData::sMonsterInfo::GetUnlockInfo()
{
	std::wstring l_strType;
	int l_iType = this->GetUnlockType();
	std::wstring l_strValue = this->getValue(L"unlockValue_CN");
	//coin
	if( l_iType == 0 )
	{
		l_strType = L"Coin:";
	}
	else//gems
	if( l_iType == 1 )
	{
		l_strType = L"Gems:";
	}
	else//stage clear
	if( l_iType == 2 )
	{
		l_strType = L"StageClear:";
	}
	l_strType += l_strValue;
	return l_strType;
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

//sGameData::sEnemyStatus::sEnemyStatus(const sEnemyStatus& e_EnemyStatus)
//{
//	assert(0&&"fuck");
//}

sGameData::sEnemyStatus::sEnemyStatus()
{
	wchar_t*l_strAttributeName[eES_MAX] = {
		L"HP",
		L"STR",
		L"Time Add",
		L"Time Minus",
		L"Speed",
		L"Close Speed",
		L"SkillT ime Reduce",
	};
	for(int i=0;i<eES_MAX;++i)
	{
		cChartWithName::sNameAndData*l_pData = new cChartWithName::sNameAndData();
		l_pData->m_Name = l_strAttributeName[i];
		m_AllRenderData.m_LineDataVector.push_back(l_pData);
	}
}

sGameData::sEnemyStatus::~sEnemyStatus()
{
	m_AllRenderData.ClearLineDataVector();
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
		std::wstring l_strName = ValueToStringW(l_pLevelInfo->GetID());
		l_strName += L":";
		l_strName += l_pLevelInfo->getValue(L"description");
		return l_strName;
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


sGameData*g_pGameData = nullptr;
sGameData::sGameData()
{
	m_bStageIndexCorrect = false;
	m_RenderFunctionPointerVector.push_back(std::bind(&sGameData::MonsterRender,this));
	m_RenderFunctionPointerVector.push_back(std::bind(&sGameData::ShopRender,this));
	m_RenderFunctionPointerVector.push_back(std::bind(&sGameData::EnemyRender,this));

	m_UpdateFunctionPointerVector.push_back(std::bind(&sGameData::MonsterUpdate,this,std::placeholders::_1));
	m_UpdateFunctionPointerVector.push_back(std::bind(&sGameData::ShopUpdate,this,std::placeholders::_1));
	m_UpdateFunctionPointerVector.push_back(std::bind(&sGameData::EnemyUpdate,this,std::placeholders::_1));
	g_pGameData = this;
	m_pCampaign = nullptr;
}

sGameData::~sGameData()
{
	g_pGameData = nullptr;
	SAFE_DELETE(m_pCampaign);
	DELETE_VECTOR(m_MonsterInfoVector,sMonsterInfo*);
	for(auto l_pData :m_MonsterShopDataMap)
	{
		auto l_pVector = l_pData.second;
		DELETE_VECTOR((*l_pVector),sMonsterShop*);
		SAFE_DELETE(l_pVector);
	}
	DELETE_VECTOR(m_EnemyStatusVector,sEnemyStatus*);
}

bool	sGameData::IsEnemy(int e_iID)
{
	auto l_pData = this->GetMonsterInfo(e_iID);
	if(l_pData)
		return true;
	return false;
}

std::wstring		sGameData::GetMonsterName(int e_iID)
{
	auto l_pData = this->GetMonsterInfo(e_iID);
	std::wstring l_strResult;
	if( l_pData )
	{
		l_strResult += l_pData->GetMonsterName();
	}
	else
		l_strResult += L"ID can not find monster";
	return l_strResult;
}

sGameData::sEnemyStatus*		sGameData::GetEnemyStatus(int e_iID)
{
	size_t l_uiSize = this->m_EnemyStatusVector.size();
	for(size_t i=0;i<l_uiSize;++i)
	{
		if(e_iID == m_EnemyStatusVector[i]->GetMonsterID())
		{
			return m_EnemyStatusVector[i];
		}
	}
	return nullptr;
}

int	sGameData::GetMonsterSpeedByLevel(int e_iID,int e_iLevel)
{
	
	sGameData::sEnemyStatus*l_pEnemyStatus = GetEnemyStatus(e_iID);
	if( l_pEnemyStatus )
		return l_pEnemyStatus->GetSpeedByLevel(e_iLevel);
	return -1;
}

sGameData::sMonsterInfo*	sGameData::GetMonsterInfo(int e_iID)
{
	size_t l_uiSize = this->m_MonsterInfoVector.size();
	for(size_t i=0;i<l_uiSize;++i)
	{
		if(e_iID == m_MonsterInfoVector[i]->GetID())
		{
			return m_MonsterInfoVector[i];
		}
	}
	return nullptr;
}


sGameData::sCampaign::sCampaign()
{
	SetRenderData();
}
sGameData::sCampaign::~sCampaign()
{
	DELETE_VECTOR(m_ChapterVector,sChapter*);
	m_AllRenderData.ClearLineDataVector();
}
void	sGameData::sCampaign::SetRenderData()
{
	m_AllRenderData.ClearLineDataVector();
	wchar_t*l_strAttributeName[eCI_MAX] = {
		L"Coin1",
		L"Coin2",
		L"Coin3",
		L"Exp1",
		L"Exp2",
		L"Exp3"
	};
	for(int i=0;i<eCI_MAX;++i)
	{
		cChartWithName::sNameAndData*l_pData = new cChartWithName::sNameAndData();
		l_pData->m_Name = l_strAttributeName[i];
		m_AllRenderData.m_LineDataVector.push_back(l_pData);
	}
}

int		sGameData::sCampaign::GetIndexByName(std::wstring e_strStageName)
{
	for(size_t i=0;i<m_strStageNameVector.size();++i)
	{
		if(m_strStageNameVector[i] == e_strStageName)
			return i;
	}
	return -1;
}


void	Assign(float e_fPosX,float e_PosY,cChartWithName::sNameAndData*e_pNameAndData,Vector2& e_vMax)
{
	float l_fPosX = e_fPosX;
	float l_fPosY = e_PosY;
	e_pNameAndData->m_pLineData->AddData(Vector3(l_fPosX,l_fPosY,0),0);
	e_vMax.x = l_fPosX;
	if( fabs(e_vMax.y) < fabs(l_fPosY) )
		e_vMax.y = l_fPosY;
}

void	sGameData::EnemyDataInit()
{
	m_fEnemyStartPosX = 0.f;
	m_fEnemyStartPosY = 0.f;
	m_fEnemyGapX = 50.f;
	m_fEnemyStatusGapX = 30.f;
	m_fEnemyScale = 3.f;
	float l_fStartPosX = m_fEnemyStartPosX;
	float l_fStartPosY = m_fEnemyStartPosY;
	float l_fGapX = m_fEnemyGapX;
	for(size_t i=0;i<m_EnemyStatusVector.size();++i)
	{
		auto l_pEnemyStatus = m_EnemyStatusVector[i];
		size_t l_uiSize = l_pEnemyStatus->m_LineDataVector.size();
		l_fStartPosX = m_fEnemyStartPosX;
		for(auto l_pObject :l_pEnemyStatus->m_LineDataVector)
		{
			sEnemyStatus::sLevelInfo*l_pLevelInfo  = (sEnemyStatus::sLevelInfo*)l_pObject;
			Assign(l_fStartPosX,l_fStartPosY-l_pLevelInfo->GetHP()*m_fEnemyScale,l_pEnemyStatus->m_AllRenderData.m_LineDataVector[sEnemyStatus::eES_HP],m_EnemyRenderFlagControl.m_MaxVector[sEnemyStatus::eES_HP]);
			Assign(l_fStartPosX,l_fStartPosY-l_pLevelInfo->GetSTR()*m_fEnemyScale,l_pEnemyStatus->m_AllRenderData.m_LineDataVector[sEnemyStatus::eES_STR],m_EnemyRenderFlagControl.m_MaxVector[sEnemyStatus::eES_STR]);
			Assign(l_fStartPosX,l_fStartPosY-l_pLevelInfo->GetTimeAdd()*m_fEnemyScale,l_pEnemyStatus->m_AllRenderData.m_LineDataVector[sEnemyStatus::eES_TIME_ADD],m_EnemyRenderFlagControl.m_MaxVector[sEnemyStatus::eES_TIME_ADD]);
			Assign(l_fStartPosX,l_fStartPosY-l_pLevelInfo->GetTimeMinus()*m_fEnemyScale,l_pEnemyStatus->m_AllRenderData.m_LineDataVector[sEnemyStatus::eES_TIME_MINUS],m_EnemyRenderFlagControl.m_MaxVector[sEnemyStatus::eES_TIME_MINUS]);
			Assign(l_fStartPosX,l_fStartPosY-l_pLevelInfo->GetSpeed()*m_fEnemyScale,l_pEnemyStatus->m_AllRenderData.m_LineDataVector[sEnemyStatus::eES_SPEED],m_EnemyRenderFlagControl.m_MaxVector[sEnemyStatus::eES_SPEED]);
			Assign(l_fStartPosX,l_fStartPosY-l_pLevelInfo->GetCloseSpeed()*m_fEnemyScale,l_pEnemyStatus->m_AllRenderData.m_LineDataVector[sEnemyStatus::eES_CLOSE_SPEED],m_EnemyRenderFlagControl.m_MaxVector[sEnemyStatus::eES_CLOSE_SPEED]);
			Assign(l_fStartPosX,l_fStartPosY-l_pLevelInfo->GetSkillTimeReduce()*m_fEnemyScale,l_pEnemyStatus->m_AllRenderData.m_LineDataVector[sEnemyStatus::eES_SKILL_TIME_REDUCE],m_EnemyRenderFlagControl.m_MaxVector[sEnemyStatus::eES_SKILL_TIME_REDUCE]);
			l_fStartPosX += l_fGapX;
		}
	}
}

void	sGameData::MonsterDataInit()
{
	m_MonsterRenderFlagControl.Init(sGameData::sMonsterInfo::eMS_MAX);
	m_EnemyRenderFlagControl.Init(sGameData::sEnemyStatus::eES_MAX);
	m_CampaignRenderFlagControl.Init(sGameData::sCampaign::eCI_MAX);
	m_fMonsterInfoStartPosX = 0.f;
	m_fMonsterInfoStartPosY = 0.f;
	m_fMonsterInfoGapX = 50.f;
	m_fMonsterStatusGapX = 5;
	m_fMonsterScale = 5.f;
	float	l_fStartPosX = m_fMonsterInfoStartPosX;
	float	l_fStartPosY = m_fMonsterInfoStartPosY;
	float	l_fGapX = m_fMonsterInfoGapX;
	for(size_t i=0;i<m_MonsterInfoVector.size();++i)
	{
		sMonsterInfo*l_pMonsterInfo = m_MonsterInfoVector[i];
		l_fStartPosX = m_fMonsterInfoStartPosX;
		l_fStartPosY = m_fMonsterInfoStartPosY;
		for(size_t j=0;j<l_pMonsterInfo->m_LineDataVector.size();++j)
		{
			auto l_pLevelInfo = (sMonsterInfo::sLevelInfo*)l_pMonsterInfo->m_LineDataVector[j];
			int l_fDis = 0;//(int)(m_fMonsterStatusGapX*j);
			float l_fPosX = l_fStartPosX+l_fDis;
			float l_fPosY = (float)-l_pLevelInfo->GetHP()*m_fMonsterScale;
			Assign(l_fPosX,l_fPosY,l_pMonsterInfo->m_AllRenderData.m_LineDataVector[sMonsterInfo::eMS_HP],m_MonsterRenderFlagControl.m_MaxVector[sMonsterInfo::eMS_HP]);
			l_fPosY = (float)-l_pLevelInfo->GetSTR()*m_fMonsterScale;
			Assign(l_fPosX,l_fPosY,l_pMonsterInfo->m_AllRenderData.m_LineDataVector[sMonsterInfo::eMS_STR],m_MonsterRenderFlagControl.m_MaxVector[sMonsterInfo::eMS_STR]);
			l_fPosY = (float)-l_pLevelInfo->GetStamina()*m_fMonsterScale;
			Assign(l_fPosX,l_fPosY,l_pMonsterInfo->m_AllRenderData.m_LineDataVector[sMonsterInfo::eMS_STAMINIA],m_MonsterRenderFlagControl.m_MaxVector[sMonsterInfo::eMS_STAMINIA]);
			l_fPosY = (float)-l_pLevelInfo->GetExp()*m_fMonsterScale;
			Assign(l_fPosX,l_fPosY,l_pMonsterInfo->m_AllRenderData.m_LineDataVector[sMonsterInfo::eMS_EXP],m_MonsterRenderFlagControl.m_MaxVector[sMonsterInfo::eMS_EXP]);
			l_fPosY = (float)-l_pLevelInfo->GetCoinCost()*m_fMonsterScale;
			Assign(l_fPosX,l_fPosY,l_pMonsterInfo->m_AllRenderData.m_LineDataVector[sMonsterInfo::eMS_COIN_COST],m_MonsterRenderFlagControl.m_MaxVector[sMonsterInfo::eMS_COIN_COST]);
			l_fPosY = (float)-l_pLevelInfo->GetTimeCost()*m_fMonsterScale;
			Assign(l_fPosX,l_fPosY,l_pMonsterInfo->m_AllRenderData.m_LineDataVector[sMonsterInfo::eMS_TIME_COST],m_MonsterRenderFlagControl.m_MaxVector[sMonsterInfo::eMS_TIME_COST]);
			l_fPosY = (float)-l_pLevelInfo->GetCrystalCost()*m_fMonsterScale;
			Assign(l_fPosX,l_fPosY,l_pMonsterInfo->m_AllRenderData.m_LineDataVector[sMonsterInfo::eMS_CRYSTAL_COST],m_MonsterRenderFlagControl.m_MaxVector[sMonsterInfo::eMS_CRYSTAL_COST]);
			l_fPosY = (float)-l_pLevelInfo->GetSkillCD()*m_fMonsterScale;
			Assign(l_fPosX,l_fPosY,l_pMonsterInfo->m_AllRenderData.m_LineDataVector[sMonsterInfo::eES_SKILL_CD],m_MonsterRenderFlagControl.m_MaxVector[sMonsterInfo::eES_SKILL_CD]);
			l_fStartPosX += l_fGapX;
		}
	}
}

void	sGameData::ShopDataInit()
{
	m_fShopStartPosX = 0.f;
	m_fShopStartPosY = 0.f;
	m_fShopGapX = 30.f;
	m_fShopScale = 1.f;
	float l_fStartPosX = m_fShopStartPosX;
	float l_fStartPosY = m_fShopStartPosY;
	float l_fGapX = m_fShopGapX;
	for(std::map<int,std::vector<sMonsterShop*>*>::iterator l_Iterator=m_MonsterShopDataMap.begin();
		l_Iterator != m_MonsterShopDataMap.end();l_Iterator++)
	{
		l_fStartPosX = m_fShopStartPosX;
		l_fStartPosY = m_fShopStartPosY;
		auto l_pVector = l_Iterator->second;
		for(size_t i=0;i<l_pVector->size();++i)
		{
			sMonsterShop*l_pMonsterShop = (sMonsterShop*)(*l_pVector)[i];
			std::map<std::wstring,float> l_Result = l_pMonsterShop->GetAttributeNameAndValue();
			l_pMonsterShop->m_strItemName = l_pMonsterShop->GetDescription();
			for(auto l_Iterator : l_Result)
			{
				cChartWithName::sNameAndData*l_pNameAndData = new cChartWithName::sNameAndData();
				l_pNameAndData->m_Name = l_Iterator.first;
				//if( l_pNameAndData->m_Name == L"Price" )
				//{
				//	int l_iPriceType = l_pMonsterShop->GetPriceType();
				//	l_pNameAndData->m_Name += ValueToStringW(l_iPriceType);
				//}
				l_pNameAndData->m_pLineData->AddData(Vector3(l_fStartPosX,0,0),0);
				l_pNameAndData->m_pLineData->AddData(Vector3(l_fStartPosX,-l_Iterator.second*m_fShopScale,0),0);
				l_pMonsterShop->m_LineDataVector.push_back(l_pNameAndData);
				l_fStartPosX += l_fGapX;
			}
		}
	}
}
float g_fCampaginDataGap = 30.f;
void	sGameData::CampaginDataInit()
{
	if( !this->m_pCampaign )
		return;
	float l_fStartPosX = 0;
	float l_fStartPosY = 0;
	float l_fGapX = g_fCampaginDataGap;
	this->m_pCampaign->m_strStageNameVector.clear();
	for(sCampaign::sChapter*l_pChapter : this->m_pCampaign->m_ChapterVector)
	{
		for(size_t i=0;i<l_pChapter->m_LineDataVector.size();++i)
		{
			sCampaign::sChapter::sLevelInfo*l_pLevelInfo = (sCampaign::sChapter::sLevelInfo*)l_pChapter->m_LineDataVector[i];
			this->m_pCampaign->m_strStageNameVector.push_back(l_pLevelInfo->getValue(L"gameplay"));
			Assign(l_fStartPosX,l_fStartPosY-l_pLevelInfo->GetCoins1(),m_pCampaign->m_AllRenderData.m_LineDataVector[sCampaign::eCI_COINS1],m_CampaignRenderFlagControl.m_MaxVector[sCampaign::eCI_EXP1]);
			Assign(l_fStartPosX,l_fStartPosY-l_pLevelInfo->GetCoins2(),m_pCampaign->m_AllRenderData.m_LineDataVector[sCampaign::eCI_COINS2],m_CampaignRenderFlagControl.m_MaxVector[sCampaign::eCI_EXP1]);
			Assign(l_fStartPosX,l_fStartPosY-l_pLevelInfo->GetCoins3(),m_pCampaign->m_AllRenderData.m_LineDataVector[sCampaign::eCI_COINS3],m_CampaignRenderFlagControl.m_MaxVector[sCampaign::eCI_EXP1]);
			Assign(l_fStartPosX,l_fStartPosY-l_pLevelInfo->GetEXP1(),m_pCampaign->m_AllRenderData.m_LineDataVector[sCampaign::eCI_EXP1],m_CampaignRenderFlagControl.m_MaxVector[sCampaign::eCI_EXP1]);
			Assign(l_fStartPosX,l_fStartPosY-l_pLevelInfo->GetEXP2(),m_pCampaign->m_AllRenderData.m_LineDataVector[sCampaign::eCI_EXP2],m_CampaignRenderFlagControl.m_MaxVector[sCampaign::eCI_EXP1]);
			Assign(l_fStartPosX,l_fStartPosY-l_pLevelInfo->GetEXP3(),m_pCampaign->m_AllRenderData.m_LineDataVector[sCampaign::eCI_EXP3],m_CampaignRenderFlagControl.m_MaxVector[sCampaign::eCI_EXP1]);
			l_fStartPosX += l_fGapX;
		}
	}
}

void	sGameData::Init()
{
	//========================================================================
	//
	//
	MonsterDataInit();
	EnemyDataInit();
	ShopDataInit();
	CampaginDataInit();
}

void	sGameData::MonsterRender()
{
	CameraOffset(Vector2(-500,-1200),0.5);
	const int l_iStartPosY = -1000;
	bool	l_bShowValue = this->m_MonsterRenderFlagControl.m_bShowValue;
	//m_iRenderMonsterFlasg = 1<<2;
	float l_fScale = m_fMonsterScale;
	int l_iRenderFlag = m_MonsterRenderFlagControl.m_iRenderFlag;
	if( l_iRenderFlag == 0 )
		l_iRenderFlag = 1;
	std::vector<int> l_RenderIndex = m_MonsterRenderFlagControl.m_RenderIndexVector;
	if( l_RenderIndex.size() == 0 )
	{
		for(size_t i=0;i<m_MonsterInfoVector.size();++i)
			l_RenderIndex.push_back(i);
	}
	for(size_t i=0;i<l_RenderIndex.size();++i)
	{
		if( i >= m_MonsterInfoVector.size() )
			continue;
		int l_iIndex = l_RenderIndex[i];
		sMonsterInfo*l_pMonsterInfo = m_MonsterInfoVector[l_iIndex];
		cGameApp::m_spGlyphFontRender->SetFontColor(g_vLineIndexAndColor[l_iIndex]);
		cGameApp::RenderFont(-350,l_iStartPosY+i*150,l_pMonsterInfo->GetMonsterName());
		cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
		cGameApp::RenderFont(-350,l_iStartPosY+30+i*150,l_pMonsterInfo->GetInfo());

		for(size_t j=0;j<l_pMonsterInfo->m_AllRenderData.m_LineDataVector.size();++j)
		{
			if( l_iRenderFlag & 1<<j )
			{
				Vector2	l_vPos = m_MonsterRenderFlagControl.m_MaxVector[j];
				float l_fYDataScale = 1;
				if( l_vPos.y != 0.f )
				{
					l_fYDataScale = fabs(l_vPos.v)/1000;
				}
				if( i == 0 )
				{
					cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
					cGameApp::RenderFont(0,0-i*(60)+100,l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_Name.c_str());
					Vector2 l_XAxis[2] = {Vector2(0,0),Vector2(l_vPos.x,0)};
					Vector2 l_YAxis[2] = {Vector2(0,0),Vector2(0,l_vPos.y/l_fYDataScale)};
					GLRender::RenderLine((float*)&l_XAxis,2,Vector4::One,2);
					GLRender::RenderLine((float*)&l_YAxis,2,Vector4::One,2);
					const int l_iStride = 5;
					float l_fStepValue = l_vPos.y/l_iStride;
					for(int k=0;k<l_iStride+1;++k)
					{
						float l_fValue = l_fStepValue*k;
						cGameApp::RenderFont(-100.f,k*l_fStepValue/l_fYDataScale,ValueToStringW(-l_fValue/l_fScale));
					}
					size_t l_iXCount = l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_pLineData->Count();
					auto l_pVector = l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_pLineData->GetLinerDataVector();
					for(size_t k=0;k<l_iXCount;++k)
					{
						cGameApp::RenderFont((*l_pVector)[k].x,30.f,ValueToStringW(k));
					}
				}
				l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_pLineData->DebugRender(false,false,g_vLineIndexAndColor[l_iIndex],cMatrix44::ScaleMatrix(Vector3(1,1/l_fYDataScale,1)));
				if( l_bShowValue )
				{
					size_t l_iXCount = l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_pLineData->Count();
					auto l_pVector = l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_pLineData->GetLinerDataVector();
					for(size_t k=0;k<l_iXCount;++k)
					{
						cGameApp::RenderFont((*l_pVector)[k].x,(*l_pVector)[k].y/l_fYDataScale,ValueToStringW((int)((*l_pVector)[k].y/-l_fScale)));
					}
				}
			}
		}
	}
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
}

void	sGameData::MonsterUpdate(float e_fElpaseTime)
{

}
void	sGameData::ShopRender()
{
	m_fShopStartPosX = 0.f;
	m_fShopStartPosY = 0.f;
	m_fShopGapX = 30.f;
	m_fShopScale = 1.f;

	float l_fPosX = m_fShopStartPosX;
	float l_fPosY = m_fShopStartPosY;
	float l_fGap = m_fShopGapX;
	float l_fGapY = 180;
	float l_fValueGapY = 30;
	float l_fScale = m_fShopScale;
	int l_iIndex = 0;
	for(std::map<int,std::vector<sMonsterShop*>*>::iterator l_Iterator = m_MonsterShopDataMap.begin();
		l_Iterator != m_MonsterShopDataMap.end();l_Iterator++)
	{
		l_fPosX = m_fShopStartPosX;
		std::wstring l_str = ValueToStringW(l_Iterator->first);
		cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1,1,0.5,1));
		l_fPosY = l_iIndex*l_fGapY;
		cGameApp::RenderFont(l_fPosX,l_fPosY,l_str.c_str());
		Vector2 l_vSize = cGameApp::m_spGlyphFontRender->GetRenderSize(l_str.c_str());
		l_fPosX += l_vSize.x+l_fGap;
		auto l_pVector = l_Iterator->second;
		for(size_t i=0;i<l_pVector->size();++i)
		{
			sMonsterShop*l_pMonsterShop = (sMonsterShop*)(*l_pVector)[i];
			cGameApp::m_spGlyphFontRender->SetFontColor(g_vLineIndexAndColor[l_pMonsterShop->GetType()]);
			l_str = l_pMonsterShop->GetDescription();;
			cGameApp::RenderFont(l_fPosX,l_fPosY,l_str);
			cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
			for(auto l_pIterator:l_pMonsterShop->m_AttributeNameAndValue)
			{
				l_fPosY += l_fValueGapY;
				std::wstring l_strResult = l_pIterator.first;
				if( l_strResult == L"Price" )
				{
					int l_iPriceType = l_pMonsterShop->GetPriceType();
					if( l_iPriceType == 1 )
					{
						cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1,1,0,1));
						l_strResult  = L"Coins";
					}
					else
					if( l_iPriceType == 2 )
					{
						cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1,1,0,1));
						l_strResult  = L"Gems";
					}
					else
					{
						cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(0,0,1,1));
						l_strResult  = L"F Points";
					}
				}
				else
				if( l_strResult == L"HP" )
				{
					cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1.5f,0.f,0.f,1));
				}
				else
				if( l_strResult.find(L"Extra") != std::wstring::npos )
				{
					if( l_strResult.find(L"HP") != std::wstring::npos )
					{
						cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1.5f,0.f,1.5f,1));
					}
					else
					if( l_strResult.find(L"STR") != std::wstring::npos )
					{
						cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(0.f,1.5f,1.5f,1));
					}
					
				}
				else
				if( l_strResult == L"STR" )
				{
					cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(0.f,1.5f,0.f,1));
				}
				l_strResult += L" : ";
				l_strResult += ValueToStringW(l_pIterator.second>1.1f?(int)l_pIterator.second:l_pIterator.second);
				cGameApp::RenderFont(l_fPosX,l_fPosY,l_strResult);
				cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
			}
			l_vSize = cGameApp::m_spGlyphFontRender->GetRenderSize(l_str.c_str());
			l_fPosX += 300;
			l_fPosY = l_iIndex*l_fGapY;
			//l_pMonsterShop->Render();
		}
		l_fPosX = m_fShopStartPosX;
		l_iIndex++;
	}
}

void	sGameData::CampaginRender()
{
	CameraOffset(Vector2(-500,-650),0.5);
	if( !m_bStageIndexCorrect )
	{
		cGameApp::RenderFont(0,0,L"please open gameplay.xml to correct index");
		if(g_pStageParser && g_pStageParser->m_GamePlayVector.size()>0)
		{
			std::vector<cChartWithName::sNameAndData*>			l_TempLineDataVector;
			for(size_t i=0;i<m_pCampaign->m_AllRenderData.m_LineDataVector.size();++i)
			{
				l_TempLineDataVector.push_back(m_pCampaign->m_AllRenderData.m_LineDataVector[i]);
			}
			std::map<int,int>	l_CorrectIndex;
			for(size_t i=0;i<g_pStageParser->m_GamePlayVector.size();++i)
			{
				
				int l_iIndex = m_pCampaign->GetIndexByName(g_pStageParser->GetStageNameByIndex(i));
				if( l_iIndex == -1 )
				{
					cGameApp::RenderFont(10010,0,L"data error please open gameplay.xml to correct index");
					return;
				}
				l_CorrectIndex[i] = l_iIndex;
			}
			for(size_t j=0;j<m_pCampaign->m_AllRenderData.m_LineDataVector.size();++j)
			{
				auto l_pCloneData = new cLinerDataProcessor<Vector3>(m_pCampaign->m_AllRenderData.m_LineDataVector[j]->m_pLineData);
				for(size_t i=0;i<l_CorrectIndex.size();++i)
				{
					int l_iCorrectIndex = l_CorrectIndex[i];
					Vector3 l_vPos = *l_pCloneData->GetData(l_iCorrectIndex);
					l_vPos.x = i*g_fCampaginDataGap;
					m_pCampaign->m_AllRenderData.m_LineDataVector[j]->m_pLineData->ChangeData(i,l_vPos);
				}				
				delete l_pCloneData;
			}
			m_bStageIndexCorrect = true;
		}
	}
	bool	l_bShowValue = this->m_CampaignRenderFlagControl.m_bShowValue;
	std::vector<int> l_RenderIndex = m_CampaignRenderFlagControl.m_RenderIndexVector;
	if( l_RenderIndex.size() == 0 )
	{
		for(size_t i=0;i<m_pCampaign->m_AllRenderData.m_LineDataVector.size();++i)
			l_RenderIndex.push_back(i);
	}
	int l_iRenderFlag = m_CampaignRenderFlagControl.m_iRenderFlag;
	if( l_iRenderFlag == 0 )
		l_iRenderFlag = 1;
	for(size_t i=0;i<l_RenderIndex.size();++i)
	{
		int l_iIndex = l_RenderIndex[i];
		auto l_pData = m_pCampaign->m_AllRenderData.m_LineDataVector[l_iIndex];
		cGameApp::m_spGlyphFontRender->SetFontColor(g_vLineIndexAndColor[l_iIndex]);
		cGameApp::m_spGlyphFontRender->SetScale(3.f);
		cGameApp::RenderFont(-350,-1000+i*50,l_pData->m_Name.c_str());
		cGameApp::m_spGlyphFontRender->SetScale(1.f);
		Vector2	l_vPos = this->m_CampaignRenderFlagControl.m_MaxVector[sCampaign::eCI_EXP1];
		cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
		if( i == 0 )
		{
			Vector2 l_XAxis[2] = {Vector2(0,0),Vector2(l_vPos.x,0)};
			Vector2 l_YAxis[2] = {Vector2(0,0),Vector2(0,l_vPos.y)};
			GLRender::RenderLine((float*)&l_XAxis,2,Vector4::One,2);
			GLRender::RenderLine((float*)&l_YAxis,2,Vector4::One,2);
			const int l_iStride = 5;
			float l_fStepValue = l_vPos.y/l_iStride;
			cGameApp::m_spGlyphFontRender->SetScale(2.f);
			for(int k=0;k<l_iStride+1;++k)
			{
				float l_fValue = l_fStepValue*k;
				cGameApp::RenderFont(-100.f,k*l_fStepValue,ValueToStringW(-l_fValue));
			}
			cGameApp::m_spGlyphFontRender->SetScale(3.f);
			size_t l_iXCount = m_pCampaign->m_AllRenderData.m_LineDataVector[l_iIndex]->m_pLineData->Count();
			auto l_pVector = m_pCampaign->m_AllRenderData.m_LineDataVector[l_iIndex]->m_pLineData->GetLinerDataVector();
			cGameApp::m_spGlyphFontRender->SetScale(0.7f);
			for(size_t k=0;k<l_iXCount;++k)
			{
				cGameApp::RenderFont((*l_pVector)[k].x,30.f,ValueToStringW(k));
			}
				cGameApp::m_spGlyphFontRender->SetScale(1.f);
		}
		m_pCampaign->m_AllRenderData.m_LineDataVector[l_iIndex]->m_pLineData->DebugRender(false,false,g_vLineIndexAndColor[l_iIndex]);
		if( l_bShowValue )
		{
			cGameApp::m_spGlyphFontRender->SetScale(0.7f);
			auto l_pData = m_pCampaign->m_AllRenderData.m_LineDataVector[l_iIndex];
			l_pData->m_Name;
			size_t l_iXCount = m_pCampaign->m_AllRenderData.m_LineDataVector[l_iIndex]->m_pLineData->Count();
			auto l_pVector = m_pCampaign->m_AllRenderData.m_LineDataVector[l_iIndex]->m_pLineData->GetLinerDataVector();
			for(size_t k=0;k<l_iXCount;++k)
			{
				cGameApp::RenderFont((*l_pVector)[k].x,(*l_pVector)[k].y,ValueToStringW((int)-(*l_pVector)[k].y));
				Vector2 l_YAxis[2] = {Vector2((*l_pVector)[k].x,(*l_pVector)[k].y),Vector2((*l_pVector)[k].x,15)};
				GLRender::RenderLine((float*)&l_YAxis,2,Vector4::One,2);

			}
			cGameApp::m_spGlyphFontRender->SetScale(1.f);
		}
	}
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
	g_pOrthogonalCamera->Render();
}

void	sGameData::ShopUpdate(float e_fElpaseTime)
{

}
void	sGameData::EnemyRender()
{
	CameraOffset(Vector2(-1100,-1200),0.5);
	float l_fScale = m_fEnemyScale;
	bool	l_bShowValue = this->m_EnemyRenderFlagControl.m_bShowValue;
	std::vector<int> l_RenderIndex = m_EnemyRenderFlagControl.m_RenderIndexVector;
	if( l_RenderIndex.size() == 0 )
	{
		for(size_t i=0;i<m_EnemyStatusVector.size();++i)
			l_RenderIndex.push_back(i);
	}
	int l_iRenderFlag = m_EnemyRenderFlagControl.m_iRenderFlag;
	if( l_iRenderFlag == 0 )
		l_iRenderFlag = 1;
	for(size_t i=0;i<l_RenderIndex.size();++i)
	{
		if( i >= this->m_EnemyStatusVector.size() )
			continue;
		int l_iIndex = l_RenderIndex[i];
		auto*l_pMonsterInfo = m_EnemyStatusVector[l_iIndex];
		cGameApp::m_spGlyphFontRender->SetFontColor(g_vLineIndexAndColor[l_iIndex]);
		cGameApp::RenderFont(-350,-1000+i*50,l_pMonsterInfo->GetMonsterName());
		for(size_t j=0;j<l_pMonsterInfo->m_AllRenderData.m_LineDataVector.size();++j)
		{
			if( l_iRenderFlag & 1<<j )
			{
				Vector2	l_vPos = this->m_EnemyRenderFlagControl.m_MaxVector[j];
				float l_fYDataScale = 1;
				if( l_vPos.y != 0.f )
				{
					l_fYDataScale = fabs(l_vPos.v)/1000;
				}
				if( i == 0 )
				{
					cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
					cGameApp::RenderFont(0,0-i*50+100,l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_Name.c_str());
					Vector2 l_XAxis[2] = {Vector2(0,0),Vector2(l_vPos.x,0)};
					Vector2 l_YAxis[2] = {Vector2(0,0),Vector2(0,l_vPos.y/l_fYDataScale)};
					GLRender::RenderLine((float*)&l_XAxis,2,Vector4::One,2);
					GLRender::RenderLine((float*)&l_YAxis,2,Vector4::One,2);
					const int l_iStride = 5;
					float l_fStepValue = l_vPos.y/l_iStride;
					for(int k=0;k<l_iStride+1;++k)
					{
						float l_fValue = l_fStepValue*k;
						cGameApp::RenderFont(-100.f,k*l_fStepValue/l_fYDataScale,ValueToStringW(-l_fValue/l_fScale));
					}
					size_t l_iXCount = l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_pLineData->Count();
					auto l_pVector = l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_pLineData->GetLinerDataVector();
					for(size_t k=0;k<l_iXCount;++k)
					{
						cGameApp::RenderFont((*l_pVector)[k].x,30.f,ValueToStringW(k));
					}
				}
				l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_pLineData->DebugRender(false,false,g_vLineIndexAndColor[l_iIndex],cMatrix44::ScaleMatrix(Vector3(1,1/l_fYDataScale,1)));
				if( l_bShowValue )
				{
					size_t l_iXCount = l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_pLineData->Count();
					auto l_pVector = l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_pLineData->GetLinerDataVector();
					for(size_t k=0;k<l_iXCount;++k)
					{
						cGameApp::RenderFont((*l_pVector)[k].x,(*l_pVector)[k].y/l_fYDataScale,ValueToStringW((int)((*l_pVector)[k].y/-l_fScale)));
					}
				}
			}
		}
	}
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
	g_pOrthogonalCamera->Render();
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
	m_strMonsterFileName = "monster.xml";
	m_strMonsterLevelFileName = "monsterlevelinfo.xml";
	m_strMonsterHealthFileName = "health.xml";
	m_strMonsterStregthFileName = "strength.xml";
	m_strMonsterStaminaFileName = "stamina.xml";
	m_strCampaginFileName = "campaign.xml";
	m_pGameData = nullptr;
}

cGameData::~cGameData()
{
	SAFE_DELETE(m_pGameData);
}

bool	cGameData::Parse(const char*e_strDirectory)
{
	SAFE_DELETE(m_pGameData);
	m_pGameData = new sGameData();
	m_strFirectory = e_strDirectory;
	std::string l_strShopFileName = m_strFirectory+m_strShopFileName;
	std::string l_strNPCInfoFileName = m_strFirectory+m_strNPCInfoFileName;
	std::string l_strMonsterFileName = m_strFirectory+m_strMonsterFileName;
	std::string l_strMonsterLevelFileName = m_strFirectory+m_strMonsterLevelFileName;
	std::string l_strMonsterHealthFileName = m_strFirectory+m_strMonsterHealthFileName;
	std::string l_strMonsterStregthFileName = m_strFirectory+m_strMonsterStregthFileName;
	std::string l_strMonsterStaminaFileName = m_strFirectory+m_strMonsterStaminaFileName;
	std::string l_strCampaginFileName = m_strFirectory+m_strCampaginFileName;
	ParseMonsterFile(l_strMonsterFileName.c_str(),
		l_strMonsterLevelFileName.c_str(),
		l_strMonsterStaminaFileName.c_str(),
		l_strMonsterStregthFileName.c_str(),
		l_strMonsterHealthFileName.c_str());
	ParseShopFile(l_strShopFileName.c_str());
	ParseNPCInfoFile(l_strNPCInfoFileName.c_str());
	ParseCampaginFile(l_strCampaginFileName.c_str());
	m_pGameData->Init();
	return true;
}

bool	cGameData::ParseShopFile(const char*e_strFileName)
{
	cNodeISAX l_NodeISAX;
	if(!LazyTempFileParse(&l_NodeISAX,e_strFileName))
		return false;
	auto l_Element = l_NodeISAX.GetRootElement();
	l_Element = l_Element->FirstChildElement();
	while(l_Element)
	{
		sGameData::sMonsterShop* l_pShopXmlNode = new sGameData::sMonsterShop();
		l_pShopXmlNode->StroeAllAttribute(l_Element);
		int	l_iID = l_pShopXmlNode->GetMonsterID();
		std::vector<sGameData::sMonsterShop*>*l_pMonsterShopVector = nullptr;
		if(!IsMapContainKey<int,std::vector<sGameData::sMonsterShop*>*>(&m_pGameData->m_MonsterShopDataMap,l_iID))
		{
			std::vector<sGameData::sMonsterShop*>* l_pMonsterShopVector = new std::vector<sGameData::sMonsterShop*>();
			m_pGameData->m_MonsterShopDataMap[l_iID] = l_pMonsterShopVector;
		}
		l_pMonsterShopVector = m_pGameData->m_MonsterShopDataMap[l_iID];
		l_pMonsterShopVector->push_back(l_pShopXmlNode);
		l_Element = l_Element->NextSiblingElement();
		
	}
	return true;
}

bool	cGameData::ParseMonsterFile(const char*e_strMonsterFileName,
		const char*e_strMonsterLevelFileName,
		const char*e_strStaminaFileName,
		const char*e_strStregthFileName,
		const char*e_strHealthFileName)
{
	cNodeISAX l_MonsterNodeISAX1;
	cNodeISAX l_MonsterLevelInfoNodeISAX1;
	cNodeISAX l_StaminaNodeISAX1;
	cNodeISAX l_StregthNodeISAX1;
	cNodeISAX l_HealthNodeISAX1;
	if(!LazyTempFileParse(&l_MonsterNodeISAX1,e_strMonsterFileName)||
		!LazyTempFileParse(&l_MonsterLevelInfoNodeISAX1,e_strMonsterLevelFileName)||
		!LazyTempFileParse(&l_StaminaNodeISAX1,e_strStaminaFileName)||
		!LazyTempFileParse(&l_StregthNodeISAX1,e_strStregthFileName)||
		!LazyTempFileParse(&l_HealthNodeISAX1,e_strHealthFileName))
		return false;
	const int l_iArraySize = 4;
	cNodeISAX*l_pNodeISAXArray[l_iArraySize] = {&l_MonsterLevelInfoNodeISAX1,&l_StaminaNodeISAX1,&l_StregthNodeISAX1,&l_HealthNodeISAX1};
	auto l_Element = l_MonsterNodeISAX1.GetRootElement();
	l_Element = l_Element->FirstChildElement();
	while(l_Element)
	{
		sGameData::sMonsterInfo* l_pMonsterLevelInfo = new sGameData::sMonsterInfo();
		l_pMonsterLevelInfo->StroeAllAttribute(l_Element);
		this->m_pGameData->m_MonsterInfoVector.push_back(l_pMonsterLevelInfo);
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
				if( l_iLevel == 0 )
					l_iStartIndex = 0;
				else
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
					SAFE_DELETE(l_pLevelInfo);
					auto l_pLevelInfo2 = l_pMonsterInfo->m_LineDataVector[l_iLevel];
					l_pLevelInfo2->Merge(l_Element);
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
	if(!LazyTempFileParse(&l_NodeISAX,e_strFileName))
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
				sGameData::sEnemyStatus* l_pEnemyStatus2 = new sGameData::sEnemyStatus();	
				this->m_pGameData->m_EnemyStatusVector.push_back(l_pEnemyStatus2);
				l_pEnemyStatus = l_pEnemyStatus2;
			}
		}
		l_pEnemyStatus->m_LineDataVector.push_back(l_pLevelInfo);
		l_Element = l_Element->NextSiblingElement();
	}
	return true;
}

bool		cGameData::ParseCampaginFile(const char*e_strFileName)
{
	cNodeISAX l_NodeISAX;
	if(!LazyTempFileParse(&l_NodeISAX,e_strFileName))
		return false;
	auto l_Element = l_NodeISAX.GetRootElement();
	l_Element = l_Element->FirstChildElement();
	SAFE_DELETE(this->m_pGameData->m_pCampaign);
	m_pGameData->m_pCampaign = new sGameData::sCampaign();
	auto l_pCampaign = m_pGameData->m_pCampaign;
	sGameData::sEnemyStatus*l_pEnemyStatus = nullptr;
	sGameData::sCampaign::sChapter*l_pCurrentChapter = nullptr;
	while(l_Element)
	{
		const WCHAR*l_strValue = l_Element->Value();
		COMPARE_VALUE("chapter")
		{
			sGameData::sCampaign::sChapter*l_pChapter = new sGameData::sCampaign::sChapter();
			l_pCampaign->m_ChapterVector.push_back(l_pChapter);
			l_pChapter->StroeAllAttribute(l_Element);
			l_pChapter->m_Name = l_pChapter->getValue(L"id");
			auto l_FirstChildElement = l_Element->FirstChildElement();
			while( l_FirstChildElement )
			{
				sGameData::sCampaign::sChapter::sLevelInfo*l_pLevelInfo = new sGameData::sCampaign::sChapter::sLevelInfo();
				l_pLevelInfo->StroeAllAttribute(l_FirstChildElement);
				l_pLevelInfo->m_Name = l_pLevelInfo->getValue(L"stagename");
				l_pChapter->m_LineDataVector.push_back(l_pLevelInfo);
				l_FirstChildElement = l_FirstChildElement->NextSiblingElement();
			}
		}
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