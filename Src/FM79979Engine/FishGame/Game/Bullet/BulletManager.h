#pragma once

#include "Bullet.h"

class cFishManager;
class cBulletManager:public cNamedTypedObjectVector<cBaseBullet>,public cNodeISAX
{
private:
	void						ProcessCollisionFile(TiXmlElement*e_pRoot);
	void						ProcessPlayerBullet(TiXmlElement*e_pRoot);
	virtual	bool				MyParse(TiXmlElement*e_pRoot);
public:
	cBulletManager();
	~cBulletManager();
	//
	cBaseBullet*					Require( int e_iPlayerIndex, int e_iLevel, int e_iPayRate);
	void						Init();
	void						Update(float e_fElpaseTime);
	void						Render();
	void						DebugRender();
	void						Destroy();
	void						Collide( cFishManager* e_pMonsterManager );
};