#include "pch.h"
#include "./TweenyManager.h"
#include "InnerTweenBinder.h"
#include "TweenCurve.h"
#include "../Core/GameplayUT/Render/Texture/png/pngLoader.h"

cTweenyCurveWithTime::cTweenyCurveWithTime(float e_fTimeForCheckCollision,float e_fTimeForRenderHint, const char* e_strDebugLineFileName)
{
	m_fTimeForCheckCollision = e_fTimeForCheckCollision;
	m_fTimeForRenderHint = e_fTimeForRenderHint;
	if (e_strDebugLineFileName)
	{
		m_pDeugLineImage = new cBaseImage(e_strDebugLineFileName);
	}
}

cTweenyCurveWithTime::~cTweenyCurveWithTime()
{
	SAFE_DELETE(m_pDeugLineImage);
	SAFE_DELETE(m_pCurveWithTime);
	SAFE_DELETE(m_pTweenBinder);
}

cCurveWithTime* cTweenyCurveWithTime::GetCurve()
{
	return m_pCurveWithTime;
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
			if (m_pCurveWithTime)
			{
				float l_fEndTime = m_pCurveWithTime->GetEndTime();
				float l_fTargetTime = e_fValue * l_fEndTime;
				this->m_pCurveWithTime->UpdateByGlobalTime(l_fTargetTime);
			}
			return false;
		}
	);
	m_pTweenBinder->m_FinishFunction = e_FinishFunction;
}

void cTweenyCurveWithTime::UpdateHintInfo(float e_fCurveCurrentTime)
{
	float l_fMinimumTimeForRender = e_fCurveCurrentTime - m_fTimeForRenderHint;
	float l_fMaximumTimeForRender = e_fCurveCurrentTime + m_fTimeForRenderHint;
	float l_fMinimumTimeForCollision = e_fCurveCurrentTime - m_fTimeForCheckCollision;
	float l_fMaximumTimeForCollision = e_fCurveCurrentTime + m_fTimeForCheckCollision;
	m_RenderHintPointIndexOfCurveAndTimeLerpHintMap.clear();
	m_CollisionPointIndexOfCurveAndTimeLerpHintMap.clear();
	if (m_pCurveWithTime)
	{
		auto l_TimeVector = this->m_pCurveWithTime->GetOriginalTimeList();
		int l_iIndex = 0;
		for (auto l_fTime : l_TimeVector)
		{
			if (m_iIgnoreAfterIndexForCollisionAndRender < l_iIndex)
			{
				float l_fTimeDiff = l_fTime - e_fCurveCurrentTime;
				if (l_fTime >= l_fMinimumTimeForRender && l_fTime < l_fMaximumTimeForRender && l_fTimeDiff > 0.f)
				{
					m_RenderHintPointIndexOfCurveAndTimeLerpHintMap.insert({ l_iIndex ,1.f - (l_fTimeDiff / this->m_fTimeForRenderHint) });
				}
				if (l_fTime >= l_fMinimumTimeForCollision && l_fTime <= l_fMaximumTimeForCollision)
				{
					m_CollisionPointIndexOfCurveAndTimeLerpHintMap.insert({ l_iIndex ,1.f - (fabs(l_fTimeDiff) / this->m_fTimeForCheckCollision) });
				}
			}
			++l_iIndex;
		}
	}
}

void cTweenyCurveWithTime::SetCurve(cCurveWithTime* e_pCurveWithTime)
{
	if (e_pCurveWithTime)
	{
		SAFE_DELETE(m_pCurveWithTime);
		//this->m_pCurveWithTime = new cCurveWithTime(e_pCurveWithTime);
		this->m_pCurveWithTime = e_pCurveWithTime;
		this->m_pCurveWithTime->Init();
		m_CurveWithTimeKeyPosition = m_pCurveWithTime->GetKeyPositionByTime();
	}
}

void cTweenyCurveWithTime::SetData(tweeny::easing::enumerated e_easing, float e_fDuration, cCurveWithTime* e_pCurveWithTime, std::function<void(unsigned int)>e_FinishFunction, float e_fDelayToStsartTime)
{
	this->SetTween(e_easing, e_fDuration, e_FinishFunction);
	this->SetCurve(e_pCurveWithTime);
	m_DelayTC.SetTargetTime(e_fDelayToStsartTime);
}

