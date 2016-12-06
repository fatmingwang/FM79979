#include "stdafx.h"
#include "StageParser.h"
#include "GameData.h"
#include <algorithm>

const float g_iMonsterSpeed = 8.f;
const int g_iObjectLineWidth = 280;

extern void	CameraOffset(Vector2 e_vMove,float e_fScale);

void	StroeAllAttribute(sXmlNode*e_pTrigger,const TiXmlElement*e_pRoot)
{
	auto l_Attribute = e_pRoot->FirstAttribute();
	while(l_Attribute)
	{
		e_pTrigger->strAttributeVector.push_back(l_Attribute->Name());
		e_pTrigger->strValueVector.push_back(l_Attribute->Value());
		l_Attribute = l_Attribute->Next();
	}
}


bool	cStageParser::MyParse(TiXmlElement*e_pRoot)
{
	m_GamePlayVector.clear();
	auto l_pElement = e_pRoot->FirstChildElement();
	while(l_pElement)
	{
		if( wcscmp(l_pElement->Value(),L"node")==0)
		{
			ParseNode(l_pElement);
		}
		l_pElement = l_pElement->NextSiblingElement();
	}
	return true;
}




bool	cStageParser::ParseNode(TiXmlElement*e_pRoot)
{
	sGameNode l_sGameNode;
	StroeAllAttribute(&l_sGameNode,e_pRoot);
	auto l_pElement = e_pRoot->FirstChildElement();
	while(l_pElement)
	{
		if( wcscmp(l_pElement->Value(),L"triggers")==0)
		{
			ParseTriggers(l_pElement,&l_sGameNode);
		}
		l_pElement = l_pElement->NextSiblingElement();
	}
	m_GamePlayVector.push_back(l_sGameNode);
	return true;
}

bool	cStageParser::ParseTriggers(TiXmlElement*e_pRoot,sGameNode*e_pGamePlay)
{
	sTriggers	l_Triggers;
	StroeAllAttribute(&l_Triggers,e_pRoot);
	auto l_pElement = e_pRoot->FirstChildElement();
	while(l_pElement)
	{
		if( wcscmp(l_pElement->Value(),L"trigger")==0)
		{
			std::wstring l_strGroupName = l_Triggers.getValue(L"groupName");
			e_pGamePlay->AddGroupType(l_strGroupName.c_str());
			ParseTrigger(l_pElement,&l_Triggers);
		}
		l_pElement = l_pElement->NextSiblingElement();
	}
	e_pGamePlay->m_TriggersVector.push_back(l_Triggers);
	return true;
}

bool	cStageParser::ParseTrigger(TiXmlElement*e_pRoot,sTriggers*e_pTriggers)
{
	sTrigger l_Trigger;
	StroeAllAttribute(&l_Trigger,e_pRoot);
	e_pTriggers->m_TriggerVector.push_back(l_Trigger);
	return true;
}

sXmlNode::sXmlNode(const TiXmlElement*e_pTiXmlElement)
{
	StroeAllAttribute(e_pTiXmlElement);
}

std::wstring	sXmlNode::getNameValue()
{
	if( strAttributeVector[0] == L"name" )
	{
		return strValueVector[0];
	}
	return L"";
}

std::wstring	sXmlNode::getValue(const wchar_t*e_str)
{
	//because some genus make somw word in first character be bigger case and some lower case
	//so I have to do this...its really qoo
	std::wstring l_str = e_str;
	std::transform(l_str.begin(), l_str.end(), l_str.begin(), ::tolower);
	for(size_t i=0;i<strAttributeVector.size();++i)
	{
		std::wstring l_str2 = strAttributeVector[i];
		std::transform(l_str2.begin(), l_str2.end(), l_str2.begin(), ::tolower);	
		if( l_str2 == l_str )
		{
			return strValueVector[i];
		}
	}
	return L"";
}

int	sXmlNode::getInt(const wchar_t*e_str)
{
	std::wstring l_str = this->getValue(e_str);
	if( l_str.length() > 0 )
	{
		int l_iValue = GetInt(l_str);
		return l_iValue;
	}
	return -1;
}

void	sXmlNode::Merge(TiXmlElement*e_pRoot)
{
	auto l_Attribute = e_pRoot->FirstAttribute();
	while(l_Attribute)
	{
		if(getValue(l_Attribute->Name()).length() == 0)
		{
			strAttributeVector.push_back(l_Attribute->Name());
			strValueVector.push_back(l_Attribute->Value());
		}
		l_Attribute = l_Attribute->Next();
	}
}

