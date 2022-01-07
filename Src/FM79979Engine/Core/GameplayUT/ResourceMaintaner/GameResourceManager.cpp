#include "../../stdafx.h"
#include "GameResourceManager.h"
#include "ObjectXMLParser.h"
namespace FATMING_CORE
{
	void FATMING_CORE::cGameResourceManager::AddGameObjectVector(cNamedTypedObjectVector<NamedTypedObject>* e_pObject, eGameObjectType e_eGameObjectType)
	{
		//assert(m_TypeIDAndGameObjectType.find(e_pObject->Type()) == m_TypeIDAndGameObjectType.end() && "same TypeID!!!");
		//m_TypeIDAndGameObjectType[e_pObject->Type()] = e_eGameObjectType;
		assert(m_GameObjectManagerVector[e_eGameObjectType] == nullptr);
		m_GameObjectManagerVector[e_eGameObjectType] = e_pObject;
	}

	cGameResourceManager::cGameResourceManager()
	{
		m_GameObjectManagerVector.resize(eGBT_MAX);
		m_pImageParser = m_MPDIListParser.GetAllBaseImageList();
		m_p2DMeshObjectManagerVector = m_FMMorphingAnimationManager.Get2DMeshObjectManager();
		AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*)m_p2DMeshObjectManagerVector, eGameObjectType::eGBT_2D_MESH_OBJECT);
		AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*) & m_FMMorphingAnimationManager, eGameObjectType::eGBT_2D_MORPHING_ANIMATION);
		AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*) & m_MPDIListParser, eGameObjectType::eGBT_MPDILIST);
		//AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*)&m_SoundParser, eGameObjectType::eGBT_SOUND);
		AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*)m_pImageParser, eGameObjectType::eGBT_IMAGE);
		AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*) & m_PaticleManager, eGameObjectType::eGBT_PARTICLE);
		AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*) & m_BehaviorPaticleManager, eGameObjectType::eGBT_BEHAVIOR_PARTICLE);
		AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*) & m_FontManager, eGameObjectType::eGBT_FONT);
		AddGameObjectVector((cNamedTypedObjectVector<NamedTypedObject>*) & m_DefaultResourceParser, eGameObjectType::eGBT_DEFAULT_RESOURCE_PARSER);
		//auto l_pData = GetObjectByFileName<cMPDI>("79979", eGameObjectType::eGBT_MPDILIST);
		//auto l_pData2 = GetObject<cMPDI>(L"6699", eGameObjectType::eGBT_MPDILIST);
		//auto l_pp = GetParser<cNamedTypedObjectVector<cAnimationParser>>(eGameObjectType::eGBT_MPDILIST);
	}
	cGameResourceManager::~cGameResourceManager()
	{
		m_DefaultResourceParser.Destroy();
	}
//something like this
//<Root>
//	<cNumeralImage PI="CarGoApp/JP_BBQ/UI.pi" Image0="ID-0" Image9="ID-09" Name="CarIDNumerial"/>
//</Root>
	bool	FATMING_CORE::cGameResourceManager::ParseLazyResource(char* e_strFileName)
	{
		cNodeISAX l_NodeISAX;
		if (l_NodeISAX.ParseDataIntoXMLNode(e_strFileName))
		{
			auto l_pRoot = l_NodeISAX.GetRootElement();
			if (l_pRoot)
			{
				l_pRoot = l_pRoot->FirstChildElement();
			}
			while (l_pRoot)
			{
				auto l_pRenderOnbject = GetRenderObjectBehavior(l_pRoot);
				if (l_pRenderOnbject)
				{
					m_DefaultResourceParser.AddObjectNeglectExist(l_pRenderOnbject);
				}
				l_pRoot = l_pRoot->NextSiblingElement();
			}
			return true;
		}
		return false;
	}
}