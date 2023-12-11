#pragma once

#include "../Core/AllCoreInclude.h"

template<class TYPE>struct sTweenBinder;

class cTweenyCurveWithTime :public NamedTypedObject
{
protected:
	std::vector<Vector3>	m_CurveWithTimeKeyPosition;
	//
	sTweenBinder<tweeny::tween<float>>*m_pTweenBinder = nullptr;
	cCurveWithTime	m_CurveWithTime;
public:
	cTweenyCurveWithTime();
	virtual ~cTweenyCurveWithTime();
	void	Update(float e_fElpaseTime);
	void	Render();
	//
	cCurveWithTime* GetCurve() { return &m_CurveWithTime; }
	void	SetTween(tweeny::easing::enumerated e_easing, float e_fDuration, std::function<void(unsigned int)>e_FinishFunction);
	void	SetCurve(cCurveWithTime* e_pCurveWithTime);
	void	SetData(tweeny::easing::enumerated e_easing, float e_fDuration, cCurveWithTime* e_pCurveWithTime, std::function<void(unsigned int)>e_FinishFunction);
	//
	void	Reset();
};