void	sXmlNode::StroeAllAttribute(const TiXmlElement*e_pelement)
{
	::StroeAllAttribute(this,e_pelement);
}

bool	sTrigger::IsCoin()
{
	std::wstring l_strObjectType = this->getValue(L"objectType");
	int	l_iID = GetInt(l_strObjectType);
	if( l_iID == 101 ||
		l_iID == 106 ||
		l_iID == 107 ||
		l_iID == 108 ||
		l_iID == 109 ||
		l_iID == 112 ||
		l_iID == 133 ||//randomly give 4-9
		l_iID == 145 ||
		l_iID == 175 )
		return true;
	return false;
}

int	sTrigger::GetObjectType()
{
	return this->getInt(L"objectType");
}

int	sTrigger::GetTrackID()
{
	std::wstring	l_str = getValue(L"trackId");
	if( l_str.length() > 0 )
	{
		return GetInt(l_str);
	}
	return -1;
}


float	sTrigger::GetInterval()
{
	float l_fInterval = 0.f;
	std::wstring l_strIntervale = this->getValue(L"intervalTime");
	if(l_strIntervale.length())
	{
		l_fInterval = GetFloat(l_strIntervale);
	}
	return l_fInterval;
}

int	sTrigger::GetLength()
{
	std::wstring l_str = this->getValue(L"length");
	if( l_str.length() )
	{
		int l_iLength = GetInt(l_str);
		return l_iLength;
	}
	return -1;
}

int	sTriggers::FindFinalLength()
{
	if( m_iLastObjectLength == 0 )
	{
		for(size_t i=0;i<m_TriggerVector.size();++i)
		{
			if(m_iLastObjectLength < m_TriggerVector[i].GetLength())
				m_iLastObjectLength = m_TriggerVector[i].GetLength();
		}
		m_iLastObjectLength += this->GetLength();

		//m_iCoinCanFetch;
		//m_iMonsterOnTheWay;

	}
	return m_iLastObjectLength;
}


bool	sTriggers::IsChaserTrigger()
{
	if(m_eTriggersGroupNameType == eTGT_HURDLE ||  m_eTriggersGroupNameType == eTGT_BREAKABLE)
		return false;
	return m_iIsTraceTrigger>0?true:false;
}


void	sTriggers::RenderChartByAttributeIndex(int e_iIndex)
{
	std::wstring l_strName = this->getNameValue();
	for(size_t i=0;i<m_TriggerVector.size();++i)
	{

	}
}

void	sTriggers::AssignData()
{
	if( m_eTriggersGroupNameType == eTGT_MAX )
	{
		int l_iSize = (int)m_TriggerVector.size();
		m_iPickableObjectCount = 0;
		m_iMonsterOnTheWay = 0;
		m_iHurdleOnTheWay = 0;
		m_iBreakableOnTheWay = 0;
		this->m_eTriggersGroupNameType = GetTriggersGroupNameType(GetGroupName());
		if( this->m_eTriggersGroupNameType == eTGT_PICK_UP )
		{
			m_iPickableObjectCount = l_iSize;
		}
		else
		if( this->m_eTriggersGroupNameType == eTGT_HURDLE )
		{
			m_iHurdleOnTheWay = l_iSize;
		}
		else
		if(eTGT_BREAKABLE == m_eTriggersGroupNameType)
		{
			m_iBreakableOnTheWay = l_iSize;
		}
		else
		if( this->m_eTriggersGroupNameType == eTGT_MONSTER || this->m_eTriggersGroupNameType == eTGT_FLIGHT || this->m_eTriggersGroupNameType == eTGT_BOSS )
		{
			m_iMonsterOnTheWay = l_iSize;
		}
		m_iIsTraceTrigger = GetInt(this->getValue(L"isChaserTrigger"));
		for(size_t i=0;i<this->m_TriggerVector.size();++i)
		{
			if( m_TriggerVector[i].IsCoin() )
			{
				++m_iCoinCount;
			}
		}
		
	}
}

int	sTriggers::GetPickupCount()
{
	AssignData();
	return m_iPickableObjectCount;
}
int	sTriggers::GetMonsterCount()
{
	AssignData();
	return m_iMonsterOnTheWay;
}

