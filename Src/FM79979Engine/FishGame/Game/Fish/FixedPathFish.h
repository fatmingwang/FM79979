#pragma once

#include "FishBase.h"


class cFixedFishMonster: public cFishBase
{
	cNamedTypedObjectVector<cCurveWithTime>	m_CurveWithTimeVector;
	cCurveWithTime*							m_pCurrentPath;
private:
	void						ProcessPathFile(TiXmlElement*e_pTiXmlElement);
protected:
	virtual	void				InternalInit()override;
	virtual	void				InternalUpdate(float e_fElpaseTime)override;
	virtual	void				SetTransform()override;
public:
	//
	cFixedFishMonster();
	virtual ~cFixedFishMonster();
	cFixedFishMonster(cFixedFishMonster*e_pMonster);
	cFixedFishMonster*GetMe(TiXmlElement*e_pTiXmlElement);
	DEFINE_TYPE_INFO();
};
