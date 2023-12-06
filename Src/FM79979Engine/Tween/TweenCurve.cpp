#include "pch.h"
#include "./TweenyManager.h"
#include "InnerTweenBinder.h"
#include "TweenCurve.h"

cTweenyCurveWithTime::cTweenyCurveWithTime()
{
}

cTweenyCurveWithTime::~cTweenyCurveWithTime()
{
	SAFE_DELETE(m_pTweenBinder);
}

void cTweenyCurveWithTime::SetTween(tweeny::easing::enumerated e_easing, float e_fDuration, std::function<void(unsigned int)>e_FinishFunction)
{
	SAFE_DELETE(m_pTweenBinder);
	int l_iSteps = GetStepsByDuration(e_fDuration);
	auto l_Tween = tweeny::from(0.f).to(1.f).during(l_iSteps);
	auto l_uiID = RequireTweenUniqueID();
	m_pTweenBinder = new sTweenBinder<tweeny::tween<float>>(e_fDuration, l_iSteps, l_Tween, e_easing, l_uiID);
	m_pTweenBinder->m_Tween.onStep
	(
		[this](float e_fValue)
		{
			this->m_CurveWithTime.SetCurrentLERPTime(e_fValue);
			return false;
		}
	);
	m_pTweenBinder->m_FinishFunction = e_FinishFunction;
}

void cTweenyCurveWithTime::SetCurve(cCurveWithTime* e_pCurveWithTime)
{
	if (e_pCurveWithTime)
	{
		this->m_CurveWithTime = *e_pCurveWithTime;
		this->m_CurveWithTime.Init();
	}
}

void cTweenyCurveWithTime::SetData(tweeny::easing::enumerated e_easing, float e_fDuration, cCurveWithTime* e_pCurveWithTime, std::function<void(unsigned int)>e_FinishFunction)
{
	this->SetTween(e_easing, e_fDuration, e_FinishFunction);
	this->SetCurve(e_pCurveWithTime);
}

void cTweenyCurveWithTime::Update(float e_fElpaseTime)
{
	if (m_pTweenBinder)
	{
		m_pTweenBinder->Update(e_fElpaseTime);
	}
}

void cTweenyCurveWithTime::Render()
{
	this->m_CurveWithTime.Render();
}