#include "../stdafx.h"
#include "EventHeaderDefine.h"
#include "../AllCoreInclude.h"
#include "../Particle/AllParticleInclude.h"
#include "EventManager.h"
#include "EventPackageRunner.h"
#ifdef ANDROID
#include <unistd.h>
#endif
namespace FATMING_CORE
{
	cEventManager*g_pCurrentEventManager = 0;
    cEventManager::cEventManager()
    {
		m_pEventObjectStatusXml = 0;
		m_pEventInstanceXml = 0;
		m_pVariableManagerXml = 0;
		m_pEventFont = 0;
		m_pExternalEventPackage = new cNamedTypedObjectVector<cEventPackage>;
		m_pEventInstanceList = new cNamedTypedObjectVector<cEventInstance>;
		m_pEventObjectStatusList = new cNamedTypedObjectVector<cEventObjectStatus>;
		m_pEventVariableManager = new cEventVariableManager();
		m_pExternalFunctionList = new cNamedTypedObjectVector<cExternalFunction>;
		m_pPrtGList = new cPrtGList();
		m_pstrChangePachageName = new std::wstring;
		m_pstrWorkingDirectory = new std::string;
		m_pEventPackageRunner = new cEventPackageRunner(this);
    }

    cEventManager::~cEventManager()
    {
		SAFE_DELETE(m_pExternalEventPackage);
		SAFE_DELETE(m_pExternalFunctionList);
		SAFE_DELETE(m_pEventObjectStatusXml);
		SAFE_DELETE(m_pEventInstanceXml);
		SAFE_DELETE(m_pVariableManagerXml);
		SAFE_DELETE(m_pEventInstanceList);
		SAFE_DELETE(m_pEventObjectStatusList);
		SAFE_DELETE(m_pEventVariableManager);
		SAFE_DELETE(m_pPrtGList);
        SAFE_DELETE(m_pEventFont);
		SAFE_DELETE(m_pstrChangePachageName);
		SAFE_DELETE(m_pstrWorkingDirectory);
		SAFE_DELETE(m_pEventPackageRunner);
    }

	std::string	cEventManager::GetFullpathFileName(const WCHAR*e_strName)
	{
		std::string	l_str;
		char	l_strTemp[MAX_PATH];
		sprintf(l_strTemp,"%s%s",m_pstrWorkingDirectory->c_str(),UT::WcharToChar(e_strName).c_str());
		l_str = l_strTemp;
		return l_str;
	}

	bool	cEventManager::AddExternFunctionName(const WCHAR*e_strName,DoButtonGoal_Callback e_pDoButtonGoal_Callback)
	{
		cExternalFunction*l_pExternalFunction = new cExternalFunction();
		l_pExternalFunction->SetName(e_strName);
		l_pExternalFunction->m_pDoButtonGoal_Callback = e_pDoButtonGoal_Callback;
		return m_pExternalFunctionList->AddObject(l_pExternalFunction);
	}

	cEventObjectInstance*	cEventManager::GetEventObjectInstanceFromCurrentEventPackage(const WCHAR*e_strName)
	{
		cEventObjectInstance*l_pEventObjectInstance = 0;
		if( m_pEventPackageRunner )
		{
			if(m_pEventPackageRunner->GetCurrentEventPackage())
			{
				l_pEventObjectInstance = m_pEventPackageRunner->GetCurrentEventPackage()->GetObject(e_strName);
			}
		}
		return l_pEventObjectInstance;
	}

