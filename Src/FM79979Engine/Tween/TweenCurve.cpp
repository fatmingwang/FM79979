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

cCurveWithTime* cTweenyCurveWithTime::GetCurve()
{
	return &m_CurveWithTime;
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
		[this, e_fDuration](float e_fValue)
		{
			float l_fEndTime = m_CurveWithTime.GetEndTime();
			this->m_CurveWithTime.UpdateByGlobalTime(e_fValue* l_fEndTime);
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
		m_CurveWithTimeKeyPosition = m_CurveWithTime.GetKeyPositionByTime();
	}
}

void cTweenyCurveWithTime::SetData(tweeny::easing::enumerated e_easing, float e_fDuration, cCurveWithTime* e_pCurveWithTime, std::function<void(unsigned int)>e_FinishFunction)
{
	this->SetTween(e_easing, e_fDuration, e_FinishFunction);
	this->SetCurve(e_pCurveWithTime);
}

void cTweenyCurveWithTime::Reset()
{
	m_CurveWithTime.Init();
	if (m_pTweenBinder)
	{
		m_pTweenBinder->Reset();
	}
}

void cTweenyCurveWithTime::Update(float e_fElpaseTime)
{
	if (m_pTweenBinder)
	{
		m_pTweenBinder->Update(e_fElpaseTime);
	}
}

void cTweenyCurveWithTime::Render(float e_fSphereRadius, Vector4 e_vSphereColor)
{
	this->m_CurveWithTime.Render();
	auto l_vPos = this->m_CurveWithTime.GetCurrentPosition();
	GLRender::RenderSphere(Vector2(l_vPos.x, l_vPos.y), e_fSphereRadius, e_vSphereColor);
	cGameApp::RenderFont(l_vPos.x, l_vPos.y, ValueToStringW(l_vPos).c_str());
	//auto l_Vector = this->m_CurveWithTime.GetOriginalPointList();
	auto l_Vector = m_CurveWithTimeKeyPosition;
	for (size_t i = 0; i < l_Vector.size(); ++i)
	{
		//if (i % l_ModulateInt == 0)
		{
			l_vPos = (l_Vector)[i];
			GLRender::RenderSphere(Vector2(l_vPos.x, l_vPos.y), 15, Vector4(1, 1, 0, 1));
		}
	}
	
}