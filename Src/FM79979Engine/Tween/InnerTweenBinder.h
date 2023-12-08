#pragma once
#include "Tweeny/tweeny.h"

#define	LAZY_EASING(e_Tween,e_Type)case tweeny::easing::enumerated::##e_Type: e_Tween.via(0,tweeny::easing::##e_Type);break;
#define	LAZY_EASING_STRING(e_Type)case tweeny::easing::enumerated::##e_Type: return WSTRING(e_Type);

template<class TWEEN>
void	SetEasing(TWEEN& e_Tween, tweeny::easing::enumerated e_easing)
{
	switch (e_easing)
	{
		LAZY_EASING(e_Tween, linear);
		LAZY_EASING(e_Tween, stepped);
		LAZY_EASING(e_Tween, quadraticIn);
		LAZY_EASING(e_Tween, quadraticOut);
		LAZY_EASING(e_Tween, quadraticInOut);
		LAZY_EASING(e_Tween, cubicIn);
		LAZY_EASING(e_Tween, cubicOut);
		LAZY_EASING(e_Tween, cubicInOut);
		LAZY_EASING(e_Tween, quarticIn);
		LAZY_EASING(e_Tween, quarticOut);
		LAZY_EASING(e_Tween, quarticInOut);
		LAZY_EASING(e_Tween, quinticIn);
		LAZY_EASING(e_Tween, quinticOut);
		LAZY_EASING(e_Tween, quinticInOut);
		LAZY_EASING(e_Tween, sinusoidalIn);
		LAZY_EASING(e_Tween, sinusoidalOut);
		LAZY_EASING(e_Tween, sinusoidalInOut);
		LAZY_EASING(e_Tween, exponentialIn);
		LAZY_EASING(e_Tween, exponentialOut);
		LAZY_EASING(e_Tween, exponentialInOut);
		LAZY_EASING(e_Tween, circularIn);
		LAZY_EASING(e_Tween, circularOut);
		LAZY_EASING(e_Tween, circularInOut);
		LAZY_EASING(e_Tween, bounceIn);
		LAZY_EASING(e_Tween, bounceOut);
		LAZY_EASING(e_Tween, bounceInOut);
		LAZY_EASING(e_Tween, elasticIn);
		LAZY_EASING(e_Tween, elasticOut);
		LAZY_EASING(e_Tween, elasticInOut);
		LAZY_EASING(e_Tween, backIn);
		LAZY_EASING(e_Tween, backOut);
		LAZY_EASING(e_Tween, backInOut);
		//default tweeny::easing::enumerated::def :
	default:
		break;
	}
}

template<class TWEEN_TYPE>
struct sTweenBinder :public NamedTypedObject
{
	UT::sTimeCounter	m_TC;
	int					m_iCurrentSteps = 0;
	int					m_iTargetSteps = 0;
	TWEEN_TYPE			m_Tween;
	unsigned int		m_uiID;
	std::function<void(unsigned int)>	m_FinishFunction;
	sTweenBinder(float e_fDuration, int e_iSteps, TWEEN_TYPE e_Tween, tweeny::easing::enumerated e_easing, unsigned int e_uiID)
	{
		m_Tween = e_Tween;
		SetEasing<TWEEN_TYPE>(m_Tween, e_easing);
		m_Tween.seek(0);
		m_iTargetSteps = e_iSteps;
		m_TC.SetTargetTime(e_fDuration);
		m_iCurrentSteps = 0;
		m_uiID = e_uiID;
	}

	virtual ~sTweenBinder()
	{
		//FMLOG("test");
	}
	//
	bool		Update(float e_fElpaseTime)
	{
		if (m_TC.bTragetTimrReached)
		{
			return true;
		}
		m_TC.Update(e_fElpaseTime);
		float l_fLERP = m_TC.GetLERP();
		int l_iCurrentSteps = (int)(l_fLERP * m_iTargetSteps);
		int l_iSteps = l_iCurrentSteps - m_iCurrentSteps;
		//for debug test
		//l_iSteps = 1;
		m_Tween.step(l_iSteps);
		m_iCurrentSteps = l_iCurrentSteps;
		if (l_fLERP >= 1.f)
		{
			if (this->m_FinishFunction)
			{
				this->m_FinishFunction(m_uiID);
			}
			return true;
		}
		return false;
	}
	void	Reset()
	{
		m_Tween.seek(0);
		m_TC.Start();
		m_iCurrentSteps = 0;
	}
};

