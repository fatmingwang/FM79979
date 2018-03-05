#pragma once

#include "PlayerBehavior.h"

class	cPlayerManager : public cNamedTypedObjectVector<cPlayerBehavior>, public cSingltonTemplate<cPlayerManager>
{
	std::map<int, std::vector<cMatrix44>>	m_IndexAndPlayerMatrixMap;
	void				ProcessPlayerCountAndPlayerPosMap(TiXmlElement*e_pTiXmlElement);
	//
protected:
	cPlayerManager();
public:
	~cPlayerManager();
	void	ProcessSet(TiXmlElement* e_pElement);
	//
	void	Init();
	void	Update(float e_fElpaseTime);
	void	Render();
	void	MessageRender();
	void	Destroy();
	void	RenderMissileDirTex();
	bool	IsAllowToFireBullet(int e_iIndex, int e_iLimitCount);
};