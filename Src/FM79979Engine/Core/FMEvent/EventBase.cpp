#include "../stdafx.h"
#include "EventBase.h"
#include "EventInstance.h"
namespace FATMING_CORE
{
	const wchar_t*     cEventBase::TypeID( L"cEventBase" );
	cEventBase::cEventBase(){ m_bSatisfiedCondition = false; m_pEventInstance = 0; }
	cEventBase::~cEventBase(){}
	//cEventInstance*	m_pEventInstance;
	void	cEventBase::Update(float e_fElpasedTime)
	{
		if( m_bSatisfiedCondition )
			return;
		InternalUpdate(e_fElpasedTime);
	}
	void	cEventBase::ForceRender()
	{
		InternalRender(); 
	}
	void	cEventBase::Render()
	{
		if( m_bSatisfiedCondition )
			return;
		InternalRender();
	}
	void	cEventBase::Init()
	{
		cClickMouseBehavior::Init();
		m_bSatisfiedCondition = false;
		InternalInit();
	}

	bool    cEventBase::IsResourceSame(cEventBase*e_pEventBase)
    {
        if( this->GetResourcePointer() && e_pEventBase->GetResourcePointer() )
        {
            if( this->GetResourcePointer() == e_pEventBase->GetResourcePointer() )
            {
                return true;
            }
        }
        return false;
    }

	void	cEventBase::CollectResourceList(cNamedTypedObjectVector<NamedTypedObject>*e_pObjectListByName)
	{
		NamedTypedObject*l_pNamedTypedObject = GetResourcePointer();
		if( l_pNamedTypedObject )
			e_pObjectListByName->AddObject(l_pNamedTypedObject);
	}
	TiXmlElement*		cEventBase::ToTiXmlElement()
	{
		UT::ErrorMsg(this->Type(),L"WriteElement hasn't implement!!");
		return 0;
	}

	TiXmlElement*		cEventBase::GetMyTiXmlElement()
	{
		TiXmlElement*l_pEvent = new TiXmlElement(this->Type());
		l_pEvent->SetAttribute(NAME,this->GetName());
		return l_pEvent;
	}

	void	cEventBase::FetchData(const WCHAR*e_strPhaseName,NamedTypedObject*e_pNamedTypedObject)
	{
		if( wcscmp(e_strPhaseName,cEventInstance::TypeID) == 0 )
			m_pEventInstance = dynamic_cast<cEventInstance*>(e_pNamedTypedObject);
		else
		{
			assert(0&&"FetchData(const WCHAR*e_strPhaseName,cEventInstance*e_pEventInstance)");
		}
//		if( e_pNamedTypedObject->Type() == cEventInstance::TypeID )
//			m_pEventInstance = dynamic_casr<cEventInstance*>(e_pNamedTypedObject);
//		else
//			m_pEventInstance = (NamedTypedObject*)e_pNamedTypedObject;
	}
//end namespace
}