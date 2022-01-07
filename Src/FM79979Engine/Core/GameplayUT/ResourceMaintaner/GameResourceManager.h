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
		eGBT_DEFAULT_RESOURCE_PARSER,//cRenderObject*GetRenderObjectBehavior(TiXmlElement*e_pElement);
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
		//this one will 
		cNamedTypedObjectVector<cRenderObject>					m_DefaultResourceParser;
		//
		std::vector<cNamedTypedObjectVector<NamedTypedObject>* >m_GameObjectManagerVector;
		void													AddGameObjectVector(cNamedTypedObjectVector<NamedTypedObject>*e_pObject, eGameObjectType e_eGameObjectType);
	public:
		cGameResourceManager();
		virtual ~cGameResourceManager();
		template<class TYPE>TYPE*							GetParser(eGameObjectType e_eGameObjectType)
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
		template<class T>T* GetObjectByDefaultResourceParser(const wchar_t* e_strObjectName)
		{
			return dynamic_cast<T*>(m_GameObjectManagerVector[eGBT_DEFAULT_RESOURCE_PARSER]->GetObject(e_strObjectName));
		}
		template<class T>T* GetObjectByDefaultResourceParserAndObjectType(const wchar_t* e_strObjectName)
		{
			int l_iCount = m_GameObjectManagerVector[eGBT_DEFAULT_RESOURCE_PARSER]->Count();
			for (int i = 0; i < l_iCount; ++i)
			{
				auto l_pObject = (*m_GameObjectManagerVector[eGBT_DEFAULT_RESOURCE_PARSER])[i];
				if (l_pObject->IsSameName(e_strObjectName) && l_pObject->Type() == T::TypeID)
				{
					return dynamic_cast<T*>(l_pObject);
				}
			}
			return nullptr;
		}
		bool	ParseLazyResource(char*e_strFileName);
	};
//end namespace FATMING_CORE
}