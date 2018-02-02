#pragma once
//
enum	eMonsterStatus
{
	eMS_NONE = 0,				//not on the scene
	eMS_ALIVE,					//on the scene,do AIMachine
	eMS_HITTED,					//probability give
	eMS_STRUGGLE,				//while fish is hitted it will has a fake probability to play this
	eMS_DYING,					//hitted play died animation
	eMS_DIED_SHOW,				//
	eMS_WAITING_FOR_CLEAN,		//
	eMS_CONTROL_BY_MINI_GAME,	//
	eMS_TRANSFORM_SHOW,
	eMS_MAX,

};

enum	eBodyType
{
	eBT_Small,
	eBT_Medium,
	eBT_Big,
	eBT_Special,
	eBT_Total,
};

eBodyType	GetBodyType(const wchar_t *e_str);

class cMonster : public Frame
{
	//
private:
	virtual	void				ProcessPhysicalData(TiXmlElement*e_pElement) = 0;
	void						ProcessMonsterData(TiXmlElement*e_pTiXmlElement);
	//
	eBodyType					m_eBodyType;
	eBodyType					GetBodyType();
protected:
	//how long s it alive
	float						m_fAliveTime;
	//
	UT::sTimeCounter			m_HittedHintTC;
	//for out of screen test
	float						m_fRadius;
	eMonsterStatus				m_eMonsterStatus;
	//internal 
	virtual	void				InternalInit() = 0;
	virtual	void				InternalUpdate(float e_fElpaseTime) = 0;
	int							m_iID;
	float						m_fScale;
public:
	//
	cMonster();
	virtual ~cMonster();
	cMonster(cMonster*e_pMonster);	
	float						GetRadius() { return m_fRadius; }
	int							GetID() { return m_iID; }
	void						Init();
	virtual	void				Update(float e_fElpaseTime);
	virtual	bool				IsCollide(cbtShapeCollision*e_pbtShapeCollision) = 0;
	virtual	void				DebugRender() {}
	virtual	void				SetPath() = 0;
	virtual	void				SetPath(cCurveWithTime* e_pCurveWithTime) = 0;
	//while monster status is wait for clear or control by mini game cannot be died
	bool						IsStatusAllowToDied();
	float						GetScale() { return m_fScale; }
};
