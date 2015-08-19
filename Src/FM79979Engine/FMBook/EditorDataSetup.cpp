#include "stdafx.h"
#include "GameApp.h"

cEventObjectInstance*					sFMBookEditorData::m_spEventObjectInstance = 0;
cEventInstance*							sFMBookEditorData::m_spEventInstance = 0;
cEventPackage*							sFMBookEditorData::m_spEventPackage = 0;

void	sFMBookEditorData::SetupResource()
{
	//
	cEventPackage*l_pEventPackage = g_pEditorMode->m_spEventPackageRunner->GetCurrentEventPackage();
	if( l_pEventPackage )
	{
		delete l_pEventPackage;
	}
	//
	m_spEventPackage = new cEventPackage();
	m_spEventObjectInstance = new cEventObjectInstance();
	m_spEventInstance = new cEventInstance();

	m_spEventPackage->SetName(L"EditorTest");
	m_spEventObjectInstance->SetName(L"EditorTest");
	m_spEventInstance->SetName(L"EditorTest");
	//m_spEventObjectInstance will be delete by m_spEventPackage.
	m_spEventPackage->AddObject(sFMBookEditorData::m_spEventObjectInstance);
	//m_spEventInstance will delete by m_spEventObjectInstance
	m_spEventObjectInstance->AddEventInstance(sFMBookEditorData::m_spEventInstance,false);
	g_pEditorMode->m_spEventPackageRunner->SetCurrentEventPackage(sFMBookEditorData::m_spEventPackage);
}

void	sFMBookEditorData::DestroyResource()
{
	SAFE_DELETE(m_spEventPackage);
	m_spEventObjectInstance = 0;
	m_spEventInstance = 0;
}

void	sFMBookEditorData::DataLegalCheck()
{
	if( !m_spEventPackage )
	{
		sFMBookEditorData::SetupResource();
	}
}

sFMBookEditorData::sFMBookEditorData()
{
	SetupResource();
}

sFMBookEditorData::~sFMBookEditorData()
{
	//all data is store at g_pEditorMode->m_spEventPackageRunner so do not kill it.
}



void	sFMBookEditorData::AddEventPackage(cEventPackage*e_pEventPackage)
{
	//cNodeISAX*g_pEditorMode->m_spEventManager->m_pEventObjectStatusXml;
	//cNodeISAX*g_pEditorMode->m_spEventManager->m_pEventInstanceXml;
	//cNodeISAX*l_pNodeISAX = g_pEditorMode->m_spEventManager->m_pVariableManagerXml;
	TiXmlElement*l_pRootTiXmlElement = g_pEditorMode->m_spEventManager->GetRootElement();
	if( !l_pRootTiXmlElement )
	{
		//<EventPackages FontFile="EventFont" 
		//ChangePackageVariableName="CurrentPackageName"
		//EventObjectStatusFileName="EventObjectStatus.xml"
		//EventVariableFileName="VariableData.xml"
		//EventInstanceFileName="EventInstance.xml">

		l_pRootTiXmlElement = new TiXmlElement(L"EventPackages");
		TiXmlDocument*l_pDoc = new TiXmlDocument();
		l_pDoc->LinkEndChild(l_pRootTiXmlElement);
		g_pEditorMode->m_spEventManager->SetDoc(l_pDoc);
		g_pEditorMode->m_spEventManager->SetRootElement(l_pRootTiXmlElement);
		l_pRootTiXmlElement->SetAttribute(CHANGE_PACKAGE_VARIABLE_NAME,L"CurrentPackageName");
		l_pRootTiXmlElement->SetAttribute(EVENT_OBJECT_STATUS_FILE_NAME,L"EventObjectStatus.xml");
		l_pRootTiXmlElement->SetAttribute(EVENT_VARIABLE_FILE_NAME,L"EventVariableData.xml");
		l_pRootTiXmlElement->SetAttribute(EVENT_INSTANCE_FILE_NAME,L"EventInstance.xml");
	}
	const WCHAR*l_strNewPackageName = e_pEventPackage->GetName();
	TiXmlElement*l_pOriginalTiXmlElement = g_pEditorMode->m_spEventManager->GetChildOfRootXmlElementByName(l_strNewPackageName);
	//remove old and add new one
	if( l_pOriginalTiXmlElement )
	{
		if(!l_pRootTiXmlElement->RemoveChild(l_pOriginalTiXmlElement))
		{
			UT::ErrorMsg(L"remove old node failed!!??",l_strNewPackageName);
		}
	}
	TiXmlElement*l_pNewElement = e_pEventPackage->ToTiXmlElement();
	l_pRootTiXmlElement->LinkEndChild(l_pNewElement);
}