int	sTriggers::GetHurdleCount()
{
	AssignData();
	return m_iHurdleOnTheWay;
}
int	sTriggers::GetBreakableCount()
{
	AssignData();
	return m_iBreakableOnTheWay;
}

std::wstring	sTriggers::GetGroupName()
{
	return this->getValue(L"groupName");
}

void	sGameNode::PlayerGo()
{
	m_PlayerPos.Init();
}


bool	sGameNode::AddGroupType(const wchar_t*e_strName)
{
	std::map<std::wstring,int>::iterator i = m_GroupTypeMapl.begin();
	for(;i != m_GroupTypeMapl.end();++i)
	{
		if( i->first == e_strName )
		{
			++i->second;
			return true;
		}
	}
	m_GroupTypeMapl[e_strName] = 0;
	return true;
}

void	sGameNode::DumpyGroupTypeInfo(std::vector<std::wstring>*e_pNameVector,std::vector<int>*e_pCountVector)
{
	std::vector<std::wstring>	l_Result;
	std::map<std::wstring,int>::iterator i = m_GroupTypeMapl.begin();
	for(;i != m_GroupTypeMapl.end();++i)
	{
		e_pNameVector->push_back(i->first);
		e_pCountVector->push_back(i->second);
	}
}

int		sGameNode::FindFinalLength()
{
	if( m_iLastObjectLength == 0 )
	{
		for(size_t i=0;i<m_TriggersVector.size();++i)
		{
			if(m_iLastObjectLength < m_TriggersVector[i].FindFinalLength())
				m_iLastObjectLength = m_TriggersVector[i].FindFinalLength();
		}
		float l_EndTime = m_iLastObjectLength / g_iMonsterSpeed;
		m_PlayerPos.AddData(Vector3(0,0,0),0);
		m_PlayerPos.AddData(Vector3(0,(float)m_iLastObjectLength,0),l_EndTime);
		m_PlayerPos.SetLoop(true);

	}
	return m_iLastObjectLength;
}


Vector4 g_TriggersGroupNameAndColor[eTGT_MAX] = {
		Vector4(0.5f	,1.0f	,0.5f	,1.f),
		Vector4(1.f		,0.5f	,0.5f	,1.f),
		Vector4(1.f		,0.3f	,0.3f	,1.f),
		Vector4(1.f		,0.0f	,0.0f	,1.f),
		Vector4(1.f		,0.1f	,0.1f	,1.f),
		Vector4(0.25	,0.25f	,0.0f	,1.f),
};
eTriggersGroupNameType GetTriggersGroupNameType(std::wstring e_str)
{
	std::wstring l_str = e_str;
	std::transform(l_str.begin(), l_str.end(), l_str.begin(), ::tolower);

	if( l_str.find(MONSTER_NAME) != std::string::npos )
	{
		return eTGT_MONSTER;
	}
	else
	if( l_str.find(BREAKABLE_NAME) != std::string::npos )
	{
		return eTGT_BREAKABLE;
	}
	else
	if( l_str.find(HURDLE_NAME) != std::string::npos || l_str.find(HURDLE_NAME2) != std::string::npos )
	{
		return eTGT_HURDLE;
	}
	else
	if( l_str.find(PICKUP_NAME) != std::string::npos || l_str.find(PICKUP_NAME2) != std::string::npos || l_str.find(PICKUP_NAME3) != std::string::npos|| l_str.find(PICKUP_NAME4) != std::string::npos )
	{
		return eTGT_PICK_UP;
	}
	else
	if( l_str.find(FLIGHT_NAME) != std::string::npos || l_str.find(FLIGHT_NAME2) != std::string::npos)
	{
		return eTGT_FLIGHT;
	}
	else
	if( l_str.find(BOSS_NAME) != std::string::npos )
	{
		return eTGT_BOSS;	
	}
	cGameApp::OutputDebugInfoString(l_str);
	return eTGT_MAX;
}


Vector4	GetColorByGroupName(std::wstring e_str)
{
	eTriggersGroupNameType l_eTriggersGroupNameType = GetTriggersGroupNameType(e_str);
	if( l_eTriggersGroupNameType != eTGT_MAX)
	{
		return g_TriggersGroupNameAndColor[l_eTriggersGroupNameType];
	}
	return Vector4::One;
}


