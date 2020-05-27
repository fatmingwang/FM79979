#pragma once
#include "2DMesh.h"
#include "MorphingAnimation.h"
namespace FATMING_CORE
{
	class cGameResourceManager
	{
	public:
		enum eGameObjectType
		{
			eGBT_2D_MESH_OBJECT,
			eGBT_2D_MORPHING_ANIMATION,
			eGBT_MPDILIST,
			eGBT_SOUND,
			eGBT_IMAGE,
			eGBT_PARTICLE,
			eGBT_BEHAVIOR_PARTICLE,
			eGBT_MAX
		};
	protected:
		cNamedTypedObjectVector<c2DMeshObjectManager>			m_2DMeshObjectManagerVector;
		cNamedTypedObjectVector<cFMMorphingAnimationManager>	m_FMMorphingAnimationManager;
		cNamedTypedObjectVector<cAnimationParser>				m_MPDIListParser;
		cNamedTypedObjectVector<cSoundParser>					m_SoundParser;
		cNamedTypedObjectVector<cImageParser>					m_ImageParser;
		cNamedTypedObjectVector<cPaticleManager>				m_PaticleManager;
		cNamedTypedObjectVector<cBehaviorPaticleManager>		m_BehaviorPaticleManager;
		//
		std::vector<cNamedTypedObjectVector<NamedTypedObject>* >m_GameObjectManagerVector;
		void													AddGameObjectVector(cNamedTypedObjectVector<NamedTypedObject>*e_pObject, eGameObjectType e_eGameObjectType);
	public:
		~cGameResourceManager();
		cGameResourceManager();
		template<class T>T*										GetParser(eGameObjectType e_eGameObjectType)
		{
			return dynamic_cast<T*>(m_GameObjectManagerVector[e_eGameObjectType]);
		}
		template<class T>T*									GetObject(const wchar_t*e_strObjectName, eGameObjectType e_eGameObjectType)
		{
			return dynamic_cast<T*>(m_GameObjectManagerVector[e_eGameObjectType]->GetObject(e_strObjectName));
		}
		template<class T>T*									GetObjectByFileName(const char*e_strObjectName, eGameObjectType e_eGameObjectType)
		{
			return dynamic_cast<T*>(m_GameObjectManagerVector[e_eGameObjectType]->GetObject(e_strObjectName));
		}
	};
}