#include "pch.h"
#include "TweenyManager.h"

TYPDE_DEFINE_MARCO(cTweenyManager)

class cInnerTweenyManager;
cTweenyManager*cTweenyManager::m_spInstance = nullptr;
unsigned int	RequireTweenUniqueID()
{
	static unsigned int l_suiID = 0;
	++l_suiID;
	return l_suiID;
}

int GetStepsByDuration(float e_fDuration)
{
	//why 35 is smooth?
	//int l_iFPS = 144;
	int l_iFPS = 35;
	int l_iSteps = (int)(e_fDuration * l_iFPS);
	return l_iSteps;
}

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
struct sTweenBinder:public NamedTypedObject
{
	UT::sTimeCounter	m_TC;
	int					m_iCurrentSteps = 0;
	int					m_iTargetSteps = 0;
	TWEEN_TYPE			m_Tween;
	unsigned int		m_uiID;
	std::function<void(unsigned int)>	m_FinishFunction;
	sTweenBinder(float e_fDuration, int e_iSteps, TWEEN_TYPE e_Tween, tweeny::easing::enumerated e_easing,unsigned int e_uiID);
	virtual ~sTweenBinder()
	{
		//FMLOG("test");
	}
	//
	bool		Update(float e_fElpaseTime)
	{
		m_TC.Update(e_fElpaseTime);
		float l_fLERP = m_TC.GetLERP();
		int l_iCurrentSteps = (int)(l_fLERP* m_iTargetSteps);
		int l_iSteps = l_iCurrentSteps-m_iCurrentSteps;
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
};


template<class MAP>void	MapDoFor(MAP& e_Map,float e_fElpaseTime)
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
public	:
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
		if (m_IDAneTweenFloatMap.size() == 0 && m_IDAneTweenIntMap.size() == 0 && m_IDAneTween2FloatMap.size() == 0 && m_IDAneTween3FloatMap.size() == 0 )
		{
			cTweenyManager::DestroyInstance();
		}
	}
};

void cTweenyManager::DestroyInstance()
{
	SAFE_DELETE(m_spInstance);
}

cTweenyManager* cTweenyManager::GetInstance()
{
	if (!m_spInstance)
	{
		m_spInstance = new cInnerTweenyManager();
	}
	return m_spInstance;
}


cTweenyObject::~cTweenyObject()
{
	while (m_TweenyIDVector.size())
	{
		auto l_uiID = m_TweenyIDVector[0];
		cInnerTweenyManager::GetInstance()->Erase(l_uiID);
		m_TweenyIDVector.erase(m_TweenyIDVector.begin());
	}
	cInnerTweenyManager::GetInstance()->DeleteIfNoData();
}

unsigned int cTweenyObject::AddTweeny(tweeny::easing::enumerated e_easing,Vector2 e_vStart, Vector2 e_vEnd, float e_fDuration, std::function<void(Vector2)> e_Function, std::function<void()> e_FinishFunction)
{
	int l_iSteps = GetStepsByDuration(e_fDuration);
	auto l_Tween = tweeny::from(e_vStart.x, e_vStart.y).to(e_vEnd.x, e_vEnd.y).during(l_iSteps);
	auto l_uiID = RequireTweenUniqueID();
	auto l_pTweenBinder = new sTweenBinder<tweeny::tween<float, float>>(e_fDuration, l_iSteps, l_Tween, e_easing, l_uiID);
	cInnerTweenyManager::GetInstance()->m_IDAneTween2FloatMap.insert({l_uiID ,l_pTweenBinder});
	l_pTweenBinder->m_Tween.onStep
	(
		[e_Function](float e_fX, float e_fY)
		{
			Vector2 l_Vector2(e_fX, e_fY);
			e_Function(l_Vector2);
			return false;
		}
	);
	l_pTweenBinder->m_FinishFunction = std::bind(&cTweenyObject::TweenFinish, this, std::placeholders::_1);	
	m_TweenyIDVector.emplace_back(l_uiID);
	if (e_FinishFunction)
	{
		m_IDAndFinishFunction[l_uiID] = e_FinishFunction;
	}
	return l_uiID;
}

