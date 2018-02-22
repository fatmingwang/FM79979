#pragma once

#include "GameEffectBase.h"

class  cGameEffectWinMoneyNubmer :public cGameEffectBase
{

public:
	cGameEffectWinMoneyNubmer();
	virtual ~cGameEffectWinMoneyNubmer();
	static cGameEffectWinMoneyNubmer*GetMe(TiXmlElement*e_pTiXmlElement);

	virtual void Update(float e_fElpaseTime) override;
	virtual void Render() override;
};