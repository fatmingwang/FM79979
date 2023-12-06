#include "pch.h"
#include "TweenyManager.h"
#include "InnerTweenBinder.h"
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

cTweenyTestObject::cTweenyTestObject()
{
	this->SetName(L"cTweenyTestObject");
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
