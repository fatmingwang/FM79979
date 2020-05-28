#include "stdafx.h"
#include "GameResourceManager.h"

void FATMING_CORE::cGameResourceManager::AddGameObjectVector(cNamedTypedObjectVector<NamedTypedObject>* e_pObject, eGameObjectType e_eGameObjectType)
{
	//assert(m_TypeIDAndGameObjectType.find(e_pObject->Type()) == m_TypeIDAndGameObjectType.end() && "same TypeID!!!");
	//m_TypeIDAndGameObjectType[e_pObject->Type()] = e_eGameObjectType;
	assert(m_GameObjectManagerVector[e_eGameObjectType] == nullptr);
	m_GameObjectManagerVector[e_eGameObjectType] = e_pObject;
}

cGameResourceManager::~cGameResourceManager()
{
	m_GameObjectManagerVector.resize(eGBT_MAX);
	AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*)&m_2DMeshObjectManagerVector, eGameObjectType::eGBT_2D_MESH_OBJECT);
	AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*)&m_FMMorphingAnimationManager, eGameObjectType::eGBT_2D_MORPHING_ANIMATION);
	AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*)&m_MPDIListParser, eGameObjectType::eGBT_MPDILIST);
	AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*)&m_SoundParser, eGameObjectType::eGBT_SOUND);
	AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*)&m_ImageParser, eGameObjectType::eGBT_IMAGE);
	AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*)&m_PaticleManager, eGameObjectType::eGBT_PARTICLE);
	AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*)&m_BehaviorPaticleManager, eGameObjectType::eGBT_BEHAVIOR_PARTICLE);
	AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*)&m_FontManager, eGameObjectType::eGBT_FONT);
	//auto l_pData = GetObjectByFileName<cMPDI>("79979", eGameObjectType::eGBT_MPDILIST);
	//auto l_pData2 = GetObject<cMPDI>(L"6699", eGameObjectType::eGBT_MPDILIST);
	//auto l_pp = GetParser<cNamedTypedObjectVector<cAnimationParser>>(eGameObjectType::eGBT_MPDILIST);
}
cGameResourceManager::cGameResourceManager()
{
}