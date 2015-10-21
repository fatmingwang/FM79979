#ifndef BASE_EVENT_H
#define BASE_EVENT_H

#include "../GameplayUT/PhaseControl.h"
#include "EventHeaderDefine.h"
namespace FATMING_CORE
{
	#define	EVENT_CLONE_DEFINE(Type)	CLONE_MYSELF(Type)
	class	cEventInstance;
	class	cEventBase:public cSimplePhase
	{
	protected:
		cEventInstance*				m_pEventInstance;
		virtual	void				InternalInit() = 0;
		virtual	void				InternalUpdate(float e_fElpasedTime) = 0;
		virtual	void				InternalRender(){}
	public:
		DEFINE_TYPE_INFO();
		cEventBase();
		virtual ~cEventBase();
		virtual	void				Init();
		virtual	void				Update(float e_fElpasedTime);
		virtual	void				Render();
		//while m_bSatisfiedCondition is true render is skip but ForceRender call internal render
		virtual	void				ForceRender();
		//to get current info
		virtual	void				DebugRender(){}
		virtual	const WCHAR*		GetNextPhaseName(){ return 0; }
        //if resource is same return true or false
        bool						IsResourceSame(cEventBase*e_pEventBase);
		virtual	void				CollectResourceList(cNamedTypedObjectVector<NamedTypedObject>*e_pObjectListByName);
        virtual NamedTypedObject*   GetResourcePointer(){return 0;}
		virtual	TiXmlElement*		ToTiXmlElement();
		TiXmlElement*				GetMyTiXmlElement();
		virtual	void				FetchData(const WCHAR*e_strPhaseName,NamedTypedObject*e_pNamedTypedObject);
	};
	//TiXmlDocument doc;
	//TiXmlElement*l_pEvent = ToTiXmlElement();
	//doc.LinkEndChild(l_pEvent);
	//doc.SaveFile("778899.xml");
	//===================================================
	//quick copy sample
	//===================================================
	//class	cEventTest:public cEventBase
	//{
	//	virtual	void	InternalInit();
	//	virtual	void	InternalUpdate(float e_fElpasedTime);
	//	virtual	void	InternalRender();
	//public:
	//	DEFINE_TYPE_INFO();
	//	cEventTest(TiXmlDocument*e_pTiXmlDocument);
	//	cEventTest();
	//	cEventTest(cEventTest*e_pEventTest);
	//}
	//TYPDE_DEFINE_MARCO(cEventTest);

	//cEventTest::cEventTest(TiXmlDocument*e_pTiXmlDocument)
	//{
	//
	//}
	//cEventTest::cEventTest()
	//{
	//
	//}

	//cEventTest::cEventTest(cEventTest*e_pEventTest)
	//{
	//
	//}

	//void	cEventTest::InternalInit()
	//{

	//}

	//void	cEventTest::InternalUpdate(float e_fElpasedTime)
	//{

	//}

	//void	cEventTest::InternalRender()
	//{

	//}
	
//end FATMING_CORE
}
#endif