void	sFMBookEditorData::DelEventPackage(WCHAR*e_strName)
{
	TiXmlElement*l_pRootTiXmlElement = g_pEditorMode->m_spEventManager->GetRootElement();
	if( !l_pRootTiXmlElement )
	{
		UT::ErrorMsg(L"no root element!!??",L"Warning");
		return;
	}
	assert(l_pRootTiXmlElement);
	TiXmlElement*l_pTargetTiXmlElement = g_pEditorMode->m_spEventManager->GetChildOfRootXmlElementByName(e_strName);
	if(!l_pRootTiXmlElement->RemoveChild(l_pTargetTiXmlElement))
	{
		UT::ErrorMsg(L"DelEventPackage failed!!??",e_strName);
	}
}

void	ReplaceDocumentwithNewElement(cNodeISAX*e_pOriginalNodeISAX,TiXmlElement*e_pTiXmlElement)
{
	TiXmlDocument*l_pOriginal = e_pOriginalNodeISAX->GetDoc();
	TiXmlDocument*l_pDoc = new TiXmlDocument();;
	l_pDoc->LinkEndChild(e_pTiXmlElement);
	if( l_pOriginal )
	{
		delete l_pOriginal;
	}
	e_pOriginalNodeISAX->SetDoc(l_pDoc);
}

void	sFMBookEditorData::SaveFile(char*e_strName)
{
	std::string	l_strDirectory = UT::GetDirectoryWithoutFileName(e_strName);
	TiXmlElement*l_pRootTiXmlElement = g_pEditorMode->m_spEventManager->GetRootElement();
	if( !l_pRootTiXmlElement )
	{
		UT::ErrorMsg(L"no root element!",L"Failed to save");
		return;
	}
	//for packages
	{
		if( !l_pRootTiXmlElement->Parent() )
		{
			UT::ErrorMsg(L"no root element!",L"Failed to save");
			return;
		}
	}
	//for eventinstance
	{
		TiXmlElement*l_pEventInstanceTiXmlElement = new TiXmlElement(L"EventInstanceList");
		int	l_iCount = g_pEditorMode->m_spEventManager->m_pEventInstanceList->Count();
		for( int i=0;i<l_iCount;++i )
		{
			TiXmlElement*l_pEventInstance = g_pEditorMode->m_spEventManager->m_pEventInstanceList->GetObject(i)->ToTiXmlElement();
			if( !l_pEventInstance )
			{
				UT::ErrorMsg(g_pEditorMode->m_spEventManager->m_pEventInstanceList->GetObject(i)->Type(),L"to tixml element not implement");
			}
			else
			{
				l_pEventInstanceTiXmlElement->LinkEndChild(l_pEventInstance);
			}
		}
		ReplaceDocumentwithNewElement(g_pEditorMode->m_spEventManager->m_pEventInstanceXml,l_pEventInstanceTiXmlElement);
	}
	//for eventObjectinstance
	{
		TiXmlElement*l_pEventObjectsTiXmlElement = new TiXmlElement(L"EventObjectStatusList");
		int	l_iCount = g_pEditorMode->m_spEventManager->m_pEventObjectStatusList->Count();
		for( int i=0;i<l_iCount;++i )
		{
			TiXmlElement*l_pEventStatus = g_pEditorMode->m_spEventManager->m_pEventObjectStatusList->GetObject(i)->ToTiXmlElement();
			if( !l_pEventStatus )
			{
				UT::ErrorMsg(g_pEditorMode->m_spEventManager->m_pEventObjectStatusList->GetObject(i)->Type(),L"to tixml element not implement");
			}
			else
			{
				l_pEventObjectsTiXmlElement->LinkEndChild(l_pEventStatus);
			}
		}
		ReplaceDocumentwithNewElement(g_pEditorMode->m_spEventManager->m_pEventObjectStatusXml,l_pEventObjectsTiXmlElement);
	}
	//event variable
	{
		//std::string	l_strVariableName = l_strDirectory.c_str();
		//l_strVariableName += "/EventVariableData.xml";
		//g_pEditorMode->m_spEventManager->m_pEventVariableManager->Export((char*)l_strVariableName.c_str());
	}
	g_pEditorMode->m_spEventManager->Export(e_strName);
}

