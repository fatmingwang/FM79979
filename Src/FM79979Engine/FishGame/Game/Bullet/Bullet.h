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
class	cBullet:public Frame
{
private:
	float					m_fAngle;
	Vector3					m_vPos;
	//web
	bool					m_bDoCollided;
	cMPDI*					m_pWebMPDI;
	void					WebUpdate(float e_fElpaseTime);
	//bullet
	eBulletStatus			m_eBulletStatus;
	cbtShapeCollision*		m_pBulletCollision;
	cMPDI*					m_pBulletMPDI;
	Vector3					m_vDirection;
	float					m_fSpeed;
	void					SetDirection( Vector3 e_vDirection );
	void					BulletUpdate( float e_fElpaseTime );
	void					SetTransform();
	//how many will be paid
	int						m_iBetScore;
	//
public:
	//
	cBullet( cMPDI*e_pBulletMPDI, cbtShapeCollision* e_pBulletCollision, float e_fSpeed,cMPDI* e_pWeb);
	~cBullet(void);
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