	//<EventPackages ChangePackageVariableName="oxox" EventObjectStatusFileName="" EventVariableFileName="" EventInstanceFileName="">
	bool	cEventManager::ParseDataIntoXMLNode(const char*e_strFileName)
	{
		*m_pstrChangePachageName = L"";
		*m_pstrWorkingDirectory = "";
		cGameApp::m_siShowErrorType = 0;
		m_pPrtGList->Destroy();
		m_pExternalFunctionList->Destroy();
		m_pEventInstanceList->Destroy();
		m_pEventObjectStatusList->Destroy();
		m_pEventVariableManager->Destroy();
		SAFE_DELETE(m_pEventFont);
		SAFE_DELETE(m_pEventObjectStatusXml);
		SAFE_DELETE(m_pEventInstanceXml);
		SAFE_DELETE(m_pVariableManagerXml);
		m_pEventObjectStatusXml = new cNodeISAX;
		m_pEventInstanceXml = new cNodeISAX;
		m_pVariableManagerXml = new cNodeISAX;
		std::string	l_strFileName;
		if(cNodeISAX::ParseDataIntoXMLNode(e_strFileName))
		{
			*m_pstrWorkingDirectory = this->m_strCurrentDirectory;
			PARSE_ELEMENT_START(this->m_pRootElement)
				COMPARE_NAME_WITH_DEFINE(CHANGE_PACKAGE_VARIABLE_NAME)
				{
					*m_pstrChangePachageName = l_strValue;
				}
				else
				COMPARE_NAME_WITH_DEFINE(EVENT_OBJECT_STATUS_FILE_NAME)
				{
					l_strFileName = this->m_strCurrentDirectory;
					l_strFileName += UT::WcharToChar(l_strValue);
					if(!m_pEventObjectStatusXml->ParseDataIntoXMLNode(l_strFileName.c_str()))
					{
						UT::ErrorMsg(l_strValue,L"not exist!");
					}
				}
				else
				COMPARE_NAME_WITH_DEFINE(EVENT_VARIABLE_FILE_NAME)
				{
					l_strFileName = this->m_strCurrentDirectory;
					l_strFileName += UT::WcharToChar(l_strValue);
					if(!m_pVariableManagerXml->ParseDataIntoXMLNode(l_strFileName.c_str()))
					{
						UT::ErrorMsg(l_strValue,L"not exist!");
					}
					else
						m_pEventVariableManager->Parse(l_strFileName.c_str());
				}
				else
				COMPARE_NAME_WITH_DEFINE(EVENT_INSTANCE_FILE_NAME)
				{
					l_strFileName = this->m_strCurrentDirectory;
					l_strFileName += UT::WcharToChar(l_strValue);
					if(!m_pEventInstanceXml->ParseDataIntoXMLNode(l_strFileName.c_str()))
					{
						UT::ErrorMsg(l_strValue,L"not exist!");
					}
				}
				else
				COMPARE_NAME_WITH_DEFINE(EVENT_FONT_FILE_NAME)
				{
					l_strFileName = this->m_strCurrentDirectory;
					l_strFileName += UT::WcharToChar(l_strValue);
					m_pEventFont = new cGlyphFontRender(l_strFileName.c_str(),2000);
				}
			PARSE_NAME_VALUE_END
			cGameApp::m_siShowErrorType = 1;
			if(wcslen(cGameApp::m_spstrErrorMsgString->c_str()))
			{
				UT::ErrorMsg(cGameApp::m_spstrErrorMsgString->c_str(),UT::CharToWchar(e_strFileName));
			}
			cGameApp::m_spstrErrorMsgString->clear();
			return true;
		}
		cGameApp::m_siShowErrorType = 1;
		UT::ErrorMsg("Packages data error",e_strFileName);
		return false;
	}

//	<EventPackage Name="">
//		<EventObjectInstance Name="" EventInstance="" EventObjectStatus="" >
//		<EventObjectInstance Name="" EventInstance="" EventObjectStatus="" >
//	</EventPackage>
	cEventPackage*		cEventManager::GetEventPackage(TiXmlElement*e_pTiXmlElement)
	{
		if( !e_pTiXmlElement )
			return 0;
		cEventPackage*l_pEventPackage = new cEventPackage();
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,EVENT_PACKAGE);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				l_pEventPackage->SetName(l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(BGCOLOR)
			{
				l_pEventPackage->SetBGColor(VALUE_TO_VECTOR4);
			}
		PARSE_NAME_VALUE_END
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while(e_pTiXmlElement)
		{
			cEventObjectInstance*l_pEventObjectInstance = GetEventObjectInstance(e_pTiXmlElement);
			l_pEventPackage->AddObjectNeglectExist(l_pEventObjectInstance);
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}
		return l_pEventPackage;
	}

	void		cEventManager::RemoveResource(NamedTypedObject*e_pResource)
	{
		const WCHAR*l_str = e_pResource->Type();
		if( e_pResource->Type() == cMPDIList::TypeID )
		{
			cGameApp::m_spAnimationParser->RemoveObject(e_pResource);
		}
		else
		if( e_pResource->Type() == cPuzzleImage::TypeID )
		{
			if(cGameApp::m_spAnimationParser->NumObjectUsingPI(dynamic_cast<cPuzzleImage*>(e_pResource)) == 0)
				cGameApp::m_spImageParser->RemoveObject(e_pResource);
		}
		else
		if( e_pResource->Type() == cBaseImage::TypeID || e_pResource->Type() == cImageButton::TypeID )
		{
			cGameApp::m_spImageParser->RemoveObject(e_pResource);
		}
		else
		if( e_pResource->Type() == cOpanalWAV::TypeID || e_pResource->Type() == cOpanalOgg::TypeID )
		{
			cBasicSound*l_pBasicSound = cGameApp::m_spSoundParser->GetObject(e_pResource->GetName());
			if( !l_pBasicSound->IsLoop() )
			{
//				if( l_pBasicSound->IsPlay() )
//				{
//					Sleep(1000);
//				}
				l_pBasicSound->Play(false);
				//cGameApp::m_spSoundParser->RemoveObject(l_pBasicSound);

			}
		}
		else
		{
			UT::ErrorMsg(e_pResource->Type(),L"such type resource can't be delete !?");
		}
	}

