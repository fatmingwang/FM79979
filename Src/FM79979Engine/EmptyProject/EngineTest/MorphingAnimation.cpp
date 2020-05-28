#include "stdafx.h"
#include "MorphingAnimation.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cFMMorphingAnimation);
	TYPDE_DEFINE_MARCO(cFMMorphingAnimationManager);
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

	void cFMMorphingAnimation::InvertOrder()
	{
	}

	float cFMMorphingAnimation::GetEndTime()
	{
		return 0.0f;
	}
	cFMMorphingAnimationManager::cFMMorphingAnimationManager(c2DMeshObjectManager*e_p2DMeshObjectManager)
	{
		m_pAnimationDataMap = new cAnimationDataMap(this);
	}

	cFMMorphingAnimationManager::~cFMMorphingAnimationManager()
	{
		SAFE_RELEASE(m_pAnimationDataMap, this);
	}

	NamedTypedObject * cFMMorphingAnimationManager::GetObjectByFileName(const char * e_strFileName)
	{
		return nullptr;
	}

	cFMMorphingAnimationManager::cAnimationDataMap::cAnimationDataMap(NamedTypedObject * e_pObject)
		:cSmartObject(e_pObject)
	{
	}

	cFMMorphingAnimationManager::cAnimationDataMap::~cAnimationDataMap()
	{
	}
//<MorphingAnimationRoot Version="20200522">
//    <MorphingAnimation Name="15686247788274" TimeList="0.00000,0.25000,0.50000,1.00000" VertexCount="56">
//        <VertexData Index="0" PosVectorDataSize="48" OptimizeTimeVectorDataSize="4" OptimizePosVectorDataSize="12" />
//        <VertexData Index="1" PosVectorDataSize="48" OptimizeTimeVectorDataSize="4" OptimizePosVectorDataSize="12" />
//    </MorphingAnimation>
//</MorphingAnimationRoot>
	bool cFMMorphingAnimationManager::MyParse(TiXmlElement * e_pRoot)
	{
		auto l_strVersion = e_pRoot->Attribute(L"Version");
		auto l_strPI_morphing = e_pRoot->Attribute(CHAR_TO_WCHAR_DEFINE(MX_ELEMENT_NAME));
		if (!l_strPI_morphing)
		{
			this->m_strErrorMsg += CHAR_TO_WCHAR_DEFINE(MX_ELEMENT_NAME);
			this->m_strErrorMsg += L" attribute not exists!";
			return false;
		}
		//<MorphingAnimationRoot Version = "20200522" PI_tri = "111.ti">
		if(l_strVersion)
		{
			if (GetInt(l_strVersion) == FM_MORPHING_ANIMATION_VER)
			{
				e_pRoot = e_pRoot->FirstChildElement();
				while (e_pRoot)
				{
					if (!wcscmp(e_pRoot->Value(), L"MorphingAnimation"))
					{
						Process_MorphingAnimationData(e_pRoot);
					}
					e_pRoot = e_pRoot->NextSiblingElement();
				}
			}
		}
		return false;
	}

	bool cFMMorphingAnimationManager::Process_MorphingAnimationData(TiXmlElement*e_pTiXmlElement)
	{
		return false;
	}
}