void	RenderColorHint(int e_iStartPosX,int e_iStartPosY)
{
	
	std::vector<std::wstring>	l_ColorAndName;
	l_ColorAndName.push_back(MONSTER_NAME);
	l_ColorAndName.push_back(BREAKABLE_NAME);
	l_ColorAndName.push_back(HURDLE_NAME);
	l_ColorAndName.push_back(PICKUP_NAME);
	l_ColorAndName.push_back(FLIGHT_NAME);
	l_ColorAndName.push_back(BOSS_NAME);
	for( size_t i=0;i<l_ColorAndName.size();++i )
	{
		Vector4	l_vColor = GetColorByGroupName(l_ColorAndName[i]);
		cGameApp::m_spGlyphFontRender->SetFontColor(l_vColor);
		cGameApp::RenderFont(e_iStartPosX,e_iStartPosY+i*30,l_ColorAndName[i]);
	}
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
}

Vector2	GetTrackStartEndPosX(int e_Width,sTrigger*e_pTrigger)
{
	const int l_TotalTrack = 5;
	int	l_iTrackID = e_pTrigger->GetTrackID();
	if( l_iTrackID == 4 )
		l_iTrackID = 0;
	else
	if( l_iTrackID == 3 )
		l_iTrackID = 1;
	else
	if( l_iTrackID == 1 )
		l_iTrackID = 3;
	else
	if( l_iTrackID == 0 )
		l_iTrackID = 4;
	//l_iTrackID = (l_TotalTrack-1)-l_iTrackID;
	int	l_iDivideWidth = e_Width / l_TotalTrack;
	Vector2 l_vWidth(l_iTrackID*l_iDivideWidth,l_iTrackID*l_iDivideWidth+l_iDivideWidth);
	return l_vWidth;
}


extern sGameData*g_pGameData;
int g_iStagePosXOffset = 0;
int g_iStagePosYOffset = 500;

int getCorrectLength(int e_iEnemySpeed,int e_iStartLength,int e_iEnemyLength)
{
	if( e_iEnemySpeed == -1 )
		return e_iEnemyLength;
	const  int l_iMonsterSpeed = 8;
	int l_iOffsetSpeed = e_iEnemySpeed-l_iMonsterSpeed;
	int l_iLengthOffset = e_iStartLength-e_iEnemyLength;
	float l_fSeconds = (float)l_iLengthOffset/(float)l_iOffsetSpeed;
	float l_fTwordLength = (l_iOffsetSpeed*l_fSeconds)+e_iStartLength;
	return (int)l_fTwordLength;
}

