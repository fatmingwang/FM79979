#include "stdafx.h"
#include "GameEffectBase.h"


class  :public cNamedTypedObjectVector<cGameEffectBase>, public cNodeISAX
{
	bool	GameEffectManager::MyParse(TiXmlElement*e_pRoot);
	bool	GameEffectManager::ProcessGameEffect(TiXmlElement*e_pRoot);
	std::list<GameEffectTypeRegister>				m_AllGameEffectRegisterList;
	//
	std::map<eGameEffectList, cWaitForFetchFunctionObjectList<cGameEffectBase*>*>	m_AllGameEffecBaseMap;
public:
	cGameEffectBase * Require(eGameEffectList e_eGameEffectList);
	void		BeforeRender();
	void		Render();
	void		LastRender();
};