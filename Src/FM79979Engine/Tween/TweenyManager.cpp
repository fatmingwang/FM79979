#include "pch.h"
#include "TweenyManager.h"

TYPDE_DEFINE_MARCO(cTweenyManager)

cTweenyObject::~cTweenyObject()
{
}

void cTweenyObject::AddTweeny(tweeny::easing e_easing,Vector2 e_vStart, Vector2 e_vEnd, int e_iSteps, float e_fDuration, std::function<void(Vector2)> e_Function)
{
	auto tween = tweeny::from(0, 0.0f).to(2, 2.0f).during(100).onStep(
	[e_Function](int i, float f)
	{
			Vector2 l_Vector2(i,f);
			e_Function(l_Vector2);
			return false; 
	});
}

void cTweenyObject::AddTweeny(Vector3 e_vStart, Vector3 e_vEnd, int e_iSteps, float e_fDuration, std::function<void(Vector3)> e_Function)
{
}

void cTweenyObject::AddTweeny(float e_fStart,float e_End, int e_iSteps, float e_fDuration, std::function<void(float)> e_Function)
{
}

void cTweenyObject::AddTweeny(int e_iStart, int e_iEnd, int e_iSteps, float e_fDuration, std::function<void(float)> e_Function)
{
}


cTweenyManager::cTweenyManager()
{

}
cTweenyManager::~cTweenyManager()
{

}

unsigned int	cTweenyManager::RequireUniqueID()
{
	static unsigned int l_suiID = 0;
	++l_suiID;
	return l_suiID;
}

void	cTweenyManager::Update(float e_fElpaseTime)
{

}