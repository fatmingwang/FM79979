#pragma once

#include "PlayerBehavior.h"

class	cPlayerManager : public cNamedTypedObjectVector<cPlayerBehavior>
{
	std::map<int, std::vector<cMatrix44>>	m_IndexAndPlayerMatrixMap;
	void				ProcessPlayerCountAndPlayerPosMap(TiXmlElement*e_pTiXmlElement);
public:
	cPlayerManager();
	~cPlayerManager();

	void	ProcessSet(TiXmlElement* e_pElement);
	//
	void	Init(void);
	void	Update(float e_fElpaseTime);
	void	Render(void);
	void	MessageRender();
	void	Destroy(void);
	void	RenderMissileDirTex();
	bool	IsAllowToFireBullet(int e_iIndex, int e_iLimitCount);
};