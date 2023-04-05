#include "Stdafx.h"
#include "FAICharacterInterface.h"
namespace FATMING_AI
{
	const wchar_t*	cFAICharacterInterface::TypeID( L"cFAICharacterInterface" );
    //const wchar_t*cFAICharacterInterface::TypeID( L"cFAICharacterInterface" );
	cFAICharacterInterface::cFAICharacterInterface(cFAICharacterInterface*e_pAICharacterInterface):Frame(e_pAICharacterInterface)
	{
		m_pAICharacterStatus = 0;
		m_fAnimationSpeed = e_pAICharacterInterface->m_fAnimationSpeed;
		m_v2DAngle = Vector3::Zero;
		m_fSpeed = e_pAICharacterInterface->m_fSpeed;
		m_fRunSpeed = e_pAICharacterInterface->m_fRunSpeed;
		if( e_pAICharacterInterface->m_pAICharacterStatus )
			this->m_pAICharacterStatus = (cFAICharacterStatus*)e_pAICharacterInterface->m_pAICharacterStatus->Clone();
	}
	//<cFAICharacterInterface Speed="" RunSpeed=""/>
    cFAICharacterInterface::cFAICharacterInterface(TiXmlElement*e_pElement)
    {
		m_v2DAngle = Vector3::Zero;
		m_fSpeed = 10.f;
		m_fAnimationSpeed = 1.f;
		m_pAICharacterStatus = 0;
		ProcessXmlData(e_pElement);
    }

	cFAICharacterInterface::cFAICharacterInterface()
	{
		m_v2DAngle = Vector3::Zero;
		m_fSpeed = 10.f;
		m_fAnimationSpeed = 1.f;
		m_pAICharacterStatus = 0;	
	}

	cFAICharacterInterface::~cFAICharacterInterface()
	{
		SAFE_DELETE(m_pAICharacterStatus);
	}

	void	cFAICharacterInterface::ProcessXmlData(TiXmlElement*e_pElement)
	{
		if( e_pElement )
		{
			TiXmlElement*l_pCharacterStatusElement = GetXmlElementByNameFromElement(cFAICharacterStatus::TypeID,e_pElement);
			if( l_pCharacterStatusElement )
				m_pAICharacterStatus = new cFAICharacterStatus(l_pCharacterStatusElement);
		}

		if( e_pElement )
		{
			const wchar_t*l_strSpeed = e_pElement->Attribute(L"Speed");
			if( l_strSpeed )
				m_fSpeed = (float)_wtof(l_strSpeed);
			const wchar_t*l_strRunAwaySpeed = e_pElement->Attribute(L"RunSpeed");
			if( l_strRunAwaySpeed )
				m_fRunSpeed = (float)_wtof(l_strRunAwaySpeed);
			else
				m_fRunSpeed = m_fSpeed;
		}	
	}

    Vector3* cFAICharacterInterface::GetPosition()
    {
        return (Vector3*)(size_t)(m_LocalTransform.m[MyMath::W]);
    }

	void	cFAICharacterInterface::DebugRender()
	{
		//NamedTypedObject*l_pNamedTypedObject = (NamedTypedObject*)this->GetCollisionData();
		//if( l_pNamedTypedObject )
		//{
		//	if( l_pNamedTypedObject->Type() == cbtConcaveShapeList::TypeID )
		//	{
		//		cbtConcaveShapeList*l_pbtConcaveShapeList = (cbtConcaveShapeList*)this->GetCollisionData();
		//		l_pbtConcaveShapeList->Render();
		//	}
		//}
	}

	bool	cFAICharacterInterface::IsCharacterBehaviorStatusLock()
	{
		if(this->m_pAICharacterStatus)
		{
			return m_pAICharacterStatus->IsCharacterBehaviorStatusLock();
		}
		return false;
	}

	bool	cFAICharacterInterface::IsCharacterBehaviorStatusLock(eFAICharacterBehaviorStatus	e_eFAICharacterBehaviorStatus)
	{
		if(this->m_pAICharacterStatus)
		{
			return m_pAICharacterStatus->IsCharacterBehaviorStatusLock(e_eFAICharacterBehaviorStatus);
		}
		return false;	
	}
}