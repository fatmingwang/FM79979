#pragma once
//
#include "NormalFish.h"

class cFixedMonsterMonster: public cNormalFish
{
	cNamedTypedObjectVector<cCurveWithTime>	m_CurveWithTimeVector;
private:
	void						ProcessPathFile(TiXmlElement*e_pTiXmlElement);
protected:
	virtual	void				InternalInit();
	virtual	void				InternalUpdate(float e_fElpaseTime);
public:
	//
	cFixedMonsterMonster();
	virtual ~cFixedMonsterMonster();
	cFixedMonsterMonster(cFixedMonsterMonster*e_pMonster);
	cFixedMonsterMonster*GetMe(TiXmlElement*e_pTiXmlElement);
};
