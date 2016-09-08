#include "stdafx.h"
#include "GameData.h"

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
	return DNCT::TempFileToUnicodeParseFunction(gcnew String(e_strFileName),l_Fun1,"utf-8");
}

sGameData::sMonsterInfo::sMonsterInfo()
{
	this->m_iID = -1;
	m_pHPData = new cChartWithName::sNameAndData();
	m_pHPData->m_Name = L"HP";
	m_pSTRData = new cChartWithName::sNameAndData();
	m_pSTRData->m_Name = L"STR";
	m_pStaminaData = new cChartWithName::sNameAndData();
	m_pStaminaData->m_Name = L"Stamina";
	SetRenderData();
}

sGameData::sMonsterInfo::sMonsterInfo(const sMonsterInfo&e_MonsterInfo):sXmlNode(e_MonsterInfo)
{
	this->m_iID = e_MonsterInfo.m_iID;
//	m_pHPData = new cLinerDataProcessor<Vector3>(e_MonsterInfo.m_pHPData);
//	m_pSTRData = new cLinerDataProcessor<Vector3>(e_MonsterInfo.m_pSTRData);
//	m_pStaminaData = new cLinerDataProcessor<Vector3>(e_MonsterInfo.m_pStaminaData);
//	SetRenderData();
}

sGameData::sMonsterInfo::~sMonsterInfo()
{
	SAFE_DELETE(m_pHPData);
	SAFE_DELETE(m_pSTRData);
	SAFE_DELETE(m_pStaminaData);
}

void	sGameData::sMonsterInfo::SetRenderData()
{
	m_AllRenderData.m_LineDataVector.push_back(m_pHPData);
	m_AllRenderData.m_LineDataVector.push_back(m_pSTRData);
	m_AllRenderData.m_LineDataVector.push_back(m_pStaminaData);
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
	this->m_iMonsterID = -1;
	this->m_iSTR = -1;
	this->m_iHP = -1;
	this->m_fExtraEXP = -1;
	this->m_fExtraCoin = -1.f;
	this->m_iPrice = -1;
	this->m_iPriceType = -1;
	std::function<float()> l_Fun1 = std::bind(&sGameData::sMonsterShop::GetExtraCoin,this);
	std::function<float()> l_Fun2 = std::bind(&sGameData::sMonsterShop::GetExtraEXP,this);
	std::function<int()> l_Fun3 = std::bind(&sGameData::sMonsterShop::GetHP,this);
	std::function<int()> l_Fun4 = std::bind(&sGameData::sMonsterShop::GetPrice,this);
	std::function<int()> l_Fun5 = std::bind(&sGameData::sMonsterShop::GetSTR,this);
	m_NameAndValueMap.insert(std::make_pair(this->GetExtraCoinName(),new cSimpleFunctionPointerHelp(l_Fun1)));
	m_NameAndValueMap.insert(std::make_pair(this->GetExtraEXPName(),new cSimpleFunctionPointerHelp(l_Fun2)));
	m_NameAndValueMap.insert(std::make_pair(this->GetHPName(),new cSimpleFunctionPointerHelp(l_Fun3)));
	m_NameAndValueMap.insert(std::make_pair(this->GetPriceName(),new cSimpleFunctionPointerHelp(l_Fun4)));
	m_NameAndValueMap.insert(std::make_pair(this->GetSTRName(),new cSimpleFunctionPointerHelp(l_Fun5)));
}

sGameData::sMonsterShop::~sMonsterShop()
{
	DEL_MAP(std::wstring,cSimpleFunctionPointerHelp*,m_NameAndValueMap);
}

