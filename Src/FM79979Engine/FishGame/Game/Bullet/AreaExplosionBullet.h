#pragma once

#include "Bullet.h"
//
class	cAreaExplosionBullet:public cBaseBullet
{
public:
	//
	cAreaExplosionBullet( cMPDI*e_pBulletMPDI, cbtShapeCollision* e_pBulletCollision, float e_fSpeed,cMPDI* e_pExplosion);
	~cAreaExplosionBullet();
	DEFINE_TYPE_INFO();
	virtual	bool			Collide(cbtShapeCollision*e_pbtShapeCollision);
};