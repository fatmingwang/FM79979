#pragma once
//
#include "FishBase.h"

class cNormalFish : public cFishBase
{
private:
	virtual	void				ProcessCollisionlData(TiXmlElement*e_pElement) override;
	virtual	void				InternalInit() override;
	virtual	void				InternalUpdate(float e_fElpaseTime)  override;
public:
	//
	cNormalFish();
	cNormalFish(cNormalFish*e_pMonsterBase);
	virtual ~cNormalFish();
	static cNormalFish*GetMe(TiXmlElement*e_pElement);
};
