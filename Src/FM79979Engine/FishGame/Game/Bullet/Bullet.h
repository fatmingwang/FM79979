#pragma once

enum	eBulletStatus
{
	eBS_MOVING = 0,
	eBS_HITTED,
	eBS_EXPLOSION,
	eBS_WAIT_FOR_FETCH,
	eBS_MAX,
};
//
class	cBaseBullet:public Frame
{
private:
	//web
	cMPDI*					m_pExplosionMPDI;
	cMPDI*					m_pBulletMPDI;
	void					ExplosionUpdate(float e_fElpaseTime);
	void					BulletUpdate(float e_fElpaseTime);
	//bullet
	eBulletStatus			m_eBulletStatus;
	cbtShapeCollision*		m_pBulletCollision;
	Vector3					m_vDirection;
	float					m_fSpeed;
	float					m_fAngle;
	Vector3					m_vPos;
	void					SetDirection( Vector3 e_vDirection );
	void					SetTransform();
public:
	//
	cBaseBullet( cMPDI*e_pBulletMPDI, cbtShapeCollision* e_pBulletCollision, float e_fSpeed,cMPDI* e_pExplosion);
	~cBaseBullet(void);
	//
	virtual	void			Init();
	virtual	void			Update(float e_fElpaseTime );
	virtual	void			Render();
	virtual	void			DebugRender();
	virtual	void			Destroy();
	//
	void					Fire(cPlayerBehaviorBase*e_pPlayer,Vector3 e_vStartPos,Vector3 e_vDirection);
	virtual	bool			Collide(cbtShapeCollision*e_pbtShapeCollision);
	int						GetBetScore(){return m_iBetScore;}
	eBulletStatus			GetBulletStatus(void){return m_eBulletStatus;}
	void					SetBulletStatus(eBulletStatus status);
	cPlayerBehaviorBase*	GetBulletOwner() { return m_pBulletOwner; }
};