std::map<std::wstring,float>	sGameData::sMonsterShop::GetAttributeNameAndValue()
{
	std::map<std::wstring,float> l_ResultMap;
	for(std::map<std::wstring,cSimpleFunctionPointerHelp*>::iterator l_Iterator = m_NameAndValueMap.begin();
		l_Iterator != m_NameAndValueMap.end();l_Iterator++)
	{
		float l_fValue = l_Iterator->second->GetValue();
		std::wstring l_str = l_Iterator->first;
		if(  l_fValue != -1 )
		{
			l_ResultMap.insert(std::make_pair(l_Iterator->first,l_fValue));
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

sGameData::sEnemyStatus::sEnemyStatus(const sEnemyStatus& e_EnemyStatus)
{
	m_pHPData = new cLinerDataProcessor<Vector3>(e_EnemyStatus.m_pHPData);
	m_pSTRData = new cLinerDataProcessor<Vector3>(e_EnemyStatus.m_pSTRData);
	m_pTimeAdd = new cLinerDataProcessor<Vector3>(e_EnemyStatus.m_pTimeAdd);
	m_pTimeMinus = new cLinerDataProcessor<Vector3>(e_EnemyStatus.m_pTimeMinus);
	m_pSpeed = new cLinerDataProcessor<Vector3>(e_EnemyStatus.m_pSpeed);
	m_pCloseSpeed = new cLinerDataProcessor<Vector3>(e_EnemyStatus.m_pCloseSpeed);
	m_pSkillTimeReduce = new cLinerDataProcessor<Vector3>(e_EnemyStatus.m_pSkillTimeReduce);
	for(size_t i=0;i<e_EnemyStatus.m_LineDataVector.size();++i)
	{
		//sLevelInfo*l_pNameAndData = new sLevelInfo(e_EnemyStatus.m_LineDataVector[i]);
//		m_LineDataVector.push_back(l_pNameAndData);
	}
}

sGameData::sEnemyStatus::sEnemyStatus()
{
	m_pHPData = new cLinerDataProcessor<Vector3>();
	m_pSTRData = new cLinerDataProcessor<Vector3>();
	m_pTimeAdd = new cLinerDataProcessor<Vector3>();
	m_pTimeMinus = new cLinerDataProcessor<Vector3>();
	m_pSpeed = new cLinerDataProcessor<Vector3>();
	m_pCloseSpeed = new cLinerDataProcessor<Vector3>();
	m_pSkillTimeReduce = new cLinerDataProcessor<Vector3>();
}

sGameData::sEnemyStatus::~sEnemyStatus()
{
	SAFE_DELETE(m_pHPData);
	SAFE_DELETE(m_pSTRData);
	SAFE_DELETE(m_pTimeAdd);
	SAFE_DELETE(m_pTimeMinus);
	SAFE_DELETE(m_pSpeed);
	SAFE_DELETE(m_pCloseSpeed);
	SAFE_DELETE(m_pSkillTimeReduce);
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
	DELETE_VECTOR(m_MonsterInfoVector,sMonsterInfo*);
	for(auto l_pData :m_MonsterShopDataMap)
	{
		auto l_pVector = l_pData.second;
		for(auto l_pObjectOfVector :*l_pVector)
		{
			SAFE_DELETE(l_pObjectOfVector);
		}
		SAFE_DELETE(l_pVector);
	}
	DELETE_VECTOR(m_EnemyStatusVector,sEnemyStatus*);

	m_MonsterInfoVector.clear();
	m_MonsterShopDataMap.clear();
	m_EnemyStatusVector.clear();
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


void	Assign(float e_fPosX,float e_PosY,cChartWithName::sNameAndData*e_pNameAndData,Vector2& e_vMax)
{
	float l_fPosX = e_fPosX;
	float l_fPosY = e_PosY;
	e_pNameAndData->m_pLineData->AddData(Vector3(l_fPosX,l_fPosY,0),0);
	e_vMax.x = l_fPosX;
	if( fabs(e_vMax.y) < fabs(l_fPosY) )
		e_vMax.y = l_fPosY;
}

void	sGameData::Init()
{
	m_MonsterInfoMaxVector.clear();
	m_fMonsterInfoStartPosX = 0.f;
	m_fMonsterInfoStartPosY = 0.f;
	m_fMonsterInfoGapX = 100.f;
	m_fMonsterStatusGapX = 5;
	m_fShopStartPosX = 0.f;
	m_fShopStartPosY = 0.f;
	m_fShopGapX = 30.f;
	m_fEnemyStartPosX = 0.f;
	m_fEnemyStartPosY = 0.f;
	m_fEnemyGapX = 30.f;
	m_fEnemyStatusGapX = 30.f;
	m_fMonsterScale = 5.f;
	m_fShopScale = 3.f;
	m_fEnemyScale = 3.f;
	//========================================================================
	//
	//
	float	l_fStartPosX = m_fMonsterInfoStartPosX;
	float	l_fStartPosY = m_fMonsterInfoStartPosY;
	float	l_fGapX = m_fMonsterInfoGapX;

	for(size_t i=0;i<m_MonsterInfoVector.size();++i)
	{
		sMonsterInfo*l_pMonsterInfo = m_MonsterInfoVector[i];
		if( i == 0 )
		{
			for(size_t j=0;j<l_pMonsterInfo->m_AllRenderData.m_LineDataVector.size();++j)
				m_MonsterInfoMaxVector.push_back(Vector2(0,0));
		}
		l_fStartPosX = m_fMonsterInfoStartPosX;
		l_fStartPosY = m_fMonsterInfoStartPosY;
		for(size_t j=0;j<l_pMonsterInfo->m_LineDataVector.size();++j)
		{
			auto l_pLevelInfo = (sMonsterInfo::sLevelInfo*)l_pMonsterInfo->m_LineDataVector[j];
			int l_fDis = 0;//(int)(m_fMonsterStatusGapX*j);
			float l_fPosX = l_fStartPosX+l_fDis;
			float l_fPosY = (float)-l_pLevelInfo->GetHP()*m_fMonsterScale;
			Assign(l_fPosX,l_fPosY,l_pMonsterInfo->m_pHPData,m_MonsterInfoMaxVector[0]);
			l_fPosY = (float)-l_pLevelInfo->GetSTR()*m_fMonsterScale;
			Assign(l_fPosX,l_fPosY,l_pMonsterInfo->m_pSTRData,m_MonsterInfoMaxVector[1]);
			l_fPosY = (float)-l_pLevelInfo->GetStamina()*m_fMonsterScale;
			Assign(l_fPosX,l_fPosY,l_pMonsterInfo->m_pStaminaData,m_MonsterInfoMaxVector[2]);
			l_fStartPosX += l_fGapX;
		}
	}
	l_fStartPosX = m_fShopStartPosX;
	l_fStartPosY = m_fShopStartPosY;
	l_fGapX = m_fShopGapX;
	for(std::map<int,std::vector<sMonsterShop*>*>::iterator l_Iterator=m_MonsterShopDataMap.begin();
		l_Iterator != m_MonsterShopDataMap.end();l_Iterator++)
	{
		auto l_pVector = l_Iterator->second;
		for(size_t i=0;i<l_pVector->size();++i)
		{
			sMonsterShop*l_pMonsterShop = (sMonsterShop*)(*l_pVector)[i];
			std::map<std::wstring,float> l_Result = l_pMonsterShop->GetAttributeNameAndValue();
			for(auto l_Iterator : l_Result)
			{
				cChartWithName::sNameAndData*l_pNameAndData = new cChartWithName::sNameAndData();
				l_pNameAndData->m_Name = l_Iterator.first;
				l_fStartPosX += l_fGapX;
				l_pNameAndData->m_pLineData->AddData(Vector3(l_fStartPosX,0,0),0);
				l_pNameAndData->m_pLineData->AddData(Vector3(l_fStartPosX,l_Iterator.second*m_fShopScale,0),0);
				l_pMonsterShop->m_LineDataVector.push_back(l_pNameAndData);
			}
		}
	}
	l_fStartPosX = m_fEnemyStartPosX;
	l_fStartPosY = m_fEnemyStartPosY;
	l_fGapX = m_fEnemyGapX;
	for(size_t i=0;i<m_EnemyStatusVector.size();++i)
	{
		auto l_pEnemyStatus = m_EnemyStatusVector[i];
		size_t l_uiSize = l_pEnemyStatus->m_LineDataVector.size();
		for(auto l_pObject :l_pEnemyStatus->m_LineDataVector)
		{
			sEnemyStatus::sLevelInfo*l_pLevelInfo  = (sEnemyStatus::sLevelInfo*)l_pObject;
			int l_iGap = 0;
			l_pEnemyStatus->m_pHPData->AddData(Vector3(l_iGap++*m_fEnemyStatusGapX+l_fStartPosX,(float)l_pLevelInfo->GetHP()*m_fEnemyScale,0),0);
			l_pEnemyStatus->m_pSTRData->AddData(Vector3(l_iGap++*m_fEnemyStatusGapX+l_fStartPosX,(float)l_pLevelInfo->GetSTR()*m_fEnemyScale,0),0);
			l_pEnemyStatus->m_pTimeAdd->AddData(Vector3(l_iGap++*m_fEnemyStatusGapX+l_fStartPosX,(float)l_pLevelInfo->GetTimeAdd()*m_fEnemyScale,0),0);
			l_pEnemyStatus->m_pTimeMinus->AddData(Vector3(l_iGap++*m_fEnemyStatusGapX+l_fStartPosX,(float)l_pLevelInfo->GetTimeMinus()*m_fEnemyScale,0),0);
			l_pEnemyStatus->m_pSpeed->AddData(Vector3(l_iGap++*m_fEnemyStatusGapX+l_fStartPosX,(float)l_pLevelInfo->GetSpeed()*m_fEnemyScale,0),0);
			l_pEnemyStatus->m_pCloseSpeed->AddData(Vector3(l_iGap++*m_fEnemyStatusGapX+l_fStartPosX,(float)l_pLevelInfo->GetCloseSpeed()*m_fEnemyScale,0),0);
			l_pEnemyStatus->m_pSkillTimeReduce->AddData(Vector3(l_iGap++*m_fEnemyStatusGapX+l_fStartPosX,(float)l_pLevelInfo->GetSkillTimeReduce()*m_fEnemyScale,0),0);
			l_fStartPosX += l_fGapX+l_iGap;
		}
	}
}

void	sGameData::MonsterRender()
{
	m_iRenderMonsterFlasg = 1<<2;
	std::vector<int> l_RenderIndex = m_MonsterRenderIndex;
	if( l_RenderIndex.size() == 0 )
	{
		for(size_t i=0;i<m_MonsterInfoVector.size();++i)
			l_RenderIndex.push_back(i);
	}
	for(size_t i=0;i<l_RenderIndex.size();++i)
	{
		sMonsterInfo*l_pMonsterInfo = m_MonsterInfoVector[l_RenderIndex[i]];
		cGameApp::m_spGlyphFontRender->SetFontColor(g_vLineIndexAndColor[i]);
		cGameApp::RenderFont(-450,-350+i*50,l_pMonsterInfo->GetMonsterName());
		if( m_iRenderMonsterFlasg == 0 )
		{
			l_pMonsterInfo->m_pHPData->m_pLineData->DebugRender(false,false,g_vLineIndexAndColor[i]);
			l_pMonsterInfo->m_pSTRData->m_pLineData->DebugRender(false,false,g_vLineIndexAndColor[i]);
			l_pMonsterInfo->m_pStaminaData->m_pLineData->DebugRender(false,false,g_vLineIndexAndColor[i]);
		}
		else
		{
			for(size_t j=0;j<l_pMonsterInfo->m_AllRenderData.m_LineDataVector.size();++j)
			{
				if( m_iRenderMonsterFlasg & 1<<j )
				{
					if( i == 1 )
					{
						cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
						cGameApp::RenderFont(0,0-i*50,l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_Name.c_str());
						Vector2	l_vPos = m_MonsterInfoMaxVector[j];
						Vector2 l_XAxis[2] = {Vector2(0,0),Vector2(l_vPos.x,0)};
						Vector2 l_YAxis[2] = {Vector2(0,0),Vector2(0,l_vPos.y)};
						GLRender::RenderLine((float*)&l_XAxis,2,Vector4::One,2);
						GLRender::RenderLine((float*)&l_YAxis,2,Vector4::One,2);
						const int l_iStride = 5;
						int l_iStepValue = (int)(l_vPos.y/l_iStride);
						for(int k=0;k<l_iStride+1;++k)
						{
							int l_iValue = l_iStepValue*k;
							cGameApp::RenderFont(-100,k*l_iStepValue,ValueToStringW(-l_iValue/m_fMonsterScale));
						}
						size_t l_iXCount = l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_pLineData->Count();
						auto l_pVector = l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_pLineData->GetLinerDataVector();
						for(size_t k=0;k<l_iXCount;++k)
						{
							
							cGameApp::RenderFont((*l_pVector)[k].x,30.f,ValueToStringW(k));
						}
						m_fMonsterScale;
					}
					l_pMonsterInfo->m_AllRenderData.m_LineDataVector[j]->m_pLineData->DebugRender(false,false,g_vLineIndexAndColor[i]);
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
	for(std::map<int,std::vector<sMonsterShop*>*>::iterator l_Iterator = m_MonsterShopDataMap.begin();
		l_Iterator != m_MonsterShopDataMap.end();l_Iterator++)
	{
		auto l_pVector = l_Iterator->second;
		for(size_t i=0;i<l_pVector->size();++i)
		{
			sMonsterShop*l_pMonsterShop = (sMonsterShop*)(*l_pVector)[i];
			l_pMonsterShop->Render();
		}
	}
}
void	sGameData::ShopUpdate(float e_fElpaseTime)
{

}
void	sGameData::EnemyRender()
{
	for(size_t i=0;i<m_EnemyStatusVector.size();++i)
	{
		auto l_pEnemyStatus = m_EnemyStatusVector[i];
		l_pEnemyStatus->m_pHPData->DebugRender();
		l_pEnemyStatus->m_pSTRData->DebugRender();
		l_pEnemyStatus->m_pTimeAdd->DebugRender();
		l_pEnemyStatus->m_pTimeMinus->DebugRender();
		l_pEnemyStatus->m_pSpeed->DebugRender();
		l_pEnemyStatus->m_pCloseSpeed->DebugRender();
		l_pEnemyStatus->m_pSkillTimeReduce->DebugRender();
	}
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
	m_pGameData = new sGameData();
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
	ParseShopFile(l_strShopFileName.c_str());
	ParseMonsterFile(l_strMonsterFileName.c_str(),
		l_strMonsterLevelFileName.c_str(),
		l_strMonsterStaminaFileName.c_str(),
		l_strMonsterStregthFileName.c_str(),
		l_strMonsterHealthFileName.c_str());
	ParseNPCInfoFile(l_strNPCInfoFileName.c_str());
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

void	cGameData::Render()
{

}
void	cGameData::Update(float e_fElpaseTime)
{

}