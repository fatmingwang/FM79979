#pragma once
//
#include "FishBase.h"
//behavior is FAIMachine
class cFAIFish : public cFishBase
{
private:
	void						ProcessFAIMachineData(TiXmlElement*e_pTiXmlElement);
	cFAIMachine*				m_pFAIMachine;
	virtual	void				InternalInit() override;
	virtual	void				InternalUpdate(float e_fElpaseTime)  override;
public:
	//
	cFAIFish();
	cFAIFish(cFAIFish*e_pFAIFish);
	virtual ~cFAIFish();
	static cFAIFish*GetMe(TiXmlElement*e_pElement);
	DEFINE_TYPE_INFO();
	cFAIMachine*			GetAIMachine();
	void					SetAIMachine(cFAIMachine* e_pFAIMachine);
	virtual	void			DebugRender()override;
};