unsigned int cTweenyObject::AddTweeny(tweeny::easing::enumerated e_easing, Vector3 e_vStart, Vector3 e_vEnd, float e_fDuration, std::function<void(Vector3)> e_Function, std::function<void()> e_FinishFunction)
{
	int l_iSteps = GetStepsByDuration(e_fDuration);
	auto l_Tween = tweeny::from(e_vStart.x, e_vStart.y, e_vStart.z).to(e_vEnd.x, e_vEnd.y, e_vEnd.z).during(l_iSteps);
	auto l_uiID = RequireTweenUniqueID();
	auto l_pTweenBinder = new sTweenBinder<tweeny::tween<float, float, float>>(e_fDuration, l_iSteps, l_Tween, e_easing, l_uiID);
	cInnerTweenyManager* l_pInnerTweenyManager = (cInnerTweenyManager*)cTweenyManager::GetInstance();
	l_pInnerTweenyManager->m_IDAneTween3FloatMap.insert({ l_uiID ,l_pTweenBinder });
	l_pTweenBinder->m_Tween.onStep
	(
		[e_Function](float e_fX, float e_fY, float e_fZ)
		{
			Vector3 l_Vector3(e_fX, e_fY, e_fZ);
			e_Function(l_Vector3);
			return false;
		}
	);
	l_pTweenBinder->m_FinishFunction = std::bind(&cTweenyObject::TweenFinish, this, std::placeholders::_1);
	m_TweenyIDVector.emplace_back(l_uiID);
	if (e_FinishFunction)
	{
		m_IDAndFinishFunction[l_uiID] = e_FinishFunction;
	}
	return l_uiID;
}

unsigned int cTweenyObject::AddTweeny(tweeny::easing::enumerated e_easing, float e_fStart,float e_fEnd, float e_fDuration, std::function<void(float)> e_Function, std::function<void()> e_FinishFunction)
{
	int l_iSteps = GetStepsByDuration(e_fDuration);
	auto l_Tween = tweeny::from(e_fStart).to(e_fEnd).during(l_iSteps);
	auto l_uiID = RequireTweenUniqueID();
	auto l_pTweenBinder = new sTweenBinder<tweeny::tween<float>>(e_fDuration, l_iSteps, l_Tween, e_easing, l_uiID);
	cInnerTweenyManager* l_pInnerTweenyManager = (cInnerTweenyManager*)cTweenyManager::GetInstance();
	l_pInnerTweenyManager->m_IDAneTweenFloatMap.insert({ l_uiID ,l_pTweenBinder });
	l_pTweenBinder->m_Tween.onStep
	(
		[e_Function](float e_fX)
		{
			e_Function(e_fX);
			return false;
		}
	);
	l_pTweenBinder->m_FinishFunction = std::bind(&cTweenyObject::TweenFinish, this, std::placeholders::_1);
	m_TweenyIDVector.emplace_back(l_uiID);
	if(e_FinishFunction)
	{
		m_IDAndFinishFunction[l_uiID] = e_FinishFunction;
	}
	return l_uiID;
}

unsigned int cTweenyObject::AddTweeny(tweeny::easing::enumerated e_easing, int e_iStart, int e_iEnd, float e_fDuration, std::function<void(int)> e_Function, std::function<void()> e_FinishFunction)
{
	int l_iSteps = GetStepsByDuration(e_fDuration);
	auto l_Tween = tweeny::from(e_iStart).to(e_iEnd).during(l_iSteps);
	auto l_uiID = RequireTweenUniqueID();
	auto l_pTweenBinder = new sTweenBinder<tweeny::tween<int>>(e_fDuration, l_iSteps, l_Tween, e_easing, l_uiID);
	cInnerTweenyManager* l_pInnerTweenyManager = (cInnerTweenyManager*)cTweenyManager::GetInstance();
	l_pInnerTweenyManager->m_IDAneTweenIntMap.insert({ l_uiID ,l_pTweenBinder });
	l_pTweenBinder->m_Tween.onStep
	(
		[e_Function](int e_iX)
		{
			e_Function(e_iX);
			return false;
		}
	);
	l_pTweenBinder->m_FinishFunction = std::bind(&cTweenyObject::TweenFinish, this, std::placeholders::_1);
	m_TweenyIDVector.emplace_back(l_uiID);
	if (e_FinishFunction)
	{
		m_IDAndFinishFunction[l_uiID] = e_FinishFunction;
	}
	return l_uiID;
}

void cTweenyObject::ChancelTween(unsigned int e_uiID, bool e_bCallFinishFunction, bool e_bDeleteTween)
{
	auto l_IT = m_IDAndFinishFunction.find(e_uiID);
	if (l_IT != m_IDAndFinishFunction.end())
	{
		if (l_IT->second && e_bCallFinishFunction)
		{
			l_IT->second();
		}
		m_IDAndFinishFunction.erase(l_IT);
	}
	if (m_TweenyIDVector.size() > 0)
	{
		m_TweenyIDVector.erase(std::remove(m_TweenyIDVector.begin(), m_TweenyIDVector.end(), e_uiID), m_TweenyIDVector.end());
	}
	if (e_bDeleteTween)
	{
		cInnerTweenyManager* l_pInnerTweenyManager = (cInnerTweenyManager*)cTweenyManager::GetInstance();
		l_pInnerTweenyManager->Erase(e_uiID);
	}
}

void cTweenyObject::TweenFinish(unsigned int e_uiID)
{
	this->ChancelTween(e_uiID, true,false);
}


cTweenyManager::cTweenyManager()
{

}
cTweenyManager::~cTweenyManager()
{

}

void	cTweenyManager::Update(float e_fElpaseTime)
{
	cInnerTweenyManager::GetInstance()->InnerUpdate(e_fElpaseTime);
}