template<class MAP>void	MapDoFor(MAP& e_Map, float e_fElpaseTime)
{
	std::vector<unsigned int>l_FinishedVector;
	for (auto l_IT = e_Map.begin(); l_IT != e_Map.end(); ++l_IT)
	{
		if (l_IT->second->Update(e_fElpaseTime))
		{
			l_FinishedVector.emplace_back(l_IT->first);
		}
	}
	while (l_FinishedVector.size())
	{
		auto l_IT = e_Map.find(l_FinishedVector[0]);
		SAFE_DELETE(l_IT->second);
		e_Map.erase(l_FinishedVector[0]);
		l_FinishedVector.erase(l_FinishedVector.begin());
	}
}

template<class MAP>void	MapErase(MAP& e_Map, unsigned int e_uiID)
{
	auto l_IT = e_Map.find(e_uiID);
	if (l_IT != e_Map.end())
	{
		SAFE_DELETE(l_IT->second);
		e_Map.erase(l_IT);
	}
}

class cInnerTweenyManager :public	cTweenyManager
{
public:
	std::map<unsigned int, sTweenBinder<tweeny::tween<float>>*>					m_IDAneTweenFloatMap;
	std::map<unsigned int, sTweenBinder<tweeny::tween<int>>*>					m_IDAneTweenIntMap;
	std::map<unsigned int, sTweenBinder<tweeny::tween<float, float>>*>			m_IDAneTween2FloatMap;
	std::map<unsigned int, sTweenBinder<tweeny::tween<float, float, float>>*>	m_IDAneTween3FloatMap;
	virtual ~cInnerTweenyManager()
	{
		DELETE_MAP(m_IDAneTweenFloatMap);
		DELETE_MAP(m_IDAneTweenIntMap);
		DELETE_MAP(m_IDAneTween2FloatMap);
		DELETE_MAP(m_IDAneTween3FloatMap);
	}
	static cInnerTweenyManager* GetInstance()
	{
		return (cInnerTweenyManager*)cTweenyManager::GetInstance();
	}
	void	InnerUpdate(float e_fElpaseTime)
	{
		MapDoFor<std::map<unsigned int, sTweenBinder<tweeny::tween<float>>*>>(m_IDAneTweenFloatMap, e_fElpaseTime);
		MapDoFor<std::map<unsigned int, sTweenBinder<tweeny::tween<int>>*>>(m_IDAneTweenIntMap, e_fElpaseTime);
		MapDoFor<std::map<unsigned int, sTweenBinder<tweeny::tween<float, float>>*>>(m_IDAneTween2FloatMap, e_fElpaseTime);
		MapDoFor<std::map<unsigned int, sTweenBinder<tweeny::tween<float, float, float>>*>>(m_IDAneTween3FloatMap, e_fElpaseTime);
	}
	void	Erase(unsigned int e_uiID)
	{
		MapErase<std::map<unsigned int, sTweenBinder<tweeny::tween<float>>*>>(m_IDAneTweenFloatMap, e_uiID);
		MapErase<std::map<unsigned int, sTweenBinder<tweeny::tween<int>>*>>(m_IDAneTweenIntMap, e_uiID);
		MapErase<std::map<unsigned int, sTweenBinder<tweeny::tween<float, float>>*>>(m_IDAneTween2FloatMap, e_uiID);
		MapErase<std::map<unsigned int, sTweenBinder<tweeny::tween<float, float, float>>*>>(m_IDAneTween3FloatMap, e_uiID);
	}

	void	DeleteIfNoData()
	{
		if (m_IDAneTweenFloatMap.size() == 0 && m_IDAneTweenIntMap.size() == 0 && m_IDAneTween2FloatMap.size() == 0 && m_IDAneTween3FloatMap.size() == 0)
		{
			cTweenyManager::DestroyInstance();
		}
	}
};