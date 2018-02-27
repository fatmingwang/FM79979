#include "stdafx.h"
#include "CommonRegisterManager.h"

//NamedTypedObject*cFishGameCommonRegisterManager::GetObject(TiXmlElement*e_pTiXmlElement)
//{
//	NamedTypedObject*l_pNamedTypedObject = nullptr;
//	for (CloneFunctionRegister l_CloneFunctionRegister : m_RegisterList)
//	{
//		l_pNamedTypedObject = l_CloneFunctionRegister(e_pTiXmlElement);
//		if (l_pNamedTypedObject)
//			return l_pNamedTypedObject;
//	}
//#ifdef DEBUG
//	std::wstring l_str = e_pTiXmlElement->Value();
//	l_str += L" not registered element";
//	cGameApp::OutputDebugInfoString(l_str);
//#endif
//	return nullptr;
//}