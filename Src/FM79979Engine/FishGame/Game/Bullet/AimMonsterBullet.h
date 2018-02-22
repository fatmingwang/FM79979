#pragma once

#include "Bullet.h"
//
class	cAimMonsterBullet :public cBaseBullet
{

public:
	cAimMonsterBullet( cMPDI*e_pBulletMPDI, cbtShapeCollision* e_pBulletCollision, float e_fSpeed,cMPDI* e_pExplosion);
	~cAimMonsterBullet();
	DEFINE_TYPE_INFO();
	//
	virtual	void			Init()override;
	virtual	void			Update(float e_fElpaseTime )override;
	virtual	void			Render()override;
	virtual	void			DebugRender()override;
	virtual	void			Destroy()override;
	virtual	bool			Collide(cbtShapeCollision*e_pbtShapeCollision)override;
};