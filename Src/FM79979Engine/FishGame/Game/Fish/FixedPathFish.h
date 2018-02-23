#pragma once

#include "FishBase.h"


class cFixedPathFish: public cFishBase
{
	cNamedTypedObjectVector<cCurveWithTime>	m_CurveWithTimeVector;
	cCurveWithTime*							m_pCurrentPath;
private:
	void						ProcessPathFile(TiXmlElement*e_pTiXmlElement);
protected:
	virtual	void				InternalInit()override;
	virtual	void				InternalUpdate(float e_fElpaseTime)override;
public:
	//
	cFixedPathFish();
	virtual ~cFixedPathFish();
	cFixedPathFish(cFixedPathFish*e_pMonster);
	static cFixedPathFish*GetMe(TiXmlElement*e_pTiXmlElement);
	DEFINE_TYPE_INFO();
};
