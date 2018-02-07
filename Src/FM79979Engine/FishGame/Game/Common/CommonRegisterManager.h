#pragma once

#include <functional>
#include <list>

typedef std::function<NamedTypedObject*(TiXmlElement*)>					CloneFunctionRegister;
#define	REGISTER_CLONE_FUNCTION_TO_MANGER(TYPE)	m_RegisterList.push_back(std::bind(&TYPE::GetMe, std::placeholders::_1))

class cFishGameCommonRegisterManager
{
protected:
	std::list<CloneFunctionRegister>				m_RegisterList;
	virtual NamedTypedObject*						GetObject(TiXmlElement*e_pTiXmlElement);
public:
	cFishGameCommonRegisterManager(){}
	virtual ~cFishGameCommonRegisterManager(){}
	virtual void	AddCloneRegisterFunction() = 0;
	//virtual void	AddCloneRegisterFunction() = 0;
	//{
	//	REGISTER_CLONE_FUNCTION_TO_MANGER(cTest);
	//}
};

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