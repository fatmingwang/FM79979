#pragma once

enum eGameEffectType
{
	eGET_TEXT = 0,
	eGET_TEXT1,
	eGET_TEXT2,
	eGET_TEXT3,
	eGET_TEXT4,
	eGET_MAX
};

//#define	REGISTER_TO_MANGER(TYPE,MANAGER)

class cGameEffectBase :public cRenderObject
{
	eGameEffectType m_eGameEffectType;
	bool			m_bGameEffectDone;
	int				m_iRenderPirority;//0 is smallest
public:
	cGameEffectBase();
	virtual ~cGameEffectBase();
	//static cGameEffectBase*GetMe(TiXmlElement*e_pTiXmlElement);
	virtual	void	Update(float e_fElpaseTime) = 0;
	virtual	void	Render() = 0;
	bool			IsGameEffectDone(){return m_bGameEffectDone;}
};

class cGameEffectManager:public cNamedTypedObjectVector<cGameEffectBase>,public cNodeISAX,public cFishGameCommonRegisterManager<cGameEffectBase>
{
	virtual	bool	MyParse(TiXmlElement*e_pRoot);
	bool	ProcessGameEffect(TiXmlElement*e_pRoot);
	//
	std::map<eGameEffectType, cWaitForFetchFunctionObjectList<cGameEffectBase>*>	m_AllGameEffecBaseMap;
public:
	cGameEffectManager();
	virtual ~cGameEffectManager();
	cGameEffectBase * Require(eGameEffectType e_eGameEffectType);
	void		Update(float e_fElpaseTime);
	void		BeforeRender();
	void		Render();
	void		LastRender();
};


cGameEffectBase * RequireGameEffect(eGameEffectType e_eGameEffectType);