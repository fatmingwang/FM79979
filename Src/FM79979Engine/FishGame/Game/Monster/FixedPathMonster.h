#pragma once
//
#include "MonsterBase"

class cFixedMonsterMonster : public cMonsterBase
{
private:
	void						ProcessPathFile(TiXmlElement*e_pTiXmlElement);
protected:
	eBodyType					m_eBodyType;
	//
	UT::sTimeCounter			m_HittedHintTC;
	int							m_iID;
	float						m_fScale;
	float						m_fRadius;
	eMonsterStatus				m_eMonsterStatus;
	//internal 
	virtual	void				InternalInit() = 0;
	virtual	void				InternalUpdate(float e_fElpaseTime) = 0;
public:
	//
	cMonster();
	virtual ~cMonster();
	cMonster(cMonster*e_pMonster);	
	eBodyType					GetBodyType();
	float						GetRadius() { return m_fRadius; }
	int							GetID() { return m_iID; }
	void						Init();
	virtual	void				Update(float e_fElpaseTime);
	virtual	bool				IsCollide(cbtShapeCollision*e_pbtShapeCollision) = 0;
	//while monster status is wait for clear or control by mini game cannot be died
	bool						IsStatusAllowToDied();
	float						GetScale() { return m_fScale; }
};
