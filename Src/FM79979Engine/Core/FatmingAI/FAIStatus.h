#ifndef _FAI_STATUS_H_
#define _FAI_STATUS_H_
namespace FATMING_AI
{
	enum	eCharacterAttribute
	{
		eCA_HP = 0,
		eCA_MP,
		eCA_DEF,
		eCA_STR,
		eCA_AGI,
		eCA_DEX,
		eCA_INT,
		eCA_SPEED,
		eCA_TORQUE,
		eCA_UNKNOW,
	};

	enum	eFAICharacterBehaviorStatus
	{
		eFAI_CBS_IDLE = 0,
		eFAI_CBS_INTO_SCENE,
		eFAI_CBS_HURT,
		eFAI_CBS_MOVING,
		eFAI_CBS_ATTACK,
		eFAI_CBS_SPECIAL_ATTACK,
		eFAI_CBS_DYING,
		eFAI_CBS_DIED_SHOW,
		eFAI_CBS_MAX,
	};

	enum	eFAICharacterBehaviorSpecialStatus
	{
		eFAI_CBSS_POSION = 0,
		eFAI_CBSS_TIME_LOCK,
		eFAI_CBSS_POWER_UP,
		eFAI_CBSS_MAX
	};

	static const wchar_t*	g_strCharacterAttribute[eCA_UNKNOW+1] = {
		L"HP",
		L"MP",
		L"DEF",
		L"STR",
		L"AGI",
		L"DEX",
		L"INT",
		L"SPEED",
		L"TORQUE",
		L"Unknow!!",
	};

	static const wchar_t*	g_strCharacterStatus[eFAI_CBS_MAX+1] = {
		L"Idle",
		L"IntoScene",
		L"Hurt",
		L"Moving",
		L"Attack",
		L"SPecialAttack",
		L"Dying",
		L"DiedShow",
		L"Unknow!!",
	};
	//
	eFAICharacterBehaviorStatus	GetCharacterBehaviodStatus(const WCHAR*e_strName);
	//it could only has 2 data just for UI show or just for data without ui info depend on the situchation
	struct	sCharacterAttributeAndUIInfo
	{
		eCharacterAttribute	CharacterAttribute;
		float				fValue;
		Vector2				vUIPos;
	};
	//
	std::vector<sCharacterAttributeAndUIInfo>	ParseAnimalStatusAndUIInfo(char*e_strFileName);
	//
	sCharacterAttributeAndUIInfo*	GetCharacterAttributeAndUIInfo(std::vector<sCharacterAttributeAndUIInfo>*e_pCharacterAttributeAndUIInfoVector,eCharacterAttribute	e_CharacterAttribute,int*e_piReceivedIndex);
	//
	class	cBattlePowerTargetValue:public NamedTypedObject
	{
		float			m_fValue;
		float			m_fTargetValue;
		//description about how this power works
		std::wstring*	m_pstrDescription;
	public:
		DEFINE_TYPE_INFO();
		cBattlePowerTargetValue(TiXmlElement*e_pTiXmlElement);
		cBattlePowerTargetValue(cBattlePowerTargetValue*e_pPowerTargetValue);
		CLONE_MYSELF(cBattlePowerTargetValue);
		virtual ~cBattlePowerTargetValue();
		void			AddValue(float e_fValue);
		bool			IsPowerFull();
		float			GetValue();
		float			SetValue();
		float			GetTargetValue();
		void			SetTargetValue(float e_fTargetValue);
		float			CurrentPercent();
		const WCHAR*	GetDescription();
	};
	//
	class	cBattlePowerList:public cNamedTypedObjectVector<cBattlePowerTargetValue>
	{
	public:
		DEFINE_TYPE_INFO();
		cBattlePowerList(TiXmlElement*e_pTiXmlElement);
		cBattlePowerList(cBattlePowerList*e_pPowerList);
		virtual ~cBattlePowerList();
		CLONE_MYSELF(cBattlePowerList);
		bool	IsPowerFull(int e_iIndex);
	};
	//
    class	cFAICharacterStatus:public NamedTypedObject
    {
		eFAICharacterBehaviorStatus	m_eFAICharacterBehaviorStatus;
		//if lock is true while character status change is call it won't change its status but force is allow to do it
		//some action must wait it is done
		bool						m_bFAICharacterBehaviorStatusLock[eFAI_CBS_MAX];
    public:
		cBattlePowerList*	m_pPowerList;
	    int			m_iHP;
	    int			m_iStrgeth;
	    int			m_iDefence;
	    int			m_iAgile;
	    int			m_iDexterous;
        int			m_iLucky;
    public:
        DEFINE_TYPE_INFO();
		cFAICharacterStatus();
	    cFAICharacterStatus(TiXmlElement*e_pElement);
	    cFAICharacterStatus(cFAICharacterStatus*e_pStatus);
		virtual ~cFAICharacterStatus();
		CLONE_MYSELF(cFAICharacterStatus);
	    cFAICharacterStatus*GetStatus(){ return this; }
		//if force is true m_bCharacterBehaviorStatusLock will be ignore
		bool						SetChracterBehaviorStatus(eFAICharacterBehaviorStatus e_eFAICharacterBehaviorStatus,bool e_bForce);
		eFAICharacterBehaviorStatus	GetFAICharacterBehaviorStatus(){return	m_eFAICharacterBehaviorStatus;}
		//if animation is don or behavior is done it will return true,override it if AI is complicated.
		bool						IsCharacterBehaviorStatusLock(){return	m_bFAICharacterBehaviorStatusLock[m_eFAICharacterBehaviorStatus];}
		bool						IsCharacterBehaviorStatusLock(eFAICharacterBehaviorStatus	e_eFAICharacterBehaviorStatus){return	m_bFAICharacterBehaviorStatusLock[e_eFAICharacterBehaviorStatus];}
		void						RenderStatusInfo(int e_iPosX,int e_iPosY);
    };
//end namespace FATMING_AI
}
#endif