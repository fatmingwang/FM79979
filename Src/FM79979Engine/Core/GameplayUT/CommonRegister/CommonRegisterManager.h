#pragma once

#include <functional>
#include <list>

#define	REGISTER_CLONE_FUNCTION_TO_MANGER(TYPE)	m_RegisterList.push_back(std::bind(&TYPE::GetMe, std::placeholders::_1))

#define	GET_ME_WITH_BASE_TYPE(TYPE,BASE_TYPE)									  \
			static BASE_TYPE*GetMe(TiXmlElement*e_pElement)						  \
			{																	  \
				if (wcscmp(e_pElement->Value() , TYPE::TypeID))					  \
					return nullptr;												  \
				BASE_TYPE*l_pBASE_TYPE = new TYPE(e_pElement);return l_pBASE_TYPE;\
			}

#define	LAZY_GET_ME(TYPE,BASE_TYPE)									  \
			static BASE_TYPE*GetMe(TiXmlElement*e_pElement)						  \
			{																	  \
				if (wcscmp(e_pElement->Value() , WSTRING(TYPE)))					\
					return nullptr;												  \
				BASE_TYPE*l_pBASE_TYPE = new TYPE(e_pElement);return l_pBASE_TYPE;\
			}
//Src is member variable Target is source,TYPE is Src class type
//LAZY_CLONE(m_pTargetFishExplosionEffectMPDI, e_pAimMonsterBullet, cMPDI);
#define	LAZY_CLONE(Src,Target,TYPE)Src = nullptr;if (Target->Src)	Src = new TYPE(Target->Src);
#define	LAZY_CLONE_WITH_INIT(Src,Target,TYPE)Src = nullptr;if (Target->Src){	Src = new TYPE(Target->Src);	Src->Init();}
#define	LAZY_CLONE_WITH_INIT_WITH_SET_PARENT(Src,Target,TYPE)Src = nullptr;if (Target->Src){Src = new TYPE(Target->Src);Src->Init();Src->SetParent(this,false);}


template<class TYPE>
class cCommonRegisterManager
{
protected:
	std::list<std::function<TYPE*(TiXmlElement*)>>				m_RegisterList;
public:
	cCommonRegisterManager(){ AddCloneRegisterFunction(); }
	virtual ~cCommonRegisterManager(){}
	virtual TYPE*									GetObjectByParseXmlElement(TiXmlElement*e_pTiXmlElement);
	void			AddCloneRegisterFunction();
	virtual void	AddRegisterFunction(std::function<TYPE*(TiXmlElement*)> e_Function)
	{
		m_RegisterList.push_back(e_Function);
	}
};
template<class TYPE>
TYPE*cCommonRegisterManager<TYPE>::GetObjectByParseXmlElement(TiXmlElement*e_pTiXmlElement)
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
	FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
#endif
	return nullptr;
}
//.h
//class cTest :public NamedTypedObject
//{
//public:
//	cTest(TiXmlElement*e_pTiXmlElement) {}
//	virtual ~cTest() {}
//	static cTest*GetMe(TiXmlElement*e_pTiXmlElement) { return nullptr; }
//	GET_ME_WITH_BASE_TYPE(cTest, cTest);
//};
//class cTest2 :public cTest
//{
//public:
//	cTest2(TiXmlElement*e_pTiXmlElement) {}
//	virtual ~cTest2() {}
//	static cTest2*GetMe(TiXmlElement*e_pTiXmlElement) { return nullptr; }
//	GET_ME_WITH_BASE_TYPE(cTest2, cTest);
//};
//class cTestManager :public cFishGameCommonRegisterManager<cTest>, public cNamedTypedObjectVector<cTest>
//{
//	void ProcessTestData(TiXmlElement*e_pElement);
//};
//.cpp
//template<>
//void	cCommonRegisterManager::AddCloneRegisterFunction()
//{
//	REGISTER_CLONE_FUNCTION_TO_MANGER(cTest);
//	REGISTER_CLONE_FUNCTION_TO_MANGER(cTest2);
//}
//void	cTestManager::ProcessTestData(TiXmlElement*e_pElement)
//{
//	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pElement)
//		cTest*l_pTest = GetObjectByParseXmlElement(e_pElement);
//		if (l_pTest)
//		{
//			this->AddObject(l_pTest);
//		}
//		else
//		{
//			UT::ErrorMsg(e_pElement->Value(), L"not support type");
//		}
//	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pElement)
//}