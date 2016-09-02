#include <vector>

#define	MONSTER_NAME		L"npc"
#define	BREAKABLE_NAME		L"break"
//#define	HURDLE_NAME			L"herdle"
#define	HURDLE_NAME			L"hurdle"
#define	HURDLE_NAME2		L"hurdl"
#define	PICKUP_NAME			L"pickup"
#define	PICKUP_NAME2		L"picup"
#define	PICKUP_NAME3		L"pcikup"
#define	PICKUP_NAME4		L"coin"
#define	FLIGHT_NAME			L"flight"
#define	FLIGHT_NAME2		L"fight"
#define	BOSS_NAME			L"boss"

#define	GET_FLOAT_DATA_FROM_ATTRIBUTE(VariableName,Attribute)\
private:	float m_f##VariableName;\
public: float	Get##VariableName()\
{\
	if( m_f##VariableName == -1.f )\
	{\
		std::wstring l_strValue = this->getValue(Attribute);\
		if( l_strValue.length() > 0 )\
		{\
			m_f##VariableName = GetFloat(l_strValue);\
		}\
	}\
	return m_f##VariableName;\
}


#define	GET_INT_DATA_FROM_ATTRIBUTE(VariableName,Attribute)\
private:	int m_i##VariableName;\
public: int	Get##VariableName()\
{\
	if( m_i##VariableName == -1 )\
	{\
		std::wstring l_strValue = this->getValue(Attribute);\
		if( l_strValue.length() > 0 )\
		{\
			m_i##VariableName = GetInt(l_strValue);\
		}\
	}\
	return m_i##VariableName;\
}

enum eTriggersGroupNameType
{
	eTGT_PICK_UP = 0,
	eTGT_HURDLE,
	eTGT_BREAKABLE,
	eTGT_MONSTER,
	eTGT_FLIGHT,
	eTGT_BOSS,
	eTGT_MAX
};

extern Vector4					g_TriggersGroupNameAndColor[eTGT_MAX];

eTriggersGroupNameType GetTriggersGroupNameType(std::wstring e_str);

struct sXmlNode
{
	std::vector<std::wstring> strAttributeVector;
	std::vector<std::wstring> strValueVector;
	std::wstring	getNameValue();
	std::wstring	getValue(const wchar_t*e_str);
	int				getInt(const wchar_t*e_str);
	void			StroeAllAttribute(TiXmlElement*e_pRoot);
	//if same attribute new one will be ingore
	void			Merge(TiXmlElement*e_pRoot);
};

struct sTrigger:public sXmlNode
{

	int				GetTrackIndex();
	int				GetLength();
	float			GetInterval();
	int				GetTrackID();
	bool			IsCoin();
};

struct sTriggers:public sTrigger
{
	int									m_iCoinCount;
	int									m_iLastObjectLength;
	int									m_iPickableObjectCount;
	int									m_iMonsterOnTheWay;
	int									m_iHurdleOnTheWay;
	int									m_iBreakableOnTheWay;
	int									m_iIsTraceTrigger;
	eTriggersGroupNameType				m_eTriggersGroupNameType;
	sTriggers(){ m_iPickableObjectCount = -1;m_iMonsterOnTheWay = -1; m_iLastObjectLength = 0;m_iHurdleOnTheWay = -1;m_iBreakableOnTheWay = -1;m_eTriggersGroupNameType = eTGT_MAX;m_iIsTraceTrigger = -1;m_iCoinCount = 0;}
	//
	std::vector<sTrigger>				m_TriggerVector;
	std::wstring						GetGroupName();
	int									FindFinalLength();
	int									GetPickupCount();
	int									GetMonsterCount();
	int									GetHurdleCount();
	int									GetBreakableCount();
	void								AssignData();
	bool								IsChaserTrigger();

	void								RenderChartByAttributeIndex(int e_iIndex);
};

struct sGameNode:public sTrigger
{
	std::wstring						m_HitTriggerName;
	cLinerDataProcessor<Vector3>		m_PlayerPos;
	int									m_iCoinCount;
	int									m_iLastObjectLength;
	int									m_iPickableObjectCount;
	int									m_iMonsterOnTheWay;
	int									m_iHurdleOnTheWay;
	int									m_iBreakableOnTheWay;
	POINT								m_ShowHitPos;
	sGameNode(){ m_iPickableObjectCount = 0;m_iMonsterOnTheWay = 0; m_iLastObjectLength = 0; m_iHurdleOnTheWay = 0;m_iBreakableOnTheWay = 0;m_iCoinCount = 0;}
	//
	std::vector<sTriggers>				m_TriggersVector;
	std::map<std::wstring,int>			m_GroupTypeMapl;
	void								PlayerGo();
	bool								AddGroupType(const wchar_t*e_strName);
	void								DumpyGroupTypeInfo(std::vector<std::wstring>*e_pNameVector,std::vector<int>*e_pCountVector);
	int									FindFinalLength();
	void								RenderScene();
	void								Update(float e_fElpaseTime);
	int									GetPickupCount();
	int									GetMonsterCount();
	int									GetHurdleAndBreakableCount();
	void								AssignData();
	float								GetFinishTime();
	bool								MouseMove(int e_iMousePosX,int e_iMousePosY);
};


class cStageParser:public cNodeISAX
{
	bool			ParseNode(TiXmlElement*e_pRoot);
	bool			ParseTriggers(TiXmlElement*e_pRoot,sGameNode*e_pGamePlay);
	bool			ParseTrigger(TiXmlElement*e_pRoot,sTriggers*e_pTriggers);
	virtual	bool	MyParse(TiXmlElement*e_pRoot);
public:
	std::vector<sGameNode>	m_GamePlayVector;
	//you should overwrite this if u call ParseWithMyParse
};