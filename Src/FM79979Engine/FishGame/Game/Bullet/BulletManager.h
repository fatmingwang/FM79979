#pragma once

#include "Bullet.h"

class cBulletManager:public cNamedTypedObjectVector<cBullet>,public cNodeISAX
{
private:
	void						ProcessCollisionFile(TiXmlElement*e_pRoot);
	void						ProcessPlayerBullet(TiXmlElement*e_pRoot);
	virtual	bool				MyParse(TiXmlElement*e_pRoot);
	int							m_iNumWorking;
	//for update only
	cBullet**					m_ppWorkingBullet;
	//each play has its limit bullet count
	int							m_iEachPlayerLimitCount;
	//
	int							m_ibCollideSkipIndex;
public:
	bool						IsDoCollideSkip(int e_iPlayerIndex);
	cBulletManager();
	~cBulletManager();
	int							GetEachPlayerLimitCount(){return  m_iEachPlayerLimitCount;}
	//
	//each play has its own bullet limit.
	cBullet*					Require( int e_iPlayerIndex, int e_iLevel, int e_iPayRate);
	void						Init();
	void						Update(float e_fElpaseTime);
	void						Render();
	void						DebugRender();
	void						Destroy();
	void						Collide( cFishManager* e_pMonsterManager );
};