void		sGameNode::RenderScene()
{
	CameraOffset(Vector2(g_iStagePosXOffset,g_iStagePosYOffset),1);
	const int l_Value = 5;
	const float l_fMonsterSpeed = g_iMonsterSpeed;
	int	l_iHeight = (FindFinalLength()+50)*l_Value;

	RenderRectangle(Vector2::Zero,(float)g_iObjectLineWidth,(float)l_iHeight,Vector4::One,0);
	//object type and count
	std::map<int,int>	l_ObjectTypeAndName;
	int l_iNPVLevel = this->GetNPCLevel();
	for(size_t i=0;i<m_TriggersVector.size();++i)
	{
		sTriggers*l_pTriggers = &m_TriggersVector[i];
		float l_fInterval = l_pTriggers->GetInterval();
		std::wstring	l_strGroupName = l_pTriggers->GetGroupName();
		int	l_iTriggersLength = l_pTriggers->GetLength();
		std::vector<Vector2>	l_Pos;
		Vector4	l_vColor = GetColorByGroupName(l_pTriggers->GetGroupName());
		l_Pos.push_back(Vector2(0,l_iTriggersLength*l_Value));
		l_Pos.push_back(Vector2(g_iObjectLineWidth,l_iTriggersLength*l_Value));
		//GLRender::RenderLine(&l_Pos,Vector4::One);
		//cGameApp::RenderFont(g_iObjectLineWidth+30,l_iTriggersLength*l_Value,l_strGroupName);
		if( !l_pTriggers->IsChaserTrigger())
		{
			l_fInterval = 0.f;
		}
		for( size_t j=0;j<l_pTriggers->m_TriggerVector.size();++j )
		{
			sTrigger*l_pTrigger  = &l_pTriggers->m_TriggerVector[j];
			int l_Type = l_pTrigger->GetObjectType();
			Vector2 l_vShowWidth = GetTrackStartEndPosX(g_iObjectLineWidth,l_pTrigger);
			float l_fLength = (float)(l_pTrigger->GetLength()+l_iTriggersLength);
			if(g_pGameData && g_pGameData->GetEnemyStatus(l_Type) )
			{
				int l_iSpeed = g_pGameData->GetMonsterSpeedByLevel(l_pTrigger->GetObjectType(),l_iNPVLevel);
				l_fLength = (float)getCorrectLength(l_iSpeed,l_iTriggersLength,(int)l_fLength);
			}
			if( l_fInterval != 0.f )
			{
				l_fLength += (l_fInterval*j)*l_fMonsterSpeed;
			}
			l_Pos.clear();
			l_Pos.push_back(Vector2(l_vShowWidth.x,l_fLength*l_Value));
			l_Pos.push_back(Vector2(l_vShowWidth.y,l_fLength*l_Value));
			GLRender::RenderLine(&l_Pos,l_vColor);
			if( l_ObjectTypeAndName.find(l_Type) == l_ObjectTypeAndName.end())
			{
				l_ObjectTypeAndName.insert(std::make_pair(l_Type,1));
			}
			else
			{
				++l_ObjectTypeAndName[l_Type];
			}
			cGameApp::RenderFont(l_Pos[0].x,l_Pos[0].y-10,ValueToStringW(l_Type));
		}
	}
	int l_iIndex = 0;
	int l_iTotalEnemy = 0;
	for(auto l_Iterator : l_ObjectTypeAndName)
	{
		if( g_pGameData )
		{
			if(g_pGameData->GetEnemyStatus(l_Iterator.first) )
			{
				std::wstring l_str = ValueToStringW(l_Iterator.first);
				l_str += L":";
				l_str += ValueToStringW(l_Iterator.second);
				l_iTotalEnemy += l_Iterator.second;
				cGameApp::RenderFont(-150,l_iIndex*30+500,l_str.c_str());
				++l_iIndex;
			}
		}
		else
		{
			//cGameApp::RenderFont(-150,l_iIndex*100+500,l_Iterator.second.c_str());
			//++l_iIndex;
		}
	}
	if( l_iTotalEnemy > 0 )
	{
		cGameApp::RenderFont(-300,400,L"TotalEnemy");
		cGameApp::RenderFont(-250,430,ValueToStringW(l_iTotalEnemy));
	}
	else
	{
		cGameApp::RenderFont(-300,400,L"please load game data!");
	}
	RenderColorHint(-150,0);
	Vector3	l_vPos = m_PlayerPos.GetCurrentData();
	GLRender::RenderSphere(Vector2(l_vPos.x,l_vPos.y*l_Value),3.14f);
	std::wstring l_strNPVLevel = L"NPCLevel";
	l_strNPVLevel += ValueToStringW(l_iNPVLevel);
	cGameApp::RenderFont(-200,450,l_strNPVLevel.c_str());
	if( m_HitTriggerName.length() > 0 )
	{
		cGameApp::RenderFont(m_ShowHitPos.x,m_ShowHitPos.y-20,m_HitTriggerName);
	}
}

void	sGameNode::Update(float e_fElpaseTime)
{
	m_PlayerPos.Update(e_fElpaseTime);
}

int		sGameNode::GetPickupCount()
{
	AssignData();
	return m_iPickableObjectCount;
}
int		sGameNode::GetMonsterCount()
{
	AssignData();
	return m_iMonsterOnTheWay;
}
int		sGameNode::GetHurdleAndBreakableCount()
{
	AssignData();
	return m_iHurdleOnTheWay+m_iBreakableOnTheWay;
}

void	sGameNode::AssignData()
{
	if(m_iPickableObjectCount+	m_iMonsterOnTheWay+m_iHurdleOnTheWay+m_iBreakableOnTheWay <= 0)
	{
		for(size_t i=0;i<m_TriggersVector.size();++i)
		{
			m_iPickableObjectCount += m_TriggersVector[i].GetPickupCount();
			m_iMonsterOnTheWay += m_TriggersVector[i].GetMonsterCount();
			m_iHurdleOnTheWay += m_TriggersVector[i].GetHurdleCount();
			m_iBreakableOnTheWay += m_TriggersVector[i].GetBreakableCount();
			m_iCoinCount += m_TriggersVector[i].m_iCoinCount;
		}
	}
}

