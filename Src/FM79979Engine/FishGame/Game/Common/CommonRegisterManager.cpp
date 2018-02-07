#include "stdafx.h"
#include "CommonRegisterManager.h"
#include "WaitForFetchFunctionObjectList.h"

NamedTypedObject*cFishGameCommonRegisterManager::GetObject(TiXmlElement*e_pTiXmlElement)
{
	NamedTypedObject*l_pNamedTypedObject = nullptr;
	for (CloneFunctionRegister l_CloneFunctionRegister : m_RegisterList)
	{
		l_pNamedTypedObject = l_CloneFunctionRegister(e_pTiXmlElement);
		if (l_pNamedTypedObject)
			return l_pNamedTypedObject;
	}
	return nullptr;
}