void cTweenyCurveWithTime::Reset()
{
	m_DelayTC.Start();
	m_iIgnoreAfterIndexForCollisionAndRender = -1;
	if (m_pCurveWithTime)
	{
		m_pCurveWithTime->Init();
	}
	if (m_pTweenBinder)
	{
		m_pTweenBinder->Reset();
	}
}

void cTweenyCurveWithTime::Update(float e_fElpaseTime)
{
	if (!m_DelayTC.bTragetTimrReached)
	{
		m_DelayTC.Update(e_fElpaseTime);
	}
	if (m_pTweenBinder&& m_DelayTC.bTragetTimrReached)
	{
		//this->SetVisible(true);
		//if wan to test override as 1
#ifdef DEBUG
		if (0)
		{
			e_fElpaseTime = 0.016f;
		}
#endif
		m_pTweenBinder->Update(e_fElpaseTime);
		if (m_pCurveWithTime)
		{
			float l_fTargetTime = m_pCurveWithTime->GetCurrentTime();
			UpdateHintInfo(l_fTargetTime);
		}
	}
	else
	{
		//this->SetVisible(false);
	}
}

void cTweenyCurveWithTime::Render()
{
	if (!m_DelayTC.bTragetTimrReached)
	{//do extend curve hint
		if (m_pDeugLineImage && m_pCurveWithTime)
		{
			if (m_DelayTC.fRestTime < 4.f)
			{
				float l_fTargetTime = (4 - m_DelayTC.fRestTime)+ m_DelayTC.fTargetTime;
				int l_iNumVertices = 0;
				const int l_ciTest = 9999;
				Vector3 l_OutPos[l_ciTest];
				Vector2 l_vOutUV[l_ciTest];
				Vector4 l_vOutColor[l_ciTest];
				m_pDeugLineImage->GenerateCurveTriangulatorRenderDataForBatchRendering(m_pCurveWithTime, l_fTargetTime, l_iNumVertices, l_OutPos, l_vOutUV, l_vOutColor, true);
			}

		}
	}
	else
	{
		this->Render2();
	}

}

void cTweenyCurveWithTime::Render2(float e_fSphereRadius, Vector4 e_vSphereColor)
{
	if (m_pCurveWithTime)
	{
		this->m_pCurveWithTime->Render();
		auto l_vPos = this->m_pCurveWithTime->GetCurrentPosition();
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
	float l_fRadius = 20.f;
	for (auto l_Iterator = m_RenderHintPointIndexOfCurveAndTimeLerpHintMap.begin(); l_Iterator != m_RenderHintPointIndexOfCurveAndTimeLerpHintMap.end(); ++l_Iterator)
	{
		float l_fSize = l_Iterator->second * l_fRadius;
		auto l_vPos = m_CurveWithTimeKeyPosition[l_Iterator->first];
		RenderSphere(Vector2(l_vPos.x, l_vPos.y), l_fSize);
	}
	for (auto l_Iterator = m_CollisionPointIndexOfCurveAndTimeLerpHintMap.begin(); l_Iterator != m_CollisionPointIndexOfCurveAndTimeLerpHintMap.end(); ++l_Iterator)
	{
		auto l_vPos = m_CurveWithTimeKeyPosition[l_Iterator->first];
		float l_fSize = l_Iterator->second * l_fRadius;
	}
	if (m_pDeugLineImage && m_pCurveWithTime)
	{
		int l_iNumVertices = 0;
		const int l_ciTest = 9999;
		Vector3 l_OutPos[l_ciTest];
		Vector2 l_vOutUV[l_ciTest];
		Vector4 l_vOutColor[l_ciTest];
		m_pDeugLineImage->GenerateCurveTriangulatorRenderDataForBatchRendering(m_pCurveWithTime, l_iNumVertices, l_OutPos, l_vOutUV, l_vOutColor, true);
	}
}