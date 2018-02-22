#include "stdafx.h"
#include "SmartPointer.h"
#include "XML/StringToStructure.h"
#include "GameplayUT/GameApp.h"

//bool	g_bShowSmartPointerInfo = true;
bool	g_bShowSmartPointerInfo = false;
void	DumpInfo(const wchar_t*e_strDescription, cSmartObject*e_pSmartObject, NamedTypedObject*e_pTarget)
{
	if (g_bShowSmartPointerInfo)
	{
		cGameApp::OutputDebugInfoString(L"\n================================\n");
		cGameApp::OutputDebugInfoString(e_strDescription);
		cGameApp::OutputDebugInfoString(FATMING_CORE::ValueToStringW(e_pSmartObject->GetReferenceCount()));
		cGameApp::OutputDebugInfoString(L"\n");
		if (e_pTarget)
		{
			cGameApp::OutputDebugInfoString(L"MyName:");
			cGameApp::OutputDebugInfoString(e_pSmartObject->GetSmartObjectName());
			cGameApp::OutputDebugInfoString(L"\n");
			//cGameApp::OutputDebugInfoString(L"\t\t\tType:");
			//cGameApp::OutputDebugInfoString(e_pSmartObject->Type());
			//cGameApp::OutputDebugInfoString(L"\n");
			cGameApp::OutputDebugInfoString(L"TargetName:");
			cGameApp::OutputDebugInfoString(e_pTarget->GetName());
			cGameApp::OutputDebugInfoString(L"\n");
			cGameApp::OutputDebugInfoString(L"\t\t\tType:");
			cGameApp::OutputDebugInfoString(e_pTarget->Type());
			cGameApp::OutputDebugInfoString(L",GUID:");
			cGameApp::OutputDebugInfoString(FATMING_CORE::ValueToStringW(e_pTarget->GetUniqueID()));
			cGameApp::OutputDebugInfoString(L"\n================================\n");
			if (e_pTarget->GetUniqueID() == 156)
			{
				int a = 0;
			}
		}
	}
}

cSmartObject::cSmartObject(NamedTypedObject*e_pResource)
{
	m_pRecource = e_pResource;
	m_refCount = 0;
	//AddRef(e_pResource);
}

cSmartObject::~cSmartObject()
{
	m_pRecource = nullptr;
#ifdef DEBUG
	assert(m_ReferenceList.Count() == 0);
#endif
	assert(m_refCount == 0);
}

int cSmartObject::GetReferenceCount()
{
	return m_refCount;
}

const wchar_t*cSmartObject::GetSmartObjectName()
{
	if (m_pRecource)
		return m_pRecource->GetName();
	return L"cSmartObject no resource!?";
}


void cSmartObject::AddRef(NamedTypedObject*e_pNamedTypedObject)
{
	if (e_pNamedTypedObject)
	{
		++m_refCount;
#ifdef DEBUG
		if (g_bShowSmartPointerInfo)
		{
			if (this->m_ReferenceList.GetObjectIndexByPointer(e_pNamedTypedObject) != -1)
			{
				cGameApp::OutputDebugInfoString(L"add ref twice with same object?why you want to do this!?");
			}
			DumpInfo(L"AddRefCount:", this, e_pNamedTypedObject);
		}
		m_ReferenceList.GetList()->push_back(e_pNamedTypedObject);
		assert(m_ReferenceList.Count() == m_refCount);
#endif
	}
}

int cSmartObject::Release(NamedTypedObject*e_pNamedTypedObject)
{
	if( e_pNamedTypedObject )
	{
		m_refCount--;
		int	l_refCount = m_refCount;
#ifdef DEBUG
		DumpInfo(L"Release:", this, e_pNamedTypedObject);
		int	l_iIndex = this->m_ReferenceList.GetObjectIndexByPointer(e_pNamedTypedObject);
		if( l_iIndex != -1 )
		{
			m_ReferenceList.GetList()->erase(m_ReferenceList.GetList()->begin()+l_iIndex);
		}
		assert(l_iIndex!=-1 &&"fuck no this pointer!!");
		assert(m_ReferenceList.Count() == l_refCount);
#endif
		if( l_refCount <= 0 )
		{
			//if( e_bDelete )
			{
				//if(this->GetOwner())
				//{
				//	cNamedTypedObjectVector<NamedTypedObject>*l_pNamedTypedObject = dynamic_cast<cNamedTypedObjectVector<NamedTypedObject>*>(this->GetOwner());
				//	l_pNamedTypedObject->RemoveObjectWithoutDelete(this);
				//}
				delete this;
			}
		}
		return l_refCount;
	}
#ifdef DEBUG
	else
	{
		int a=0;
	}
#endif
	assert(0&&"release null pointer!?");
	return m_refCount;
}