float	sGameNode::GetFinishTime()
{
	return FindFinalLength()/g_iMonsterSpeed+5.f;
}



bool	sGameNode::MouseMove(int e_iMousePosX,int e_iMousePosY)
{
	e_iMousePosX += g_iStagePosXOffset;
	e_iMousePosY += g_iStagePosYOffset;

	const int l_Value = 5;
	const float l_fMonsterSpeed = g_iMonsterSpeed;
	int	l_iHeight = (FindFinalLength()+50)*l_Value;
	if( e_iMousePosX <0 || e_iMousePosX>g_iObjectLineWidth )
		return false;
	if( e_iMousePosY <0 || e_iMousePosY>l_iHeight )
		return false;
	int l_iNPVLevel = this->GetNPCLevel();
	for(size_t i=0;i<m_TriggersVector.size();++i)
	{
		sTriggers*l_pTriggers = &m_TriggersVector[i];
		float l_fInterval = l_pTriggers->GetInterval();
		int	l_iTriggersLength = l_pTriggers->GetLength();
		std::vector<Vector2>	l_Pos;
		if( !l_pTriggers->IsChaserTrigger())
			l_fInterval = 0.f;
		for( size_t j=0;j<l_pTriggers->m_TriggerVector.size();++j )
		{
			sTrigger*l_pTrigger  = &l_pTriggers->m_TriggerVector[j];
			int l_Type = l_pTrigger->GetObjectType();
			Vector2 l_vShowWidth = GetTrackStartEndPosX(g_iObjectLineWidth,l_pTrigger);
			float l_fLength = (float)(l_pTrigger->GetLength()+l_iTriggersLength);
			if(g_pGameData && g_pGameData->GetEnemyStatus(l_Type) )
			{
				int l_iSpeed = g_pGameData->GetMonsterSpeedByLevel(l_pTrigger->GetObjectType(),l_iNPVLevel);
				l_fLength = (float)getCorrectLength(l_iSpeed,l_iTriggersLength,(int)l_fLength);
			}
			if( l_fInterval != 0.f )
			{
				l_fLength += (l_fInterval*j)*l_fMonsterSpeed;
			}

			Vector2 l_PosLeft(l_vShowWidth.x,l_fLength*l_Value);
			Vector2 l_PosRight(l_vShowWidth.y,l_fLength*l_Value);
			if( l_PosLeft.x <= e_iMousePosX &&
				l_PosRight.x >= e_iMousePosX)
			{
				if(l_PosLeft.y-5<=e_iMousePosY &&
					l_PosLeft.y+5>=e_iMousePosY)
				{
					std::wstring	l_strGroupName = l_pTriggers->GetGroupName();
					m_HitTriggerName = l_strGroupName;
					m_HitTriggerName += L",index:";
					m_HitTriggerName += l_pTrigger->getValue(L"TriggerIndex");
					m_HitTriggerName += L"\n";
					m_HitTriggerName += L"length";
					m_HitTriggerName += ValueToStringW(l_fLength);
					m_ShowHitPos.x = e_iMousePosX;
					m_ShowHitPos.y = e_iMousePosY;
					return true;
				}
			}
		}
	}
	m_HitTriggerName = L"";
	return false;
}
cStageParser*g_pStageParser = nullptr;
cStageParser::cStageParser()
{
	g_pStageParser = this;
}

cStageParser::~cStageParser()
{
	g_pStageParser = nullptr;
}

int	cStageParser::GetIndexByStageName(std::wstring e_strStageName)
{
	if( m_StageNameWithIndex.find(e_strStageName) == m_StageNameWithIndex.end() )
	{
		int l_iIndex = 0;
		for(auto l_piterator = m_GamePlayVector.begin();l_piterator != this->m_GamePlayVector.end();l_piterator++)
		{
			if(wcscmp(e_strStageName.c_str(),l_piterator->getValue(L"gameplayName").c_str()) == 0)
			{
				m_StageNameWithIndex[e_strStageName] = l_iIndex;
			}
			++l_iIndex;
		}
	}
	if( m_StageNameWithIndex.find(e_strStageName) == m_StageNameWithIndex.end() )
		return -1;
	return m_StageNameWithIndex[e_strStageName];
}

std::wstring	cStageParser::GetStageNameByIndex(int e_iIndex)
{
	return m_GamePlayVector[e_iIndex].getValue(L"gameplayName");
}