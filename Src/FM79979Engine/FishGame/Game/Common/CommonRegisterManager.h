#pragma once

#include <functional>
#include <list>

#define	REGISTER_CLONE_FUNCTION_TO_MANGER(TYPE)	m_RegisterList.push_back(std::bind(&TYPE::GetMe, std::placeholders::_1))
template<class TYPE>
class cFishGameCommonRegisterManager
{
protected:
	std::list<std::function<TYPE*(TiXmlElement*)>>				m_RegisterList;
	virtual TYPE*									GetObject(TiXmlElement*e_pTiXmlElement);
public:
	cFishGameCommonRegisterManager(){}
	virtual ~cFishGameCommonRegisterManager(){}
	virtual void	AddCloneRegisterFunction() = 0;
	//virtual void	AddCloneRegisterFunction() = 0;
	//{
	//	REGISTER_CLONE_FUNCTION_TO_MANGER(cTest);
	//}
};
template<class TYPE>
TYPE*cFishGameCommonRegisterManager<TYPE>::GetObject(TiXmlElement*e_pTiXmlElement)
{
	TYPE*l_pNamedTypedObject = nullptr;
	for (std::function<TYPE*(TiXmlElement*)> l_CloneFunctionRegister : m_RegisterList)
	{
		l_pNamedTypedObject = l_CloneFunctionRegister(e_pTiXmlElement);
		if (l_pNamedTypedObject)
			return l_pNamedTypedObject;
	}
#ifdef DEBUG
	std::wstring l_str = e_pTiXmlElement->Value();
	l_str += L" not registered element";
	cGameApp::OutputDebugInfoString(l_str);
#endif
	return nullptr;
}

//class cTest :public NamedTypedObject
//{
//public:
//	cTest(TiXmlElement*e_pTiXmlElement) {}
//	virtual ~cTest() {}
//	static cTest*GetMe(TiXmlElement*e_pTiXmlElement) { return nullptr; }
//};

//void	cFishGameCommonRegisterManager::AddCloneRegisterFunction()
//{
//	REGISTER_CLONE_FUNCTION_TO_MANGER(cTest);
//}