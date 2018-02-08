#pragma once
//
#include "NormalFish.h"

class cFixedFishMonster: public cNormalFish
{
	cNamedTypedObjectVector<cCurveWithTime>	m_CurveWithTimeVector;
private:
	void						ProcessPathFile(TiXmlElement*e_pTiXmlElement);
protected:
	virtual	void				InternalInit();
	virtual	void				InternalUpdate(float e_fElpaseTime);
public:
	//
	cFixedFishMonster();
	virtual ~cFixedFishMonster();
	cFixedFishMonster(cFixedFishMonster*e_pMonster);
	cFixedFishMonster*GetMe(TiXmlElement*e_pTiXmlElement);
};
