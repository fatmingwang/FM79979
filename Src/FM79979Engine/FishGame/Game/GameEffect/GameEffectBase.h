#pragma once

#include "../Common/Common.h"

enum eGameEffectList
{
	eGEL_TEXT = 0,
	eGEL_MAX
};

#define	REGISTER_TO_MANGER(TYPE,MANAGER)

class cGameEffectBase :public cRenderObject
{
	eGameEffectList m_eGameEffectList;
	cGameEffectBase*	RegisterToGameEffectManager()
	{
	}
public:
	cGameEffectBase();
	virtual ~cGameEffectBase();
	virtual	void	Update(float e_fElpaseTime) = 0;
	virtual	void	Render() = 0;
};

typedef std::function<cGameEffectBase*(TiXmlElement*)> GameEffectTypeRegister;

class GameEffectManager:public cNamedTypedObjectVector<cGameEffectBase>,public cNodeISAX
{
	virtual	bool	MyParse(TiXmlElement*e_pRoot);
	bool	ProcessGameEffect(TiXmlElement*e_pRoot);
	std::list<GameEffectTypeRegister>				m_AllGameEffectRegisterList;
	//
	std::map<eGameEffectList, cWaitForFetchFunctionObjectList<cGameEffectBase*>*>	m_AllGameEffecBaseMap;
public:
	cGameEffectBase * Require(eGameEffectList e_eGameEffectList);
	void		BeforeRender();
	void		Render();
	void		LastRender();
};