//for editor use,clean xml data to fill m_pEventInstanceList,m_pEventObjectStatusList
void	ConvertXMLDataToListAndCleanXmlData()
{
	cEventManager*l_pEventManager = g_pEditorMode->m_spEventManager;
	//if( l_pEventManager->GetRootElement() )
	//{
	//	TiXmlElement*l_pEventEveintPackageRoot = l_pEventManager->GetRootElement()
	//}
	if( l_pEventManager->m_pEventObjectStatusXml )
	{
		TiXmlElement*l_pEventStatusRoot = l_pEventManager->m_pEventObjectStatusXml->GetRootElement();
		if( l_pEventStatusRoot )
		{
			l_pEventStatusRoot = l_pEventStatusRoot->FirstChildElement();
			if( l_pEventStatusRoot )
			{
				while( l_pEventStatusRoot )
				{
					cEventObjectStatus*l_pEventObjectStatus = g_pEditorMode->m_spEventManager->GetEventObjectStatus(l_pEventStatusRoot);
					if( l_pEventObjectStatus )
					{
						bool	l_b = l_pEventManager->m_pEventObjectStatusList->AddObject(l_pEventObjectStatus);
						if( !l_b )
						{
							//UT::ErrorMsg(l_pEventObjectStatus->GetName(),L"exist");
						}
					}
					l_pEventStatusRoot = l_pEventStatusRoot->NextSiblingElement();
				}
				
			}
		}
	}
	if( l_pEventManager->m_pEventInstanceXml )
	{
		TiXmlElement*l_pEventInstanceRoot = g_pEditorMode->m_spEventManager->m_pEventInstanceXml->GetRootElement();
		if( l_pEventInstanceRoot )
		{
			l_pEventInstanceRoot = l_pEventInstanceRoot->FirstChildElement();
			while( l_pEventInstanceRoot )
			{				
				cEventInstance*l_pEventInstance = g_pEditorMode->m_spEventManager->GetEventInstance(l_pEventInstanceRoot);
				if( l_pEventInstance )
				{
					bool	l_b = l_pEventManager->m_pEventInstanceList->AddObject(l_pEventInstance);
					if( !l_b )
					{
						//UT::ErrorMsg(l_pEventInstance->GetName(),L"exist");
					}
				}
				l_pEventInstanceRoot = l_pEventInstanceRoot->NextSiblingElement();
			}
		}
	}
	//variable xml do not need to assign from xml to m_pEventVariableManager
}

bool	sFMBookEditorData::OpenFile(char*e_strName)
{
	 g_pEditorMode->Destroy();
	 if(g_pEditorMode->m_spEventManager->ParseDataIntoXMLNode(e_strName))
	 {
		sFMBookEditorData::DestroyResource();
		g_pEditorMode->m_spEventPackageRunner->SetCurrentEventPackage(0);
		g_pEditorMode->m_spEventManager->m_pEventPackageRunner->LoadPackage();
		ConvertXMLDataToListAndCleanXmlData();
		SetupResource();
		return true;
	 }
	 return false;
}