	bool		cEventManager::IsEventInstanceElementExist(const WCHAR*e_strName)
	{
		if( !m_pEventInstanceXml )
			return false;
		return m_pEventInstanceXml->GetChildOfRootXmlElementByAttribueName(cEventInstance::TypeID,e_strName,NAME)?true:false;
	}

	bool		cEventManager::IsEventObjectStatusElementExist(const WCHAR*e_strName)
	{
		if( !m_pEventObjectStatusXml )
			return false;
		return m_pEventObjectStatusXml->GetChildOfRootXmlElementByAttribueName(cEventInstance::TypeID,e_strName,NAME)?true:false;
	}

	cEventObjectInstance*		cEventManager::AddEventObjectInstanceToCurrentPackage(const WCHAR*e_strEventObjectInstanceName,const WCHAR*e_strEventInstanceName,const WCHAR*e_strEventObjectStatusName,bool e_bAllStatusWorking,const WCHAR*e_strEventVariableName)
	{
		if(!GetEventObjectInstanceFromCurrentEventPackage(e_strEventObjectInstanceName))
		{
			cEventPackage*l_pEventPackage = this->m_pEventPackageRunner->GetCurrentEventPackage();
			if( l_pEventPackage )
			{
				cEventObjectInstance*l_pEventObjectInstance = new cEventObjectInstance();
				if(e_strEventObjectInstanceName)
					l_pEventObjectInstance->SetName(e_strEventObjectInstanceName);
				if( e_strEventInstanceName )
					l_pEventObjectInstance->ProcessEventInstance(e_strEventInstanceName);
				if( e_strEventObjectStatusName )
					l_pEventObjectInstance->ProcessEventObjectStatus(e_strEventObjectStatusName);
				l_pEventObjectInstance->SetAllStatusWorking(e_bAllStatusWorking);
				if( e_strEventVariableName )
					l_pEventObjectInstance->ProcessStatusEventVariable(e_strEventVariableName);
				l_pEventPackage->AddObjectNeglectExist(l_pEventObjectInstance);
				return l_pEventObjectInstance;
			}			
		}
		return 0;
	}

	bool	cEventManager::Export(char*e_strFileName)
	{
		if(!this->m_pDoc)
		{
			return false;
		}
		std::wstring	l_strDirectory = UT::CharToWchar(UT::GetDirectoryWithoutFileName(e_strFileName));
		std::wstring	l_strFileName = UT::GetFileNameWithoutFullPath(UT::CharToWchar(e_strFileName));
		if(!this->m_pRootElement ||!this->m_pEventObjectStatusXml||!this->m_pEventInstanceXml||!this->m_pVariableManagerXml||!this->m_pEventFont )
		{
			return false;
		}
		std::wstring	l_strFullFileName;
		std::wstring	l_strFullName;

		m_pRootElement->SetAttribute(L"FontFile",L"EventFont");
		m_pRootElement->SetAttribute(CHANGE_PACKAGE_VARIABLE_NAME,L"CurrentPackageName");

		l_strFullFileName = l_strDirectory.c_str();
		l_strFullFileName  += l_strFileName.c_str();	l_strFullName = l_strFileName.c_str();
		l_strFullFileName  += L"EventObjectStatus.xml";	l_strFullName += L"EventObjectStatus.xml";
		m_pRootElement->SetAttribute(EVENT_OBJECT_STATUS_FILE_NAME,l_strFullName.c_str());
		m_pEventObjectStatusXml->Export(l_strFullFileName .c_str());

		l_strFullFileName = l_strDirectory.c_str();
		l_strFullFileName  += l_strFileName.c_str();	l_strFullName = l_strFileName.c_str();
		l_strFullFileName  += L"EventVariable.xml";		l_strFullName += L"EventVariable.xml";
		m_pRootElement->SetAttribute(EVENT_VARIABLE_FILE_NAME,l_strFullName.c_str());
		m_pVariableManagerXml->Export(l_strFullFileName .c_str());

		l_strFullFileName = l_strDirectory.c_str();
		l_strFullFileName  += l_strFileName.c_str();	l_strFullName = l_strFileName.c_str();
		l_strFullFileName  += L"EventInstance.xml";		l_strFullName += L"EventInstance.xml";
		m_pRootElement->SetAttribute(EVENT_INSTANCE_FILE_NAME,l_strFullName.c_str());
		m_pEventInstanceXml->Export(l_strFullFileName .c_str());

		return ISAXCallback::Export(e_strFileName);
		//return this->m_pDoc->SaveFile(e_strFileName);
	}
//end namespace FATMING_CORE
}