template<class TWEEN_TYPE>
sTweenBinder<TWEEN_TYPE>::sTweenBinder(float e_fDuration, int e_iSteps, TWEEN_TYPE e_Tween, tweeny::easing::enumerated e_easing, unsigned int e_uiID)
{
	m_Tween = e_Tween;
	SetEasing<TWEEN_TYPE>(m_Tween, e_easing);
	m_Tween.seek(0);
	m_iTargetSteps = e_iSteps;
	m_TC.SetTargetTime(e_fDuration);
	m_iCurrentSteps = 0;
	m_uiID = e_uiID;
}

cTweenyTestObject::cTweenyTestObject()
{
}

//void cTweenyTestObject::Render()
//{
//	for (auto l_vPos : m_vTestVector)
//	{
//		//GLRender::RenderSphere(l_vPos, 30);
//	}
//}

const wchar_t* ValueToString(tweeny::easing::enumerated e_easing)
{
	switch (e_easing)
	{
		LAZY_EASING_STRING(linear);
		LAZY_EASING_STRING(stepped);
		LAZY_EASING_STRING(quadraticIn);
		LAZY_EASING_STRING(quadraticOut);
		LAZY_EASING_STRING(quadraticInOut);
		LAZY_EASING_STRING(cubicIn);
		LAZY_EASING_STRING(cubicOut);
		LAZY_EASING_STRING(cubicInOut);
		LAZY_EASING_STRING(quarticIn);
		LAZY_EASING_STRING(quarticOut);
		LAZY_EASING_STRING(quarticInOut);
		LAZY_EASING_STRING(quinticIn);
		LAZY_EASING_STRING(quinticOut);
		LAZY_EASING_STRING(quinticInOut);
		LAZY_EASING_STRING(sinusoidalIn);
		LAZY_EASING_STRING(sinusoidalOut);
		LAZY_EASING_STRING(sinusoidalInOut);
		LAZY_EASING_STRING(exponentialIn);
		LAZY_EASING_STRING(exponentialOut);
		LAZY_EASING_STRING(exponentialInOut);
		LAZY_EASING_STRING(circularIn);
		LAZY_EASING_STRING(circularOut);
		LAZY_EASING_STRING(circularInOut);
		LAZY_EASING_STRING(bounceIn);
		LAZY_EASING_STRING(bounceOut);
		LAZY_EASING_STRING(bounceInOut);
		LAZY_EASING_STRING(elasticIn);
		LAZY_EASING_STRING(elasticOut);
		LAZY_EASING_STRING(elasticInOut);
		LAZY_EASING_STRING(backIn);
		LAZY_EASING_STRING(backOut);
		LAZY_EASING_STRING(backInOut);
		//default tweeny::easing::enumerated::def :
	default:
		return L"unknow easing type";
	}
	return nullptr;
}

void	cTweenyTestObject::Test(tweeny::easing::enumerated e_enumerated)
{
	static bool l_sbTest = false;
	if (!l_sbTest)
	{
		Vector2 l_vTest = Vector2::Zero;
		l_vTest.y = 500;
		l_vTest.x = 300;
		m_vTestVector.push_back(l_vTest);
		l_sbTest = true;
		m_TweenyObject.AddTweeny(e_enumerated, 300.f, 2000.f, 5,
		[this](float e_fValue)
		{
			m_vTestVector[0].x = e_fValue;
		},
		[this]()
		{
			m_vTestVector[0].x = 2000.f;
			SetTimeoutByCommonApp([this]()
				{
					m_vTestVector.clear();
					l_sbTest = false;
				}, 3.f);
		});
	}
}

void	cTweenyTestObject::TestAll()
{
	static bool l_sbTest = false;
	if (!l_sbTest)
	{
		static int l_iNumFinish = 0;
		for (int i = 1; i < (int)tweeny::easing::enumerated::backInOut; ++i)
		{
			Vector2 l_vTest = Vector2::Zero;
			l_vTest.y = 100 + i * 60.f;
			l_vTest.x = 300;
			m_vTestVector.push_back(l_vTest);
			l_sbTest = true;
			int l_iType = i;
			m_TweenyObject.AddTweeny((tweeny::easing::enumerated)l_iType, 300.f, 2000.f, 5,
				[this, i](float e_fValue)
				{
					m_vTestVector[i - 1].x = e_fValue;
				},
				[this, i]()
				{
					m_vTestVector[i - 1].x = 2000.f;
					++l_iNumFinish;
					if (l_iNumFinish == m_vTestVector.size())
					{
						SetTimeoutByCommonApp([this]()
							{
								m_vTestVector.clear();
								l_sbTest = false;
								l_iNumFinish = 0;
							}, 3.f);
					}
				});
		}
	}
}

void cTweenyTestObject::KeyUp()
{
	//this->Test(tweeny::easing::enumerated::quadraticInOut);
	TestAll();
}
