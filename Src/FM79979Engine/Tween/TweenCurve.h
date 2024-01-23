#pragma once

#include "../Core/AllCoreInclude.h"
#include "./TweenyManager.h"

template<class TYPE>struct sTweenBinder;

class cTweenyCurveWithTime :public cRenderObject
{
protected:
	std::map<int, float>					m_CollisionPointIndexOfCurveAndTimeLerpHintMap;
	std::map<int, float>					m_RenderSphereOfHintPointIndexOfCurveAndTimeLerpHintMap;
	int										m_iIgnoreAfterIndexForCollisionAndRender = -1;
	std::vector<Vector3>					m_CurveWithTimeKeyPosition;
	//
	sTweenBinder<tweeny::tween<float>>*		m_pTweenBinder = nullptr;
	cCurveWithTime*							m_pCurveWithTime = nullptr;
	float									m_fTimeForCheckCollision = 1.f;
	float									m_fTimeForRenderHint = 1.f;
	UT::sTimeCounter						m_DelayTC;
	float									m_fGenerateRenderCurveTime = 4.f;
	float									m_fTotalTime = 0;
	float									m_fElpaseTime = 0;
public:
	cBaseImage*								m_pDeugLineImage = nullptr;
	void									UpdateHintInfo(float efCurveCurrentTime);
public:
	cTweenyCurveWithTime(float e_fTimeForCheckCollision = 1.f, float e_fTimeForRenderHint = 1.f,const char*e_strDebugLineFileName = nullptr);
	virtual ~cTweenyCurveWithTime();
	void									Update(float e_fElpaseTime);
	virtual void							Render()override;
	void									Render2(float e_fSphereRadius = 50.f,Vector4 e_vSphereColor = Vector4::Green);
	//
	cCurveWithTime*							GetCurve();
	void									SetTween(tweeny::easing::enumerated e_easing, float e_fDuration, std::function<void(unsigned int)>e_FinishFunction);
	void									SetCurve(cCurveWithTime* e_pCurveWithTime);
	void									SetData(tweeny::easing::enumerated e_easing, float e_fDuration, cCurveWithTime* e_pCurveWithTime, std::function<void(unsigned int)>e_FinishFunction,float e_fDelayToStsartTime, float e_fGenerateRenderCurveTime);
	//
	void									Reset();
	bool									IsStart();
	bool									IsTimeLegal(float e_fOffsetTime);
	//
	std::map<int, float>& GetCollisionPointIndexOfCurveAndTimeLerpHintMap() { return	m_CollisionPointIndexOfCurveAndTimeLerpHintMap; }
	std::map<int, float>& GetRenderHintPointIndexOfCurveAndTimeLerpHintMap() { return	m_RenderSphereOfHintPointIndexOfCurveAndTimeLerpHintMap; }
};