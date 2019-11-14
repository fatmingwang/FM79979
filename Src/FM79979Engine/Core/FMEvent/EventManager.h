#ifndef _FMBOOK_MANAGER_H_
#define _FMBOOK_MANAGER_H_

#include "EventVariable.h"
#include "EventObjectInstance.h"
namespace FATMING_CORE
{
	#define	NEW_EVENT_BT_XML_ELEMENT(Type,Element) COMPARE_VALUE_WITH_DEFINE(Type::TypeID){ l_pEventBase = new Type(Element); }else

	class	cEventObjectInstance;
	class	cEventPackage;
	class	cEventVariable;
	//EventInstance.xml
	//EventObjectStatus.xml
	//<EventPackages FontFile="EventFont" ChangePackageVariableName="CurrentPackageName" EventObjectStatusFileName="EventObjectStatus.xml" EventVariableFileName="VariableData.xml" EventInstanceFileName="EventInstance.xml">
	//	<EventPackage Name="FirstPage">
	//		<cEventObjectInstance Name="GeneratorTest" cEventInstance="EventTest" cEventObjectStatus="Generator" />
	//		<cEventObjectInstance Name="GeneratorTest3" cEventInstance="MinusBetLine" />
	//		<cEventObjectInstance Name="GeneratorTest2" cEventInstance="EventTest2" cEventObjectStatus="ShowLinePlayable" />
	//	</EventPackage>
	//	<EventPackage Name="SecondPage">
	//		<cEventObjectInstance Name="GeneratorTest" cEventInstance="EventTest" cEventObjectStatus="Generator" />
	//	</EventPackage>
	//	<EventPackage Name="ThirdPage">
	//		<cEventObjectInstance Name="GeneratorTest" cEventInstance="MinusBetLine" cEventObjectStatus="Generator" />
	//	</EventPackage>
	//	<EventPackage Name="FourthPage">
	//		<cEventObjectInstance Name="GeneratorTest2" cEventInstance="EventTest2" cEventObjectStatus="ShowLinePlayable" />
	//	</EventPackage>
	//</EventPackages>
	//EventPachages.xml
	//VariableData.xml
	//why keep cEventObjectStatus,m_pEventInstanceList data?
	//because most resource are store at them,so keep those data and erase if we do not need,and use again as need
	//
	//

	//
	//while parse new package is called,collect all using resource from Old ackage and compare with new package
	//fin out unnecessary resource and release it!.take a c of
	//PackageRunner
	//PackageRunner::LoadPackage
	//PackageRunner::RemoveUnUsingEventObjectStatusAtEventManager
	//PackageRunner::RemoveUnUsingEventInstanceAtEventManager,RemoveUnUsingResourceAtEventManager
	//
	class	cEventPackageRunner;
	class   cEventManager:public cNodeISAX
    {
		friend class									cEventPackageRunner;
		//
		void											ParseVariableManagerData(char*e_strFileName);
		void											ParseEventObjectStatusData(char*e_strFileName);
		void											EventInstanceAssignEventList(TiXmlElement*e_pElement,cClickMouseBehaviorVector<cEventBase>*e_pList,bool e_bTrigger);
        //void    Export(char*e_strFileName);
        //int     GetNumPage();
		void											RemoveResource(NamedTypedObject*e_pResource);
		//cEventObjectInstance*							GetEventObjectInstance(const WCHAR*e_strEventObjectInstanceName,const WCHAR*e_strEventInstanceName,const WCHAR*e_strEventObjectStatusName,bool e_bAllStatusWorking,const WCHAR*e_strEventVariableName);
    public:
		virtual	bool									ParseDataIntoXMLNode(const char*e_strFileName);
        cEventManager();
        virtual ~cEventManager();

		cNodeISAX*										m_pEventObjectStatusXml;
		cNodeISAX*										m_pEventInstanceXml;
		cNodeISAX*										m_pVariableManagerXml;
		//
        cGlyphFontRender*								m_pEventFont;
		cEventVariableManager*							m_pEventVariableManager;
		cNamedTypedObjectVector<cEventInstance>*		m_pEventInstanceList;
		cNamedTypedObjectVector<cEventObjectStatus>*	m_pEventObjectStatusList;
		cNamedTypedObjectVector<cExternalFunction>*		m_pExternalFunctionList;
		//for programming not xml
		cNamedTypedObjectVector<cEventPackage>*			m_pExternalEventPackage;
		std::string*									m_pstrWorkingDirectory;
		cPrtGList*										m_pPrtGList;
		std::wstring*									m_pstrChangePachageName;
		cEventPackageRunner*							m_pEventPackageRunner;

		cEventObjectStatus*								GetEventObjectStatus(const WCHAR*e_strName);
		cEventInstance*									GetEventInstance(const WCHAR*e_strName);
		cEventPackage*									GetEventPackage(const WCHAR*e_strName);
		cEventVariable*									GetEventVariable(const WCHAR*e_strName);

		bool											IsEventInstanceElementExist(const WCHAR*e_strName);
		bool											IsEventObjectStatusElementExist(const WCHAR*e_strName);
		cEventObjectInstance*							AddEventObjectInstanceToCurrentPackage(const WCHAR*e_strEventObjectInstanceName,const WCHAR*e_strEventInstanceName,const WCHAR*e_strEventObjectStatusName,bool e_bAllStatusWorking,const WCHAR*e_strEventVariableName);
		//xml parse
		cEventPackage*									GetEventPackage(TiXmlElement*e_pElement);
		cEventObjectInstance*							GetEventObjectInstance(TiXmlElement*e_pElement);
		cEventObjectStatus*								GetEventObjectStatus(TiXmlElement*e_pElement,bool e_bForceRequire = false);
		cEventInstance*									GetEventInstance(TiXmlElement*e_pElement,bool e_bForceRequire = false);
		cEventObject*									GetEventObject(TiXmlElement*e_pElement);
		cEventBase*										GetActiverEvent(TiXmlElement*e_pTiXmlElement);
		cEventBase*										GetTriggerEvent(TiXmlElement*e_pTiXmlElement);
		//combine file name with directory
		std::string										GetFullpathFileName(const WCHAR*e_strName);
		bool											AddExternFunctionName(const WCHAR*e_strName,DoButtonGoal_Callback e_pDoButtonGoal_Callback);
		//quickly to get current eventpackage's data
		cEventObjectInstance*							GetEventObjectInstanceFromCurrentEventPackage(const WCHAR*e_strName);
		//
		bool											Export(const char*e_strFileName);
    };
	//do not use this directly,and there are multithread which contain 2 or more eventmanager.
	//because g_pCurrentEventManager will use internally.
	extern cEventManager*g_pCurrentEventManager;
}

#endif