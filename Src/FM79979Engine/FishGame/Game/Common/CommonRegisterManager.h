#pragma once

#include <functional>
#include <list>

class cFishGameBaseObject:public cRenderObject
{
public:
	virtual ~cFishGameBaseObject(){}
	static cFishGameBaseObject*GetMe(TiXmlElement*e_pTiXmlElement) { return nullptr; }
};

//class cTest:public cFishGameBaseObject
//{
//public:
//	cTest(TiXmlElement*e_pTiXmlElement){}
//	virtual ~cTest(){}
//	static cFishGameBaseObject*GetMe(TiXmlElement*e_pTiXmlElement) { return nullptr; }
//};

typedef std::function<cFishGameBaseObject*(TiXmlElement*)>					CloneFunctionRegister;
#define	REGISTER_CLONE_FUNCTION_TO_MANGER(TYPE)	m_RegisterList.push_back(std::bind(&TYPE::GetMe, std::placeholders::_1))

class cFishGameCommonRegisterManager
{
	std::list<CloneFunctionRegister>				m_RegisterList;
public:
	cFishGameCommonRegisterManager(){}
	virtual ~cFishGameCommonRegisterManager(){}
	virtual void	AddCloneRegisterFunction();
	void	GameEffectManager::AddCloneRegisterFunction() = 0;
	//{
	//	REGISTER_CLONE_FUNCTION_TO_MANGER(cTest);
	//}
};