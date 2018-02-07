#pragma once

enum eGameEffectType
{
	eGET_TEXT = 0,
	eGET_MAX
};

#define	REGISTER_TO_MANGER(TYPE,MANAGER)

class cGameEffectBase :public cRenderObject
{
	eGameEffectType m_eGameEffectType;
	bool			m_bGameEffectDone;
	int				m_iRenderPirority;//0 is smallest
public:
	cGameEffectBase();
	virtual ~cGameEffectBase();
	//static cGameEffectBase*GetMe(TiXmlElement*e_pTiXmlElement);
	virtual	void	Update(float e_fElpaseTime) {}
	virtual	void	Render() {}
	bool			IsGameEffectDone(){return m_bGameEffectDone;}
};

class cGameEffectManager:public cNamedTypedObjectVector<cGameEffectBase>,public cNodeISAX,public cFishGameCommonRegisterManager
{
	virtual	bool	MyParse(TiXmlElement*e_pRoot);
	bool	ProcessGameEffect(TiXmlElement*e_pRoot);
	//
	std::map<eGameEffectType, cWaitForFetchFunctionObjectList<cGameEffectBase>*>	m_AllGameEffecBaseMap;
	virtual void	AddCloneRegisterFunction() override;
public:
	cGameEffectManager();
	virtual ~cGameEffectManager();
	cGameEffectBase * Require(eGameEffectType e_eGameEffectType);
	void		Update(float e_fElpaseTime);
	void		BeforeRender();
	void		Render();
	void		LastRender();
};