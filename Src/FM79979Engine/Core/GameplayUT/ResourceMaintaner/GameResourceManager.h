#pragma once

#include "../Render/2DMesh/2DMesh.h"
#include "../Render/Morphing/MorphingAnimation.h"
#include "../Render/Font/FontManager.h"
#include "../Render/Parser/AnimationParser.h"
#include "../Render/Particle/AllParticleInclude.h"
#include "../../Sound/SoundManager.h"

namespace FATMING_CORE
{
	enum eGameObjectType
	{
		eGBT_2D_MESH_OBJECT,
		eGBT_2D_MORPHING_ANIMATION,
		eGBT_MPDILIST,
		eGBT_SOUND,
		eGBT_IMAGE,
		eGBT_PARTICLE,
		eGBT_BEHAVIOR_PARTICLE,
		eGBT_FONT,
		eGBT_MAX
	};
	class cGameResourceManager
	{
	protected:
		c2DMeshObjectManager*									m_p2DMeshObjectManagerVector;
		cFMMorphingAnimationManager								m_FMMorphingAnimationManager;
		cPaticleManager											m_PaticleManager;
		cBehaviorPaticleManager									m_BehaviorPaticleManager;
		//cSoundParser*											m_pSoundParser;
		cAnimationParser										m_MPDIListParser;
		cImageParser*											m_pImageParser;
		cFontManager											m_FontManager;
		//
		std::vector<cNamedTypedObjectVector<NamedTypedObject>* >m_GameObjectManagerVector;
		void													AddGameObjectVector(cNamedTypedObjectVector<NamedTypedObject>*e_pObject, eGameObjectType e_eGameObjectType);
	public:
		~cGameResourceManager();
		cGameResourceManager();
		template<class TYPE>TYPE*										GetParser(eGameObjectType e_eGameObjectType)
		{
			return dynamic_cast< TYPE* >(m_GameObjectManagerVector[e_eGameObjectType]);
		}
		template<class T>T*									GetObject(const wchar_t*e_strObjectName, eGameObjectType e_eGameObjectType)
		{
			return dynamic_cast<T*>(m_GameObjectManagerVector[e_eGameObjectType]->GetObject(e_strObjectName));
		}
		template<class T>T*									GetObjectByFileName(const char*e_strObjectName, eGameObjectType e_eGameObjectType)
		{
			return dynamic_cast<T*>(m_GameObjectManagerVector[e_eGameObjectType]->GetObjectByFileName(e_strObjectName));
		}
	};
}