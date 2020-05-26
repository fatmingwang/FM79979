#include "stdafx.h"
#include "MorphingAnimation.h"
namespace FATMING_CORE
{
	Vector3 cFMMorphingAnimation::sTimeAndPosAnimationData::UpdateAnimationByGlobalTime(float e_fGlobalTime)
	{
		return Vector3();
	}
	void cFMMorphingAnimation::sTimeAndPosAnimationData::RearrangeTime(float e_fScale)
	{
	}
	void cFMMorphingAnimation::sTimeAndPosAnimationData::RenderPointByTime(float e_fTime, Vector4 e_vColor, float e_fPointSize)
	{
	}
	void cFMMorphingAnimation::InternalInit()
	{
	}

	void cFMMorphingAnimation::InternalUpdate(float e_fElpaseTime)
	{
	}

	void cFMMorphingAnimation::InternalRender()
	{
	}

	cFMMorphingAnimation::cFMMorphingAnimation(c2DMeshObject::sMeshBuffer * e_pMeshBuffer)
	{
	}

	cFMMorphingAnimation::cFMMorphingAnimation(cFMMorphingAnimation * e_pFMMorphingAnimation)
	{
	}

	cFMMorphingAnimation::~cFMMorphingAnimation()
	{
	}


	void cFMMorphingAnimation::UpdateByGlobalTime(float e_fGlobalTime)
	{
	}

	void cFMMorphingAnimation::RenderByGlobalTime(float e_fTime)
	{
	}

	void cFMMorphingAnimation::RearrangeTime(float e_fNewTime)
	{
	}

	void cFMMorphingAnimation::RearrangeTimeByPercent(float e_fPercenttage)
	{
	}

	float cFMMorphingAnimation::GetEndTime()
	{
		return 0.0f;
	}
	cFMMorphingAnimationManager::cFMMorphingAnimationManager()
	{
		m_pAnimationDataMap = new cAnimationDataMap(this);
	}

	cFMMorphingAnimationManager::~cFMMorphingAnimationManager()
	{
		SAFE_RELEASE(m_pAnimationDataMap, this);
	}

	cFMMorphingAnimationManager::cAnimationDataMap::cAnimationDataMap(NamedTypedObject * e_pObject)
		:cSmartObject(e_pObject)
	{
	}

	cFMMorphingAnimationManager::cAnimationDataMap::~cAnimationDataMap()
	{
	}

	bool cFMMorphingAnimationManager::MyParse(TiXmlElement * e_pRoot)
	{
		return false;
	}
}